#include "clickableprogressbar.h"

#include "audioorbitcontroller.h"
#include "include/AudioManager.h"

ClickableProgressBar::ClickableProgressBar(QWidget *parent)
    : QProgressBar(parent) {
  setMouseTracking(true);
  floating_label = new QLabel(parent->parentWidget());
  floating_label->setStyleSheet(
      "background-color: black; border: 2px solid white; border-radius: "
      "6px;");
  floating_label->setText("");
  floating_label->hide();
}

ClickableProgressBar::~ClickableProgressBar() = default;

// 播放位置改变槽
void ClickableProgressBar::on_playpos_changed(double time) {
  // qDebug() << "进度条收到播放位置更新信号参数:" << time;
  setValue((int)(time / (double)total_timemilliseconds * 1000));
  repaint();
}

// 设置轨道信息
void ClickableProgressBar::setup_orbit(std::shared_ptr<XAudioOrbit> orbit) {
  audio_orbit = orbit;
  total_timemilliseconds = xutil::plannerpcmpos2milliseconds(
      audio_orbit->sound->pcm[0].size(), static_cast<int>(Config::samplerate));
}

void ClickableProgressBar::mousePressEvent(QMouseEvent *event) {
  // qDebug() << "鼠标按下,发送设置进度条信号";
  // qDebug() << "设置进度条信号参数:" << ratio *
  // (double)total_timemilliseconds;
  pressed = true;

  if (ratio <= 0) {
    setValue(0);
    emit progressbar_set_signal(0);
  } else if (ratio >= 1) {
    setValue(1000);
    emit progressbar_set_signal((double)total_timemilliseconds);
  } else {
    setValue((int)(ratio * 1000));
    emit progressbar_set_signal(ratio * (double)total_timemilliseconds);
  }
  repaint();
}
void ClickableProgressBar::mouseReleaseEvent(QMouseEvent *event) {
  pressed = false;
}
void ClickableProgressBar::enterEvent(QEnterEvent *event) {
  floating_label->show();
}
void ClickableProgressBar::leaveEvent(QEvent *event) { floating_label->hide(); }
void ClickableProgressBar::mouseMoveEvent(QMouseEvent *event) {
  auto pos = event->pos() + QPoint(0, -floating_label->height() - 4);

  // 计算比例
  ratio = (double)event->pos().x() / (double)(this->size().width());

  // 计算悬浮位置事件戳
  auto time = ratio * (double)total_timemilliseconds;

  // 格式化时间
  QString text;
  AudioOrbitController::format_time(time, text);
  floating_label->setText(text);

  // 移动到2x相对位置上方
  floating_label->move(pos + this->pos() + parentWidget()->pos());

  // 正在拖动
  if (pressed) {
    if (ratio <= 0) {
      setValue(0);
      emit progressbar_set_signal(0);
    } else if (ratio >= 1) {
      setValue(1000);
      emit progressbar_set_signal(1000);
    } else {
      setValue((int)(ratio * 1000));
      emit progressbar_set_signal(time);
    }
    repaint();
  }
}
