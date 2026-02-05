#include "MainWindow.h"
#include "model/LogEntry.h"

#include <QApplication>

/*
 * 程序入口
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<LogEntry>("LogEntry");
    qRegisterMetaType<QList<LogEntry>>("QList<LogEntry>");

    MainWindow w;
    w.show();
    return a.exec();
}
