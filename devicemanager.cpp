#include "devicemanager.h"

#include "audioorbitcontroller.h"
#include "ui_devicemanager.h"

DeviceManager::DeviceManager(std::shared_ptr<XOutputDevice> dev,
                             QWidget *parent)
    : QWidget(parent), device(dev), ui(new Ui::DeviceManager) {
  ui->setupUi(this);
  auto layout = new QVBoxLayout;
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  // 纵向弹簧
  spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  layout->addItem(spacer);
  ui->audio_orbit_container->setLayout(layout);
  device->creat_player();
  device->player->start();
  device->player->pause();
}

DeviceManager::~DeviceManager() { delete ui; }

// 添加音频轨道
void DeviceManager::add_audio_orbit(std::shared_ptr<XAudioOrbit> &audioorbit) {
  // 构造音频轨道控制器
  auto audio_orbit_controller = new AudioOrbitController(audioorbit);

  // 将控制器加入设备轨道管理器布局(确保弹簧在最下面)
  ui->audio_orbit_container->layout()->removeItem(spacer);
  ui->audio_orbit_container->layout()->addWidget(audio_orbit_controller);
  ui->audio_orbit_container->layout()->addItem(spacer);

  // 添加音轨到混音器
  device->player->mixer->add_orbit(audioorbit);

  // 添加索引
  audio_orbits.try_emplace(audioorbit, audio_orbit_controller);
}
// 移除音频轨道
void DeviceManager::remove_audio_orbit(
    std::shared_ptr<XAudioOrbit> &audioorbit) {
  auto it = audio_orbits.find(audioorbit);
  if (it == audio_orbits.end()) {
    qDebug() << "轨道错误";
    return;
  }

  // 移除和销毁组件
  ui->audio_orbit_container->layout()->removeWidget(it->second);
  it->second->destroy();

  // 移除索引
  audio_orbits.erase(it);
}
