#include <QApplication>
#include <QDir>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QString appPath = QCoreApplication::applicationFilePath();

  // 获取 .app 文件所处目录 (上层目录)
  // MacOS目录
  QDir appDir = QFileInfo(appPath).absoluteDir();
#ifdef __unix
  while (!appDir.absolutePath().endsWith("/build")) {
    appDir.cdUp();
  }
#endif  //__unix
  // 设置工作目录
  QDir::setCurrent(appDir.absolutePath());

  MainWindow w;
  w.show();
  return QApplication::exec();
}
