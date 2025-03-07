#ifndef AUDIOORBITCONTROLLER_H
#define AUDIOORBITCONTROLLER_H

#include <QWidget>
#include <memory>

#include "include/AudioManager.h"

class DeviceManager;
class XAudioOrbit;

class TimeCallback;
class LabelWithSignal;

namespace Ui {
class AudioOrbitController;
}

class AudioOrbitController : public QWidget {
  Q_OBJECT

 public:
  explicit AudioOrbitController(std::shared_ptr<XAudioOrbit> &xaudioorbit,
                                QWidget *parent = nullptr);
  ~AudioOrbitController() override;

 private slots:
  void on_pauseorresume_clicked();

  void on_seek_back_clicked();

  void on_seek_forward_clicked();

  void on_mute_button_clicked();

  void on_volume_slider_valueChanged(int value);

  void on_audio_info_button_clicked();

 private:
  Ui::AudioOrbitController *ui;
  // 此控制器控制的音频轨道
  std::shared_ptr<XAudioOrbit> xaudio_orbit;
  // 格式化时间
  void format_time(size_t time, QString &text);
  friend DeviceManager;
  friend TimeCallback;
};

#endif  // AUDIOORBITCONTROLLER_H
