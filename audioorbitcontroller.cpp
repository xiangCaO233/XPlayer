#include "audioorbitcontroller.h"

#include "include/AudioManager.h"
#include "ui_audioorbitcontroller.h"

AudioOrbitController::AudioOrbitController(
    std::shared_ptr<XAudioOrbit> &xaudioorbit, QWidget *parent)
    : QWidget(parent),
      xaudio_orbit(xaudioorbit),
      ui(new Ui::AudioOrbitController) {
  ui->setupUi(this);

  // 初始化轨道控制器标题
  ui->audio_title->setText(QString::fromStdString(xaudio_orbit->sound->name));
}

AudioOrbitController::~AudioOrbitController() { delete ui; }
