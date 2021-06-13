#include "mainwindow.h"
#include "HelloThread.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    HelloThread thread;
    thread.w = &w;
    thread.start();
    qDebug() << "hello from GUI thread " << a.thread()->currentThreadId();

    a.exec();

    thread.wait();  // do not exit before the thread is completed!

    return 0;
}
