#include "audioorbitcontroller.h"

#include <qnamespace.h>
#include <qsize.h>

#include <memory>

#include "devicemanager.h"
#include "ui_audioorbitcontroller.h"
#include "util.h"

class TimeCallback : public PlayposCallBack {
 public:
  // 控制器指针
  AudioOrbitController *audio_orbit_controller;

  TimeCallback(AudioOrbitController *controller) : PlayposCallBack() {
    // 构造传递控制器指针
    audio_orbit_controller = controller;
  }

  virtual ~TimeCallback() override = default;

  // 实现回调函数
  virtual void playpos_call(double playpos) override {
    // 回调中设置当前播放时间

    auto time_milliseconds = xutil::pcmpos2milliseconds(
        (size_t)playpos, static_cast<int>(Config::samplerate),
        static_cast<int>(Config::channel));

    audio_orbit_controller->update_current_time_label(
        (double)time_milliseconds);
    // qDebug() << "控制器发送播放时间更新信号参数:" << time_milliseconds;
    // qDebug() << "playpos:" << audio_orbit_controller->xaudio_orbit->playpos;
    emit audio_orbit_controller->time_update_signal((double)time_milliseconds);
  }
};

AudioOrbitController::AudioOrbitController(
    std::shared_ptr<XAudioOrbit> &xaudioorbit, DeviceManager *devicemanager,
    QWidget *parent)
    : QWidget(parent),
      xaudio_orbit(xaudioorbit),
      ui(new Ui::AudioOrbitController) {
  ui->setupUi(this);
  // 设置进度条绑定的音频轨道
  ui->play_progress->setup_orbit(xaudioorbit);
  ui->current_volume_label->setText(QString::number(xaudioorbit->volume * 100) +
                                    "%");
  // 更新图标
  ui->seek_back->setIcon(qutil::colorSvgIcon(":/svg/svgs/seekbackward.svg",
                                             Qt::white, QSize(24, 24)));
  ui->seek_forward->setIcon(qutil::colorSvgIcon(":/svg/svgs/seekforward.svg",
                                                Qt::white, QSize(24, 24)));
  update_volume_button_icon((int)(xaudioorbit->volume * 100));
  ui->audio_info_button->setIcon(
      qutil::colorSvgIcon(":/svg/svgs/settings.svg", Qt::white, QSize(24, 24)));
  // 更新暂停按钮图标 
  ui->pauseorresume->setIcon(qutil::colorSvgIcon(
      xaudioorbit->paused ? ":/svg/svgs/resume.svg" : ":/svg/svgs/pause.svg",
      Qt::white, QSize(24, 24)));
  // 连接进度条改变信号
  QObject::connect(ui->play_progress,
                   &ClickableProgressBar::progressbar_set_signal, this,
                   &AudioOrbitController::on_progress_changed);
  // 连接控制器播放位置改变信号
  QObject::connect(this, &AudioOrbitController::time_update_signal,
                   ui->play_progress,
                   &ClickableProgressBar::on_playpos_changed);
  // 连接暂停按钮点击信号
  QObject::connect(this, &AudioOrbitController::pause_button_clicked_signal,
                   devicemanager, &DeviceManager::on_orbit_pausebutton_clicked);

  // 初始化轨道控制器标题
  ui->audio_title->setText(QString::fromStdString(xaudio_orbit->sound->name));

  // 初始化总播放时间
  total_time_milliseconds = xutil::pcmpos2milliseconds(
      xaudioorbit->sound->pcm_data.size(), static_cast<int>(Config::samplerate),
      static_cast<int>(Config::channel));
  QString formated_text;
  format_time(total_time_milliseconds, formated_text);
  ui->time_total->setText(formated_text);

  // 添加播放时间回调
  std::shared_ptr<PlayposCallBack> callback =
      std::make_shared<TimeCallback>(this);
  xaudioorbit->add_playpos_callback(callback);
}

AudioOrbitController::~AudioOrbitController() { delete ui; }
// 格式化时间
void AudioOrbitController::format_time(size_t time_milliseconds,
                                       QString &text) {
  // 格式化时间
  auto timeminutes = time_milliseconds / 1000 / 60;
  auto timeseconds = time_milliseconds / 1000;
  auto timemilliseconds = time_milliseconds % 1000;
  text = QString("%1").arg(timeminutes, 2, 10, QChar('0')) + ":" +
         QString("%1").arg(timeseconds % 60, 2, 10, QChar('0')) + "." +
         QString("%1").arg(timemilliseconds, 3, 10, QChar('0'));
}

