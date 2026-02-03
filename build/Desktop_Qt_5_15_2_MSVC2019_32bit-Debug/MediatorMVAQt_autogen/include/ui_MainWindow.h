/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayoutMain;
    QGroupBox *groupStatus;
    QFormLayout *formLayoutStatus;
    QLabel *labelScoreTitle;
    QLabel *labelScore;
    QLabel *labelLevelTitle;
    QLabel *labelLevel;
    QLabel *labelTemperatureTitle;
    QLabel *labelTemperature;
    QLabel *labelLoginTitle;
    QLabel *labelLoginState;
    QGroupBox *groupUser;
    QFormLayout *formLayoutUser;
    QLabel *labelUserNameTitle;
    QLineEdit *lineEdit;
    QGroupBox *groupCommands;
    QGridLayout *gridLayoutCommands;
    QPushButton *btnPublishScore;
    QPushButton *btnPublishTemperature;
    QPushButton *btnLogin;
    QPushButton *btnLogout;
    QPushButton *pushButton;
    QGroupBox *groupDemo;
    QGridLayout *gridLayoutDemo;
    QCheckBox *checkBoxLoggedIn;
    QLabel *labelCheckBoxState;
    QLabel *labelSliderTitle;
    QSlider *horizontalSliderTemp;
    QLabel *labelSliderValue;
    QLabel *labelModeTitle;
    QComboBox *comboBoxMode;
    QLabel *labelModeValue;
    QLabel *labelCountTitle;
    QSpinBox *spinBoxCount;
    QLabel *labelCountValue;
    QLabel *labelGainTitle;
    QDoubleSpinBox *doubleSpinBoxGain;
    QLabel *labelGainValue;
    QPushButton *btnApply;
    QSpacerItem *verticalSpacerBottom;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(980, 640);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutMain = new QVBoxLayout(centralwidget);
        verticalLayoutMain->setObjectName(QString::fromUtf8("verticalLayoutMain"));
        groupStatus = new QGroupBox(centralwidget);
        groupStatus->setObjectName(QString::fromUtf8("groupStatus"));
        formLayoutStatus = new QFormLayout(groupStatus);
        formLayoutStatus->setObjectName(QString::fromUtf8("formLayoutStatus"));
        labelScoreTitle = new QLabel(groupStatus);
        labelScoreTitle->setObjectName(QString::fromUtf8("labelScoreTitle"));

        formLayoutStatus->setWidget(0, QFormLayout::LabelRole, labelScoreTitle);

        labelScore = new QLabel(groupStatus);
        labelScore->setObjectName(QString::fromUtf8("labelScore"));
        labelScore->setMinimumWidth(120);

        formLayoutStatus->setWidget(0, QFormLayout::FieldRole, labelScore);

        labelLevelTitle = new QLabel(groupStatus);
        labelLevelTitle->setObjectName(QString::fromUtf8("labelLevelTitle"));

        formLayoutStatus->setWidget(1, QFormLayout::LabelRole, labelLevelTitle);

        labelLevel = new QLabel(groupStatus);
        labelLevel->setObjectName(QString::fromUtf8("labelLevel"));

        formLayoutStatus->setWidget(1, QFormLayout::FieldRole, labelLevel);

        labelTemperatureTitle = new QLabel(groupStatus);
        labelTemperatureTitle->setObjectName(QString::fromUtf8("labelTemperatureTitle"));

        formLayoutStatus->setWidget(2, QFormLayout::LabelRole, labelTemperatureTitle);

        labelTemperature = new QLabel(groupStatus);
        labelTemperature->setObjectName(QString::fromUtf8("labelTemperature"));

        formLayoutStatus->setWidget(2, QFormLayout::FieldRole, labelTemperature);

        labelLoginTitle = new QLabel(groupStatus);
        labelLoginTitle->setObjectName(QString::fromUtf8("labelLoginTitle"));

        formLayoutStatus->setWidget(3, QFormLayout::LabelRole, labelLoginTitle);

        labelLoginState = new QLabel(groupStatus);
        labelLoginState->setObjectName(QString::fromUtf8("labelLoginState"));

        formLayoutStatus->setWidget(3, QFormLayout::FieldRole, labelLoginState);


        verticalLayoutMain->addWidget(groupStatus);

        groupUser = new QGroupBox(centralwidget);
        groupUser->setObjectName(QString::fromUtf8("groupUser"));
        formLayoutUser = new QFormLayout(groupUser);
        formLayoutUser->setObjectName(QString::fromUtf8("formLayoutUser"));
        labelUserNameTitle = new QLabel(groupUser);
        labelUserNameTitle->setObjectName(QString::fromUtf8("labelUserNameTitle"));

        formLayoutUser->setWidget(0, QFormLayout::LabelRole, labelUserNameTitle);

        lineEdit = new QLineEdit(groupUser);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        formLayoutUser->setWidget(0, QFormLayout::FieldRole, lineEdit);


        verticalLayoutMain->addWidget(groupUser);

        groupCommands = new QGroupBox(centralwidget);
        groupCommands->setObjectName(QString::fromUtf8("groupCommands"));
        gridLayoutCommands = new QGridLayout(groupCommands);
        gridLayoutCommands->setObjectName(QString::fromUtf8("gridLayoutCommands"));
        btnPublishScore = new QPushButton(groupCommands);
        btnPublishScore->setObjectName(QString::fromUtf8("btnPublishScore"));

        gridLayoutCommands->addWidget(btnPublishScore, 0, 0, 1, 1);

        btnPublishTemperature = new QPushButton(groupCommands);
        btnPublishTemperature->setObjectName(QString::fromUtf8("btnPublishTemperature"));

        gridLayoutCommands->addWidget(btnPublishTemperature, 0, 1, 1, 1);

        btnLogin = new QPushButton(groupCommands);
        btnLogin->setObjectName(QString::fromUtf8("btnLogin"));

        gridLayoutCommands->addWidget(btnLogin, 0, 2, 1, 1);

        btnLogout = new QPushButton(groupCommands);
        btnLogout->setObjectName(QString::fromUtf8("btnLogout"));

        gridLayoutCommands->addWidget(btnLogout, 0, 3, 1, 1);

        pushButton = new QPushButton(groupCommands);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayoutCommands->addWidget(pushButton, 1, 0, 1, 4);


        verticalLayoutMain->addWidget(groupCommands);

        groupDemo = new QGroupBox(centralwidget);
        groupDemo->setObjectName(QString::fromUtf8("groupDemo"));
        gridLayoutDemo = new QGridLayout(groupDemo);
        gridLayoutDemo->setObjectName(QString::fromUtf8("gridLayoutDemo"));
        checkBoxLoggedIn = new QCheckBox(groupDemo);
        checkBoxLoggedIn->setObjectName(QString::fromUtf8("checkBoxLoggedIn"));

        gridLayoutDemo->addWidget(checkBoxLoggedIn, 0, 0, 1, 2);

        labelCheckBoxState = new QLabel(groupDemo);
        labelCheckBoxState->setObjectName(QString::fromUtf8("labelCheckBoxState"));

        gridLayoutDemo->addWidget(labelCheckBoxState, 0, 2, 1, 2);

        labelSliderTitle = new QLabel(groupDemo);
        labelSliderTitle->setObjectName(QString::fromUtf8("labelSliderTitle"));

        gridLayoutDemo->addWidget(labelSliderTitle, 1, 0, 1, 1);

        horizontalSliderTemp = new QSlider(groupDemo);
        horizontalSliderTemp->setObjectName(QString::fromUtf8("horizontalSliderTemp"));
        horizontalSliderTemp->setOrientation(Qt::Horizontal);
        horizontalSliderTemp->setMinimum(0);
        horizontalSliderTemp->setMaximum(100);
        horizontalSliderTemp->setValue(30);

        gridLayoutDemo->addWidget(horizontalSliderTemp, 1, 1, 1, 2);

        labelSliderValue = new QLabel(groupDemo);
        labelSliderValue->setObjectName(QString::fromUtf8("labelSliderValue"));

        gridLayoutDemo->addWidget(labelSliderValue, 1, 3, 1, 1);

        labelModeTitle = new QLabel(groupDemo);
        labelModeTitle->setObjectName(QString::fromUtf8("labelModeTitle"));

        gridLayoutDemo->addWidget(labelModeTitle, 2, 0, 1, 1);

        comboBoxMode = new QComboBox(groupDemo);
        comboBoxMode->addItem(QString());
        comboBoxMode->addItem(QString());
        comboBoxMode->addItem(QString());
        comboBoxMode->setObjectName(QString::fromUtf8("comboBoxMode"));

        gridLayoutDemo->addWidget(comboBoxMode, 2, 1, 1, 1);

        labelModeValue = new QLabel(groupDemo);
        labelModeValue->setObjectName(QString::fromUtf8("labelModeValue"));

        gridLayoutDemo->addWidget(labelModeValue, 2, 2, 1, 2);

        labelCountTitle = new QLabel(groupDemo);
        labelCountTitle->setObjectName(QString::fromUtf8("labelCountTitle"));

        gridLayoutDemo->addWidget(labelCountTitle, 3, 0, 1, 1);

        spinBoxCount = new QSpinBox(groupDemo);
        spinBoxCount->setObjectName(QString::fromUtf8("spinBoxCount"));
        spinBoxCount->setMinimum(0);
        spinBoxCount->setMaximum(9999);
        spinBoxCount->setValue(10);

        gridLayoutDemo->addWidget(spinBoxCount, 3, 1, 1, 1);

        labelCountValue = new QLabel(groupDemo);
        labelCountValue->setObjectName(QString::fromUtf8("labelCountValue"));

        gridLayoutDemo->addWidget(labelCountValue, 3, 2, 1, 2);

        labelGainTitle = new QLabel(groupDemo);
        labelGainTitle->setObjectName(QString::fromUtf8("labelGainTitle"));

        gridLayoutDemo->addWidget(labelGainTitle, 4, 0, 1, 1);

        doubleSpinBoxGain = new QDoubleSpinBox(groupDemo);
        doubleSpinBoxGain->setObjectName(QString::fromUtf8("doubleSpinBoxGain"));
        doubleSpinBoxGain->setMinimum(0.000000000000000);
        doubleSpinBoxGain->setMaximum(1000.000000000000000);
        doubleSpinBoxGain->setDecimals(3);
        doubleSpinBoxGain->setSingleStep(0.100000000000000);
        doubleSpinBoxGain->setValue(1.000000000000000);

        gridLayoutDemo->addWidget(doubleSpinBoxGain, 4, 1, 1, 1);

        labelGainValue = new QLabel(groupDemo);
        labelGainValue->setObjectName(QString::fromUtf8("labelGainValue"));

        gridLayoutDemo->addWidget(labelGainValue, 4, 2, 1, 2);

        btnApply = new QPushButton(groupDemo);
        btnApply->setObjectName(QString::fromUtf8("btnApply"));

        gridLayoutDemo->addWidget(btnApply, 5, 0, 1, 4);


        verticalLayoutMain->addWidget(groupDemo);

        verticalSpacerBottom = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayoutMain->addItem(verticalSpacerBottom);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MediatorMVAQt Demo", nullptr));
        groupStatus->setTitle(QCoreApplication::translate("MainWindow", "Live Status", nullptr));
        labelScoreTitle->setText(QCoreApplication::translate("MainWindow", "Score:", nullptr));
        labelScore->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        labelLevelTitle->setText(QCoreApplication::translate("MainWindow", "Level:", nullptr));
        labelLevel->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        labelTemperatureTitle->setText(QCoreApplication::translate("MainWindow", "Temperature:", nullptr));
        labelTemperature->setText(QCoreApplication::translate("MainWindow", "0.0 \342\204\203", nullptr));
        labelLoginTitle->setText(QCoreApplication::translate("MainWindow", "Login State:", nullptr));
        labelLoginState->setText(QCoreApplication::translate("MainWindow", "Logged Out", nullptr));
        groupUser->setTitle(QCoreApplication::translate("MainWindow", "User", nullptr));
        labelUserNameTitle->setText(QCoreApplication::translate("MainWindow", "User Name:", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Input user name (demo)", nullptr));
        groupCommands->setTitle(QCoreApplication::translate("MainWindow", "Commands", nullptr));
        btnPublishScore->setText(QCoreApplication::translate("MainWindow", "Publish Score", nullptr));
        btnPublishTemperature->setText(QCoreApplication::translate("MainWindow", "Publish Temperature", nullptr));
        btnLogin->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        btnLogout->setText(QCoreApplication::translate("MainWindow", "Logout", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Open Trace Viewer", nullptr));
        groupDemo->setTitle(QCoreApplication::translate("MainWindow", "Other Widgets / Events Demo (optional)", nullptr));
        checkBoxLoggedIn->setText(QCoreApplication::translate("MainWindow", "Logged In (toggled(bool))", nullptr));
        labelCheckBoxState->setText(QCoreApplication::translate("MainWindow", "False", nullptr));
        labelSliderTitle->setText(QCoreApplication::translate("MainWindow", "Temp Target Slider:", nullptr));
        labelSliderValue->setText(QCoreApplication::translate("MainWindow", "30", nullptr));
        labelModeTitle->setText(QCoreApplication::translate("MainWindow", "Mode:", nullptr));
        comboBoxMode->setItemText(0, QCoreApplication::translate("MainWindow", "Mode A", nullptr));
        comboBoxMode->setItemText(1, QCoreApplication::translate("MainWindow", "Mode B", nullptr));
        comboBoxMode->setItemText(2, QCoreApplication::translate("MainWindow", "Mode C", nullptr));

        labelModeValue->setText(QCoreApplication::translate("MainWindow", "Mode A", nullptr));
        labelCountTitle->setText(QCoreApplication::translate("MainWindow", "Count:", nullptr));
        labelCountValue->setText(QCoreApplication::translate("MainWindow", "10", nullptr));
        labelGainTitle->setText(QCoreApplication::translate("MainWindow", "Gain:", nullptr));
        labelGainValue->setText(QCoreApplication::translate("MainWindow", "1.000", nullptr));
        btnApply->setText(QCoreApplication::translate("MainWindow", "Apply (demo)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
