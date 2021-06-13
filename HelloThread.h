#ifndef HELLOTHREAD_H
#define HELLOTHREAD_H
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>
#include <QCoreApplication>
#include "mainwindow.h"

// hellothread/hellothread.h
class HelloThread : public QThread
{
    Q_OBJECT
public:
    MainWindow* w;
    void _wait(float sec)
    {
        QElapsedTimer timer;
        timer.start();

        while(! timer.hasExpired(sec*1000))
            QCoreApplication::processEvents();
    }
private:

    void run() {
        while (true) {
            _wait(0.016);
            w->sceneQWidget->fElapsedTime++;
            w->sceneQWidget->_repaint();
            //qDebug() << "hello from worker thread " << thread()->currentThreadId();
        }
        //timer->start(1000);
    }
};

#endif // HELLOTHREAD_H