// 更新当前位置标签
void AudioOrbitController::update_current_time_label(double time) {
  QString formated_text;
  format_time((size_t)time, formated_text);
  ui->time_current->setText(formated_text);
}
// 进度条改变槽
void AudioOrbitController::on_progress_changed(double time) {
  // qDebug() << "控制器收到进度条改变信号参数" << time;
  // 更新属性
  xaudio_orbit->playpos = (double)xutil::milliseconds2pcmpos(
      (size_t)time, static_cast<int>(Config::samplerate),
      static_cast<int>(Config::channel));
  update_current_time_label(time);
}
// 暂停/播放按键事件
void AudioOrbitController::on_pauseorresume_clicked() {
  // 切换暂停状态
  xaudio_orbit->paused = !xaudio_orbit->paused;
  ui->pauseorresume->setIcon(qutil::colorSvgIcon(
      xaudio_orbit->paused ? ":/svg/svgs/resume.svg" : ":/svg/svgs/pause.svg",
      Qt::white, ui->pauseorresume->size()));
  emit pause_button_clicked_signal();
}

// 快退按键事件
void AudioOrbitController::on_seek_back_clicked() {
  // 获取当前时间并减少5000ms
  auto time_milliseconds =
      xutil::pcmpos2milliseconds((size_t)xaudio_orbit->playpos,
                                 static_cast<int>(Config::samplerate),
                                 static_cast<int>(Config::channel)) -
      5000;
  // 非法检查
  if (time_milliseconds < 0) {
    time_milliseconds = 0;
  }
  // 更新属性
  xaudio_orbit->playpos = (double)xutil::milliseconds2pcmpos(
      (size_t)time_milliseconds, static_cast<int>(Config::samplerate),
      static_cast<int>(Config::channel));
  // 更新label并发送信号
  update_current_time_label((double)time_milliseconds);
  emit time_update_signal((double)time_milliseconds);
}

// 快进按键事件
void AudioOrbitController::on_seek_forward_clicked() {
  // 获取当前时间并增加5000ms
  auto time_milliseconds =
      xutil::pcmpos2milliseconds((size_t)xaudio_orbit->playpos,
                                 static_cast<int>(Config::samplerate),
                                 static_cast<int>(Config::channel)) +
      5000;
  // 非法检查
  if (time_milliseconds > total_time_milliseconds) {
    time_milliseconds = total_time_milliseconds - 100;
  }
  // 更新属性
  xaudio_orbit->playpos = (double)xutil::milliseconds2pcmpos(
      (size_t)time_milliseconds, static_cast<int>(Config::samplerate),
      static_cast<int>(Config::channel));
  // 更新label并发送信号
  update_current_time_label((double)time_milliseconds);
  emit time_update_signal((double)time_milliseconds);
}

// 静音按键事件
void AudioOrbitController::on_mute_button_clicked() {}

// 音量条值变化事件
void AudioOrbitController::on_volume_slider_valueChanged(int value) {
  auto volume = (double)value / 100.0;
  ui->current_volume_label->setText(QString::number(value) + "%");
  xaudio_orbit->volume = ((float)volume);
  update_volume_button_icon(value);
}

// 音频轨道设置按键事件
void AudioOrbitController::on_audio_info_button_clicked() {
  auto v = (float)ui->volume_slider->value();
  // 切换静音
  if (v != 0) {
    last_volume = v / 100.0f;
    ui->current_volume_label->setText(QString::number(0) + "%");
    xaudio_orbit->volume = 0.0f;
    update_volume_button_icon(0);
  } else {
    ui->current_volume_label->setText(QString::number(last_volume) + "%");
    xaudio_orbit->volume = last_volume;
    update_volume_button_icon((int)(last_volume * 100.0f));
  }
}

// 更新音量按钮图标
void AudioOrbitController::update_volume_button_icon(int value) {
  if (value >= 67) {
    ui->mute_button->setIcon(qutil::colorSvgIcon(":/svg/svgs/volumehigh.svg",
                                                 Qt::white, QSize(24, 24)));
  } else if (value >= 33) {
    ui->mute_button->setIcon(qutil::colorSvgIcon(":/svg/svgs/volumemiddle.svg",
                                                 Qt::white, QSize(24, 24)));
  } else if (value > 0) {
    ui->mute_button->setIcon(qutil::colorSvgIcon(":/svg/svgs/volumelow.svg",
                                                 Qt::white, QSize(24, 24)));
  } else {
    ui->mute_button->setIcon(qutil::colorSvgIcon(":/svg/svgs/volumemute.svg",
                                                 Qt::white, QSize(24, 24)));
  }
}
