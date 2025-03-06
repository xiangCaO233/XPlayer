#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QWidget>
#include <memory>
#include <unordered_map>

#include "audioorbitcontroller.h"

class XOutputDevice;

namespace Ui {
class AudioManager;
}

class AudioManager : public QWidget {
  Q_OBJECT

 public:
  explicit AudioManager(std::shared_ptr<XOutputDevice> device,
                        QWidget *parent = nullptr);
  ~AudioManager() override;

 private:
  Ui::AudioManager *ui;
  // 此设备的音频管理器所保存的所有音频轨道
  std::unordered_map<std::shared_ptr<XAudioOrbit>, AudioOrbitController *>
      audio_orbits;
  // 此设备的音频管理器所对应的输出设备
  std::shared_ptr<XOutputDevice> outdevice;
  // 添加音频轨道
  void add_audio_orbit(std::shared_ptr<XAudioOrbit> &audioorbit);
  // 移除音频轨道
  void remove_audio_orbit(std::shared_ptr<XAudioOrbit> &audioorbit);
};

#endif  // AUDIOMANAGER_H
