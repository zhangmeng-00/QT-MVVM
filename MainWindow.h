#pragma once

#include <QMainWindow>
#include <QThread>
#include <memory>
#include <vector>

#include "core/mediator/Mediator.h"
#include "model/SensorModel.h"
#include "model/UserModel.h"
#include "view/LabelViewObserve.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*
 * MainWindow
 * ============================================================
 * MVA(Qt) + Actor/Mailbox Demo
 *
 * 线程结构：
 * ------------------------------------------------------------
 * 1) UI 线程：MainWindow / View（LabelViewObserve）
 * 2) Mediator 线程：Mediator（总线）
 * 3) Actor 线程1：SensorModel（ActorObserve）
 * 4) Actor 线程2：UserModel（ActorObserve）
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnPublish_clicked();

private:
    void SetupThreads();
    void SetupObjects();
    void SetupConnections();
    void SetupSubscriptions();

private:
    Ui::MainWindow *ui = nullptr;

    // ============ Mediator 线程 ============
    Mediator* m_mediator = nullptr;
    QThread*  m_mediatorThread = nullptr;

    // ============ Actor(Model) 线程 ============
    SensorModel* m_sensorModel = nullptr;
    QThread*     m_sensorThread = nullptr;

    UserModel*   m_userModel = nullptr;
    QThread*     m_userThread = nullptr;

    // ============ View（UI线程，不 moveToThread） ============
    LabelViewObserve* m_tempView  = nullptr;
    LabelViewObserve* m_scoreView = nullptr;
    LabelViewObserve* m_levelView = nullptr;
};
