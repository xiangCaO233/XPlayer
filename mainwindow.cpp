#include "mainwindow.h"

#include <qfilesystemmodel.h>
#include <qglobal.h>
#include <qlist.h>
#include <qstandarditemmodel.h>
#include <qstringliteral.h>
#include <qwidget.h>

#include <QFileDialog>
#include <QStandardItemModel>
#include <filesystem>
#include <memory>

#include "./ui_mainwindow.h"
#include "devicemanager.h"
#include "include/AudioManager.h"
#include "newaudioorbitdialog.h"

// 声明元数据类型
Q_DECLARE_METATYPE(std::shared_ptr<XSound>);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // 初始化最后一次打开的目录
  auto appPath = QDir(QCoreApplication::applicationDirPath());
#ifdef __APPLE__
  for (int i = 0; i < 3; i++) appPath.cdUp();
#endif  //__APPLE__
  last_select_directory = appPath.absolutePath();
  qDebug() << last_select_directory;

  // 音频文件管理器模型
  auto audio_file_browser_model =
      new QStandardItemModel(ui->audio_file_browser);
  audio_file_browser_model->setHorizontalHeaderLabels(
      QStringList() << QStringLiteral("句柄") << QStringLiteral("文件名")
                    << QStringLiteral("内存"));
  ui->audio_file_browser->setModel(audio_file_browser_model);
  ui->audio_file_browser->header()->setSectionResizeMode(
      0, QHeaderView::ResizeToContents);
  ui->audio_file_browser->header()->setSectionResizeMode(1,
                                                         QHeaderView::Stretch);
  ui->audio_file_browser->header()->setSectionResizeMode(
      2, QHeaderView::ResizeToContents);

  // 文件管理器模型
  QFileSystemModel *file_model = new QFileSystemModel;
#ifndef _WIN32
  file_model->setRootPath(QDir::homePath());
#endif  //_WIN32
  ui->file_browser->setModel(file_model);

  // 跳转到当前运行目录
  ui->file_browser->setRootIndex(file_model->index(last_select_directory));

  // 过滤器(不显示.显示..)
  file_model->setFilter(QDir::AllEntries | QDir::NoDot);
  // 返回上一层lambda
  auto cduplambda = [this, file_model](const QModelIndex &index) {
    QString path = file_model->fileName(index);
    if (path == "..") {
      QDir dir(file_model->filePath(ui->file_browser->rootIndex()));
      dir.cdUp();
      ui->file_browser->setRootIndex(file_model->index(dir.absolutePath()));
    }
  };
  // 点击".." 的展开箭头返回上一级
  QObject::connect(ui->file_browser, &QTreeView::expanded, cduplambda);
  // 双击 ".." 返回上一层
  QObject::connect(ui->file_browser, &QTreeView::doubleClicked, cduplambda);

  // 设置不可编辑文件
  ui->file_browser->setEditTriggers(QAbstractItemView::NoEditTriggers);
  // 设置文件名栏初始宽度 (0表示文件名列)
  // 设置 300px 宽度
  ui->file_browser->setColumnWidth(0, 300);
  // 显示表头
  ui->file_browser->setHeaderHidden(false);

  // 注册音频数据类型
  qRegisterMetaType<std::shared_ptr<XSound>>("XSoundptr");

  // 初始化音频管理器
  audio_manager = XAudioManager::newmanager();

  // 每个输出设备添加一个标签页
  auto output_devices = audio_manager->get_outdevices();

  for (const auto &[sdl_id, device] : *output_devices) {
    if (sdl_id == -1) continue;
    auto controller = new DeviceManager(device);
    ui->device_tab_widget->addTab(controller,
                                  QString::fromStdString(device->device_name));
    devices.try_emplace(device, controller);
  }
}

MainWindow::~MainWindow() {
  delete ui;
  for (const auto &[audio, handle] : audio_list) {
    std::filesystem::path path(audio);
    audio_manager->unloadaudio(path.filename().string());
  }
}

void MainWindow::on_open_file_triggered([[maybe_unused]] bool checked) {
  auto options = QFileDialog::DontUseNativeDialog;
  auto fileNames = QFileDialog::getOpenFileNames(
      this, "选择文件", last_select_directory,
      "音频文件(*.mp3 *.wav *.ogg *.ncm *.flac *.wma *.aac *.mpc *.ape *.aiff)",
      nullptr, options);
  for (const auto &file : fileNames) {
    // 切换最后一次选择的目录
    QFileInfo file_info(file);
    if (file_info.isDir()) {
      last_select_directory = file_info.absoluteFilePath();
    } else if (file_info.isFile()) {
      QDir file_dir(file);
      file_dir.cdUp();
      last_select_directory = file_dir.dirName();
    }

    const auto filepath = file.toStdString();
    std::string name;
    // 使用XAudioLib加载音频
    auto handle = audio_manager->loadaudio(filepath, name);
    audio_list.try_emplace(name, handle);

    // 构建音频条目加入音频管理器
    QList<QStandardItem *> audio_row;
    auto handle_item = new QStandardItem(QString::number(handle));
    auto name_item = new QStandardItem(QString::fromStdString(name));
    auto &audio = (*audio_manager->get_audios())[handle];
    auto size = ((double)audio->get_pcm_data_size() * 4) / 1024.0 / 1024.0;
    auto size_item = new QStandardItem(QString::number(size, 'f', 2) + "MB");

    // 设置仅可选不可编辑
    handle_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    name_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    size_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    // 填充数据
    handle_item->setData(QVariant::fromValue(audio));
    name_item->setData(QVariant::fromValue(audio));
    size_item->setData(QVariant::fromValue(audio));

    audio_row.append(handle_item);
    audio_row.append(name_item);
    audio_row.append(size_item);

    static_cast<QStandardItemModel *>(ui->audio_file_browser->model())
        ->appendRow(audio_row);
  }
  // 设置文件管理器目录为最后一次打开的文件目录
}

// 双击音频项事件
void MainWindow::on_audio_file_browser_doubleClicked(const QModelIndex &index) {
  auto item = ((QStandardItemModel *)(ui->audio_file_browser->model()))
                  ->itemFromIndex(index);
  auto sound = item->data().value<std::shared_ptr<XSound>>();
  auto xdevices = audio_manager->get_outdevices();

  NewAudioOrbitDialog norbitdialog(xdevices);
  norbitdialog.setWindowTitle("添加音频轨道");

  norbitdialog.set_audio_name(QString::fromStdString(sound->name));
  norbitdialog.set_audio_duration(xutil::pcmpos2milliseconds(
      sound->pcm_data.size(), static_cast<int>(Config::samplerate), 2));
  norbitdialog.set_audio_memory(sound->pcm_data.size() * 4);

  if (norbitdialog.exec() == QDialog::Accepted) {
    auto device = norbitdialog.choosed_device();
    // 确认
    auto it = devices.find(device);
    if (it == devices.end()) {
      qDebug() << "设备有误";
      return;
    }
    auto volume = norbitdialog.get_volume_setting();
    auto speed = norbitdialog.get_speed_setting();
    auto autoplay = norbitdialog.isautoplay();
    auto isloop = norbitdialog.isloop();
    // 获取设备管理器
    auto device_controller = it->second;
    // 构造音频轨道
    auto orbit = std::make_shared<XAudioOrbit>(sound);
    orbit->volume = (float)volume / 100.0f;
    orbit->speed = (float)speed / 100.0f;
    orbit->paused = !autoplay;
    orbit->loop = isloop;
    device_controller->add_audio_orbit(orbit);
  }
}
