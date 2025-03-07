#ifndef QUTIL_H
#define QUTIL_H

#include <QIcon>
#include <QPainter>
#include <QSvgRenderer>

namespace qutil {
// 渲染图标
inline QIcon colorSvgIcon(const QString &svgPath, const QColor &color,
                          const QSize &size) {
  QSvgRenderer renderer(svgPath);
  // 设置图标大小
  QPixmap pixmap(size);
  pixmap.fill(Qt::transparent);

  QPainter painter(&pixmap);
  renderer.render(&painter);

  // 设置颜色遮罩
  painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  painter.fillRect(pixmap.rect(), color);
  painter.end();

  return QIcon(pixmap);
}
};  // namespace qutil

#endif  // QUTIL_H
