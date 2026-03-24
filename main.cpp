#include "MainWindow.h"
#include "LogEntry.h"

#include <QApplication>
#include <QThread>
#include <QDebug>

/*
 * 程序入口
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "主线程ID:" << QThread::currentThreadId();

    qRegisterMetaType<LogEntry>("LogEntry");
    qRegisterMetaType<QList<LogEntry>>("QList<LogEntry>");

    MainWindow w;
    w.show();
    return a.exec();
}
