#include "audiomanager.h"

#include "audioorbitcontroller.h"
#include "ui_audiomanager.h"

AudioManager::AudioManager(std::shared_ptr<XOutputDevice> device,
                           QWidget *parent)
    : QWidget(parent), outdevice(device), ui(new Ui::AudioManager) {
  ui->setupUi(this);
  auto layout = new QVBoxLayout;
  layout->setSpacing(2);
  layout->setContentsMargins(0, 0, 0, 0);
  ui->audio_orbit_container->setLayout(layout);
}

AudioManager::~AudioManager() { delete ui; }

// 添加音频轨道
void AudioManager::add_audio_orbit(std::shared_ptr<XAudioOrbit> &audioorbit) {
  // 构造音频轨道控制器
  auto audio_orbit_controller = new AudioOrbitController(audioorbit);
  // 将控制器加入设备轨道管理器布局
  ui->audio_orbit_container->layout()->addWidget(audio_orbit_controller);
  // 添加索引
  audio_orbits.try_emplace(audioorbit, audio_orbit_controller);
}
// 移除音频轨道
void AudioManager::remove_audio_orbit(
    std::shared_ptr<XAudioOrbit> &audioorbit) {
  auto it = audio_orbits.find(audioorbit);
  if (it == audio_orbits.end()) {
    return;
  }
  it->second->destroy();
  audio_orbits.erase(it);
}
