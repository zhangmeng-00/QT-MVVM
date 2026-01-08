#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AppContext.h"
#include "model/LoggerActor.h"
#include "model/RecorderActor.h"
#include "model/SQLiteRecorderActor.h"
#include "view/TraceViewer.h"

/*
 * 构造函数
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
        delete ui;
}

/*
 * 2️⃣ 创建 Model（纯业务层）
 */
void MainWindow::setupModels()
{
    m_userModel   = new UserModel(this);
    m_sensorModel = new SensorModel(this);
    auto logger   = new LoggerActor(&AppContext::instance());
    //auto recorder = new RecorderActor("run_trace.csv",
    auto recorder = new SQLiteRecorderActor("run_trace.db",&AppContext::instance());



    // Model 接入 Mediator
    AppContext::instance().ConnectObserve(m_userModel);
    AppContext::instance().ConnectObserve(m_sensorModel);
    AppContext::instance().ConnectObserve(logger);
    AppContext::instance().ConnectObserve(recorder);


    // 初始化Model
    logger->Init();
    recorder->Init();
}

/*
 * 3️⃣ 创建 ViewModel（UI 逻辑层）
 */
void MainWindow::setupViewModels()
{
    m_userVM   = new UserViewModel(this);
    m_sensorVM = new SensorViewModel(this);

    // ViewModel 接入 Mediator
    AppContext::instance().ConnectObserve(m_userVM);
    AppContext::instance().ConnectObserve(m_sensorVM);
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
    Binding::BindProperty(ui->labelScore,"text",
                       m_userVM,
                       "scoreText");
    Binding::BindProperty(ui->lineEdit,"text",
                          m_userVM,
                          "userName");

    Binding::BindProperty(ui->labelLevel,"text",
                       m_userVM,
                       "levelText");

    Binding::BindProperty(ui->labelTemperature,"text",
                       m_sensorVM,
                       "temperatureText");

    // ---------- Command Binding ----------
    BindingCommand::Bind(
        ui->btnPublishScore,
        &QPushButton::clicked,
        m_userVM,
        &UserViewModel::publishCommand
        );


    BindingCommand::Bind(
        ui->btnPublishTemperature,
        &QPushButton::clicked,
        m_sensorVM,
        &SensorViewModel::publishCommand
        );

}

void MainWindow::on_pushButton_clicked()
{
    auto viewer = new TraceViewer("run_trace.db", nullptr);
    viewer->setAttribute(Qt::WA_DeleteOnClose); // 关闭即销毁（推荐）
    viewer->show();

}

