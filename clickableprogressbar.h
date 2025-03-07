#ifndef CLICKABLE_PROGRESSBAR_H
#define CLICKABLE_PROGRESSBAR_H

#include <qprogressbar.h>
class ClickableProgressBar : public QProgressBar {
 public:
  explicit ClickableProgressBar(QWidget *parent = nullptr)
      : QProgressBar(parent){};
  ~ClickableProgressBar() override = default;
};

#endif  // CLICKABLE_PROGRESSBAR_H
