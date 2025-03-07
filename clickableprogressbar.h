#ifndef CLICKABLE_PROGRESSBAR_H
#define CLICKABLE_PROGRESSBAR_H

#include <qlabel.h>
#include <qprogressbar.h>
#include <qtmetamacros.h>

#include <QMouseEvent>
#include <cstddef>
#include <memory>

class XAudioOrbit;

class ClickableProgressBar : public QProgressBar {
  Q_OBJECT
 public:
  QLabel *floating_label;
  std::shared_ptr<XAudioOrbit> audio_orbit;
  double ratio;
  size_t total_timemilliseconds;
  bool pressed{false};

  ClickableProgressBar(QWidget *parent = nullptr);
  ~ClickableProgressBar() override;
  // 设置轨道信息
  void setup_orbit(std::shared_ptr<XAudioOrbit> orbit);
 public slots:
  // 播放位置改变槽
  void on_playpos_changed(double time);

 signals:
  void progressbar_set_signal(double timemilliseconds);

 protected:
  // 发出信号
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
};

#endif  // CLICKABLE_PROGRESSBAR_H
