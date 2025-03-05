#include <QApplication>
#ifdef __APPLE__
#include <QDir>
#endif  //__APPLE__

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
#ifdef __APPLE__
  // 获取 .app 文件路径 (applicationFilePath() ->
  // /xxx/xxx/myapp.app/Contents/MacOS/myapp)
  QString appPath = QCoreApplication::applicationFilePath();

  // 获取 .app 文件所处目录 (上层目录)
  // MacOS目录
  QDir appDir = QFileInfo(appPath).absoluteDir();
  // 返回到 myapp.app 的目录
  appDir.cdUp();
  appDir.cdUp();
  appDir.cdUp();

  // 设置工作目录
  QDir::setCurrent(appDir.absolutePath());
#endif  //__APPLE__
  MainWindow w;
  w.show();
  return QApplication::exec();
}
