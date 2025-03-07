#ifndef AUDIOORBITCONTROLLER_H
#define AUDIOORBITCONTROLLER_H

#include <qtmetamacros.h>

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
                                DeviceManager *devicemanager,
                                QWidget *parent = nullptr);
  ~AudioOrbitController() override;
  // 格式化时间
  static void format_time(size_t time, QString &text);
 signals:
  void time_update_signal(double time);
  void pause_button_clicked_signal();

 private slots:
  // 进度条改变槽
  void on_progress_changed(double time);

  void on_pauseorresume_clicked();

  void on_seek_back_clicked();

  void on_seek_forward_clicked();

  void on_mute_button_clicked();

  void on_volume_slider_valueChanged(int value);

  void on_audio_info_button_clicked();

 private:
  Ui::AudioOrbitController *ui;
  // 总时间
  size_t total_time_milliseconds;
  // 此控制器控制的音频轨道
  std::shared_ptr<XAudioOrbit> xaudio_orbit;
  // 更新当前位置标签
  void update_current_time_label(double time);

  friend DeviceManager;
  friend TimeCallback;
};

#endif  // AUDIOORBITCONTROLLER_H
