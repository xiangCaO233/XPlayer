#include "audioorbitcontroller.h"

#include <memory>

#include "ui_audioorbitcontroller.h"

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
    QString formated_text;
    audio_orbit_controller->format_time((size_t)playpos, formated_text);
    audio_orbit_controller->ui->time_current->setText(formated_text);
  }
};

AudioOrbitController::AudioOrbitController(
    std::shared_ptr<XAudioOrbit> &xaudioorbit, QWidget *parent)
    : QWidget(parent),
      xaudio_orbit(xaudioorbit),
      ui(new Ui::AudioOrbitController) {
  ui->setupUi(this);

  // 初始化轨道控制器标题
  ui->audio_title->setText(QString::fromStdString(xaudio_orbit->sound->name));

  // 初始化总播放时间
  auto time_milliseconds = xutil::pcmpos2milliseconds(
      xaudioorbit->sound->pcm_data.size(), static_cast<int>(Config::samplerate),
      static_cast<int>(Config::channel));
  QString formated_text;
  format_time(time_milliseconds, formated_text);
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
         QString("%1").arg(timeseconds, 2, 10, QChar('0')) + "." +
         QString("%1").arg(timemilliseconds, 3, 10, QChar('0'));
}
