#include "mainwindow.h"

#include <QFileDialog>
#include <filesystem>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  audio_manager = XAudioManager::newmanager();
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
  }
}
