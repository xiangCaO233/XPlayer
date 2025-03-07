#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <unordered_map>

class XAudioManager;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

private slots:
  // 打开文件事件
  void on_open_file_triggered(bool checked);

  void on_audio_file_browser_doubleClicked(const QModelIndex &index);

private:
  Ui::MainWindow* ui;
  QString last_select_directory;
  // 音频管理器
  std::shared_ptr<XAudioManager> audio_manager;
  // 音频名称列表
  std::unordered_map<std::string, int> audio_list;
};
#endif  // MAINWINDOW_H
