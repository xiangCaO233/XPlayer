#include "mainwindow.h"

#include <qglobal.h>
#include <qlist.h>
#include <qstandarditemmodel.h>
#include <qstringliteral.h>
#include <qwidget.h>

#include <QFileDialog>
#include <QStandardItemModel>
#include <filesystem>

#include "./ui_mainwindow.h"
#include "include/AudioManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  auto model = new QStandardItemModel(ui->audio_file_browser);
  model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("句柄")
                                                 << QStringLiteral("文件"));
  ui->audio_file_browser->setModel(model);
  ui->audio_file_browser->setColumnWidth(0,
                                         ui->audio_file_browser->width() * 0.4);
  ui->audio_file_browser->setColumnWidth(1,
                                         ui->audio_file_browser->width() * 0.6);

  // 初始化音频管理器
  audio_manager = XAudioManager::newmanager();

  // 每个输出设备添加一个标签页
  auto output_devices = audio_manager->get_outdevices();

  for (const auto &[sdl_id, device] : *output_devices) {
    if (sdl_id == -1) continue;
    auto temp = new QWidget;
    ui->device_tab_widget->addTab(temp,
                                  QString::fromStdString(device->device_name));
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
      this, "选择文件", "/",
      "音频文件(*.mp3 *.wav *.ogg *.ncm *.flac *.wma *.aac *.mpc *.ape *.aiff)",
      nullptr, options);
  for (const auto &file : fileNames) {
    const auto filepath = file.toStdString();
    std::string name;
    auto handle = audio_manager->loadaudio(filepath, name);
    audio_list.try_emplace(name, handle);

    QList<QStandardItem *> audio_row;
    auto handle_item = new QStandardItem(QString::number(handle));
    auto name_item = new QStandardItem(QString::fromStdString(name));
    // 仅可选不可编辑
    handle_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    name_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    audio_row.append(handle_item);
    audio_row.append(name_item);
    static_cast<QStandardItemModel *>(ui->audio_file_browser->model())
        ->appendRow(audio_row);
  }
}
