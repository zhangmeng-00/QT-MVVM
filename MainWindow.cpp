#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QRandomGenerator>
#include "core/policy/AlwaysPolicy.h"

/*
 * 构造函数
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetupThreads();
    SetupObjects();
    SetupConnections();
    SetupSubscriptions();

    // 按钮信号连接（也可用 auto-connect on_btnPublish_clicked）
    connect(ui->btnPublish, &QPushButton::clicked,
            this, &MainWindow::on_btnPublish_clicked);
}

/*
 * 析构函数
 *
 * 退出顺序（关键）：
 * ------------------------------------------------------------
 * 1) 先 quit Actor 线程、Mediator 线程
 * 2) thread finished -> deleteLater 对象（在其线程安全销毁）
 * 3) wait 等线程结束
 *
 * 注意：
 * - 我们不在 Observe 析构 emit 任何 queued signal
 * - Mediator 通过 QObject::destroyed 自动清订阅
 */
MainWindow::~MainWindow()
{
    // 先停 Actor 线程
    if (m_sensorThread) {
        m_sensorThread->quit();
        m_sensorThread->wait();
    }
    if (m_userThread) {
        m_userThread->quit();
        m_userThread->wait();
    }

    // 再停 Mediator 线程
    if (m_mediatorThread) {
        m_mediatorThread->quit();
        m_mediatorThread->wait();
    }

    // 线程对象（有 parent=this）会自动 delete
    delete ui;
}

/*
 * 创建线程并启动
 */
void MainWindow::SetupThreads()
{
    // Mediator 线程
    m_mediatorThread = new QThread(this);
    m_mediatorThread->setObjectName("MediatorThread");
    m_mediatorThread->start();

    // Sensor Actor 线程
    m_sensorThread = new QThread(this);
    m_sensorThread->setObjectName("SensorActorThread");
    m_sensorThread->start();

    // User Actor 线程
    m_userThread = new QThread(this);
    m_userThread->setObjectName("UserActorThread");
    m_userThread->start();
}

/*
 * 创建对象并 moveToThread
 */
void MainWindow::SetupObjects()
{
    // ============ Mediator ============
    m_mediator = new Mediator();
    m_mediator->moveToThread(m_mediatorThread);

    // 线程结束时，安全销毁对象（deleteLater 会在对象所在线程执行）
    connect(m_mediatorThread, &QThread::finished,
            m_mediator, &QObject::deleteLater);

    // ============ Actor Models ============
    m_sensorModel = new SensorModel();
    m_sensorModel->moveToThread(m_sensorThread);
    connect(m_sensorThread, &QThread::finished,
            m_sensorModel, &QObject::deleteLater);

    m_userModel = new UserModel();
    m_userModel->moveToThread(m_userThread);
    connect(m_userThread, &QThread::finished,
            m_userModel, &QObject::deleteLater);

    // ============ Views（UI线程） ============
    m_tempView  = new LabelViewObserve(ui->labelTemperature, this);
    m_scoreView = new LabelViewObserve(ui->labelScore, this);
    m_levelView = new LabelViewObserve(ui->labelLevel, this);
}

/*
 * 接入 Mediator（建立 queued 连接）
 */
void MainWindow::SetupConnections()
{
    // Model / View 都接入 Mediator（Mediated Data Bus）
    m_mediator->ConnectObserve(m_sensorModel);
    m_mediator->ConnectObserve(m_userModel);

    m_mediator->ConnectObserve(m_tempView);
    m_mediator->ConnectObserve(m_scoreView);
    m_mediator->ConnectObserve(m_levelView);
}

/*
 * 建立订阅关系（策略：AlwaysPolicy）
 *
 * 说明：
 * - SubscribeRequest 发信号到 Mediator（queued）
 * - Mediator 会 GetOrCreateTopic + AddSubscription
 * - Publish 时会 Notify 订阅者
 * - ActorObserve 的 OnDataReceived 会投递到自身 Mailbox（串行、线程正确）
 */
void MainWindow::SetupSubscriptions()
{
    auto always = std::make_shared<AlwaysPolicy>();

    // Model：UserModel 订阅 user:score，内部算 level 再发布 user:level
    m_userModel->Subscribe("score", always);

    // View：显示温度/积分/等级
    m_tempView->Subscribe("temperature", always);
    m_scoreView->Subscribe("score", always);
    m_levelView->Subscribe("level", always);
}

/*
 * 点击按钮：模拟外部数据发布
 */
void MainWindow::on_btnPublish_clicked()
{
    int temperature = QRandomGenerator::global()->bounded(10, 35);
    int score       = QRandomGenerator::global()->bounded(0, 500);

    // 发布温度（通过 SensorModel 发布，实际谁发布都行）
    m_sensorModel->Publish("temperature", temperature);

    // 发布积分（UserModel 订阅后会计算 level 并再次发布）
    m_sensorModel->Publish("score", score);
}
