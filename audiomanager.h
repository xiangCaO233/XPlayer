#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QWidget>
#include <memory>

class XOutputDevice;

namespace Ui {
class AudioManager;
}

class AudioManager : public QWidget {
  Q_OBJECT

 public:
  explicit AudioManager(std::shared_ptr<XOutputDevice> device,
                        QWidget *parent = nullptr);
  ~AudioManager();

 private:
  Ui::AudioManager *ui;
  std::shared_ptr<XOutputDevice> outdevice;
};

#endif  // AUDIOMANAGER_H
