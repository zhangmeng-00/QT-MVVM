#include "MainWindow.h"
#include "ui_MainWindow.h"

/*
 * 构造函数
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupMediator();
    setupModels();
    setupViewModels();
    setupSubscriptions();
    setupBindings();
}

/*
 * 析构函数
 */
MainWindow::~MainWindow()
{
    // 先停线程，再销毁对象
    if (m_mediatorThread) {
        m_mediatorThread->quit();
        m_mediatorThread->wait();
    }

    delete m_mediator;
    delete ui;
}

/*
 * 1️⃣ 创建并启动 Mediator（可独立线程）
 */
void MainWindow::setupMediator()
{
    m_mediator = new Mediator;

    // 如果你暂时不想用线程，可以直接 return
    m_mediatorThread = new QThread(this);
    m_mediator->moveToThread(m_mediatorThread);
    m_mediatorThread->start();
}

/*
 * 2️⃣ 创建 Model（纯业务层）
 */
void MainWindow::setupModels()
{
    m_userModel   = new UserModel(this);
    m_sensorModel = new SensorModel(this);

    // Model 接入 Mediator
    m_mediator->ConnectObserve(m_userModel);
    m_mediator->ConnectObserve(m_sensorModel);
}

/*
 * 3️⃣ 创建 ViewModel（UI 逻辑层）
 */
void MainWindow::setupViewModels()
{
    m_userVM   = new UserViewModel(this);
    m_sensorVM = new SensorViewModel(this);

    // ViewModel 接入 Mediator
    m_mediator->ConnectObserve(m_userVM);
    m_mediator->ConnectObserve(m_sensorVM);
}

/*
 * 4️⃣ 建立订阅关系（唯一一次）
 */
void MainWindow::setupSubscriptions()
{
    // UserModel
    m_userModel->Subscribe("user/score",std::make_shared<AlwaysPolicy>());

    // ViewModels
    m_userVM->Subscribe("user/score",std::make_shared<AlwaysPolicy>());
    m_userVM->Subscribe("user/level",std::make_shared<AlwaysPolicy>());

    m_sensorVM->Subscribe("sensor/temperature",std::make_shared<AlwaysPolicy>());
    qDebug() << ">>> setupSubscriptions called";

}

/*
 * 5️⃣ UI 绑定（Property + Command）
 */
void MainWindow::setupBindings()
{
    // ---------- Property Binding ----------
    Binding::BindLabel(ui->labelScore,
                       m_userVM,
                       "scoreText");

    Binding::BindLabel(ui->labelLevel,
                       m_userVM,
                       "levelText");

    Binding::BindLabel(ui->labelTemperature,
                       m_sensorVM,
                       "temperatureText");

    // ---------- Command Binding ----------
    BindingCommand::BindCommand(
        ui->btnPublishScore,
        m_userVM,
        &UserViewModel::publishCommand
        );


    BindingCommand::BindCommand(
        ui->btnPublishTemperature,
        m_sensorVM,
        &SensorViewModel::publishCommand
        );

}
