#include <QApplication>
#include <QDir>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QString appPath = QCoreApplication::applicationFilePath();
  QDir appDir = QFileInfo(appPath).absoluteDir();

#ifdef _WIN32
#else
  while (!appDir.absolutePath().endsWith("/build")) {
    appDir.cdUp();
  }
#endif
  // 设置工作目录
  QDir::setCurrent(appDir.absolutePath());

  MainWindow w;
  w.show();
  return QApplication::exec();
}
