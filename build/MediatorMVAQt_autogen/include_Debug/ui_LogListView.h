/********************************************************************************
** Form generated from reading UI file 'LogListView.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGLISTVIEW_H
#define UI_LOGLISTVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LogListView
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tableViewLogs;

    void setupUi(QWidget *LogListView)
    {
        if (LogListView->objectName().isEmpty())
            LogListView->setObjectName(QString::fromUtf8("LogListView"));
        LogListView->resize(640, 320);
        verticalLayout = new QVBoxLayout(LogListView);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableViewLogs = new QTableView(LogListView);
        tableViewLogs->setObjectName(QString::fromUtf8("tableViewLogs"));

        verticalLayout->addWidget(tableViewLogs);


        retranslateUi(LogListView);

        QMetaObject::connectSlotsByName(LogListView);
    } // setupUi

    void retranslateUi(QWidget *LogListView)
    {
        LogListView->setWindowTitle(QCoreApplication::translate("LogListView", "Log List", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LogListView: public Ui_LogListView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGLISTVIEW_H
