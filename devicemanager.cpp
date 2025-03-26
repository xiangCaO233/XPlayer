#include "devicemanager.h"

#include <qglobal.h>

#include "audioorbitcontroller.h"
#include "ui_devicemanager.h"
#include "util.h"

DeviceManager::DeviceManager(std::shared_ptr<XOutputDevice> dev,
                             QWidget *parent)
    : QWidget(parent), device(dev), ui(new Ui::DeviceManager) {
  ui->setupUi(this);
  // 更新按钮图标
  ui->mute_button->setIcon(qutil::colorSvgIcon(":/svg/svgs/volumehigh.svg",
                                               Qt::white, QSize(24, 24)));
  ui->resetspeed_button->setIcon(
      qutil::colorSvgIcon(":/svg/svgs/speed.svg", Qt::white, QSize(24, 24)));

  // 应用布局
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

  // 更新标签
  ui->current_volume_label->setText(
      QString::number(device->player->global_volume * 100) + "%");
  ui->volume_slider->setValue(int(device->player->global_volume * 100));
  ui->speed_slider->setValue(int(device->player->global_speed * 100));
  ui->current_speed_label->setText(
      QString::number(device->player->global_speed * 100) + "%");
}

DeviceManager::~DeviceManager() { delete ui; }

// 轨道暂停按钮点击信号槽
void DeviceManager::on_pauseorresume_clicked() {
  // 检查目前是否需要播放
  bool shouldplay{false};
  for (const auto &orbit : audio_orbits) {
    if (!orbit.first->paused) {
      shouldplay = true;
      break;
    }
  }
  if (shouldplay && device->player->paused) {
    // 需要播放且播放器暂停中/恢复播放器
    device->player->resume();
  }
}
// 添加音频轨道
void DeviceManager::add_audio_orbit(std::shared_ptr<XAudioOrbit> &audioorbit) {
  // 构造音频轨道控制器
  auto audio_orbit_controller = new AudioOrbitController(audioorbit, this);

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

// 播放速度slider值变化事件
void DeviceManager::on_speed_slider_valueChanged(int value) {
  if (value >= 10 && value <= 500) {
    auto ratio = (double)value / 100.0;
    ui->current_speed_label->setText(QString::number(ratio, 'f', 2) + "x");
  }
}

// 播放速度slider释放事件
void DeviceManager::on_speed_slider_sliderReleased() {
  auto value = ui->speed_slider->value();
  if (value >= 25 && value <= 400) {
    auto ratio = (double)value / 100.0;
    ui->current_speed_label->setText(QString::number(ratio, 'f', 2) + "x");
    device->player->ratio((float)ratio);
  }
}

// 重置播放速度事件
void DeviceManager::on_resetspeed_button_clicked() {
  auto ratio = 1.0;
  ui->current_speed_label->setText(QString::number(ratio, 'f', 2) + "x");
  device->player->ratio((float)ratio);
}

// 音量条值变化事件
void DeviceManager::on_volume_slider_valueChanged(int value) {
  auto volume = (double)value / 100.0;
  ui->current_volume_label->setText(QString::number(value) + "%");
  device->player->set_player_volume((float)volume);
  update_volume_button_icon(value);
}

// 静音按钮事件
void DeviceManager::on_mute_button_clicked() {
  auto v = (float)ui->volume_slider->value();
  // 切换静音
  if (v != 0) {
    last_volume = v / 100.0f;
    ui->current_volume_label->setText(QString::number(0) + "%");
    device->player->set_player_volume(0.0f);
    ui->volume_slider->setValue(0);
    update_volume_button_icon(0);
  } else {
    ui->current_volume_label->setText(QString::number(last_volume) + "%");
    device->player->set_player_volume(last_volume);
    ui->volume_slider->setValue((int)(last_volume * 100.0f));
    update_volume_button_icon((int)(last_volume * 100.0f));
  }
}

// 更新音量按钮图标
void DeviceManager::update_volume_button_icon(int value) {
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
