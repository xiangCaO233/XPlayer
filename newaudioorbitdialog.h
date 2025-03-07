#ifndef NEWAUDIOORBITDIALOG_H
#define NEWAUDIOORBITDIALOG_H

#include <QDialog>
#include <memory>
#include <utility>
class XOutputDevice;

namespace Ui {
class NewAudioOrbitDialog;
}

class NewAudioOrbitDialog : public QDialog {
  Q_OBJECT

 public:
  explicit NewAudioOrbitDialog(
      std::unordered_map<int, std::shared_ptr<XOutputDevice>> *devs,
      QWidget *parent = nullptr);
  ~NewAudioOrbitDialog() override;

  // 设置音频名
  void set_audio_name(const QString &name);
  // 设置音频时长title(ms)
  void set_audio_duration(size_t milliseconds);
  // 设置音频内存(b)
  void set_audio_memory(size_t bytes);

  // 获取设定
  int get_volume_setting() const;
  int get_speed_setting() const;
  bool isautoplay() const;
  bool isloop() const;
  std::shared_ptr<XOutputDevice> choosed_device() const;

 private:
  std::unordered_map<int, std::shared_ptr<XOutputDevice>> *devices;
  Ui::NewAudioOrbitDialog *ui;
};

#endif  // NEWAUDIOORBITDIALOG_H
