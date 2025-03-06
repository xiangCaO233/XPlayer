#ifndef AUDIOORBITCONTROLLER_H
#define AUDIOORBITCONTROLLER_H

#include <QWidget>

namespace Ui {
class AudioOrbitController;
}

class AudioOrbitController : public QWidget {
  Q_OBJECT

 public:
  explicit AudioOrbitController(QWidget *parent = nullptr);
  ~AudioOrbitController();

 private:
  Ui::AudioOrbitController *ui;
};

#endif  // AUDIOORBITCONTROLLER_H
