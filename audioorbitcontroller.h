#ifndef AUDIOORBITCONTROLLER_H
#define AUDIOORBITCONTROLLER_H

#include <QWidget>
#include <memory>

class AudioManager;
class XAudioOrbit;

namespace Ui {
class AudioOrbitController;
}

class AudioOrbitController : public QWidget {
  Q_OBJECT

 public:
  explicit AudioOrbitController(std::shared_ptr<XAudioOrbit> &xaudioorbit,
                                QWidget *parent = nullptr);
  ~AudioOrbitController() override;

 private:
  Ui::AudioOrbitController *ui;
  // 此控制器控制的音频轨道
  std::shared_ptr<XAudioOrbit> xaudio_orbit;
  friend AudioManager;
};

#endif  // AUDIOORBITCONTROLLER_H
