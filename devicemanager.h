#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <qlayoutitem.h>

#include <QWidget>
#include <memory>
#include <unordered_map>

#include "audioorbitcontroller.h"

class XOutputDevice;

namespace Ui {
class DeviceManager;
}

class DeviceManager : public QWidget {
  Q_OBJECT

 public:
  explicit DeviceManager(std::shared_ptr<XOutputDevice> dev,
                         QWidget *parent = nullptr);
  ~DeviceManager() override;
  // 添加音频轨道
  void add_audio_orbit(std::shared_ptr<XAudioOrbit> &audioorbit);
  // 移除音频轨道
  void remove_audio_orbit(std::shared_ptr<XAudioOrbit> &audioorbit);

 private:
  Ui::DeviceManager *ui;
  QSpacerItem *spacer;
  std::shared_ptr<XAudioManager> manager;
  // 此设备的音频管理器所保存的所有音频轨道
  std::unordered_map<std::shared_ptr<XAudioOrbit>, AudioOrbitController *>
      audio_orbits;
  // 此设备的音频管理器所对应的输出设备
  std::shared_ptr<XOutputDevice> device;
};

#endif  // DEVICEMANAGER_H
