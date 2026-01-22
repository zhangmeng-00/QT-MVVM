#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AppContext.h"
#include "model/LoggerActor.h"
#include "model/SQLiteRecorderActor.h"
#include "view/TraceViewer.h"

#include <QAbstractButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QCheckBox>

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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupModels()
{
    m_userModel   = new UserModel(this);
    m_sensorModel = new SensorModel(this);

    auto logger   = new LoggerActor(&AppContext::instance());
    auto recorder = new SQLiteRecorderActor("run_trace.db", &AppContext::instance());

    AppContext::instance().ConnectObserve(m_userModel);
    AppContext::instance().ConnectObserve(m_sensorModel);
    AppContext::instance().ConnectObserve(logger);
    AppContext::instance().ConnectObserve(recorder);

    logger->Init();
    recorder->Init();
}

void MainWindow::setupViewModels()
{
    m_userVM   = new UserViewModel(this);
    m_sensorVM = new SensorViewModel(this);

    AppContext::instance().ConnectObserve(m_userVM);
    AppContext::instance().ConnectObserve(m_sensorVM);
}

void MainWindow::setupSubscriptions()
{
    // UserModel
    m_userModel->Subscribe("user/score", std::make_shared<AlwaysPolicy>());

    // ViewModels
    m_userVM->Subscribe("user/score", std::make_shared<AlwaysPolicy>());
    m_userVM->Subscribe("user/level", std::make_shared<ValueChangedPolicy>());

    m_sensorVM->Subscribe("sensor/temperature", std::make_shared<AlwaysPolicy>());
}

void MainWindow::setupBindings()
{
    // ---------- Property Binding（VM -> View）----------
    Binding::BindProperty(ui->labelScore, "text", m_userVM, "scoreText");
    Binding::BindProperty(ui->labelLevel, "text", m_userVM, "levelText");
    Binding::BindProperty(ui->labelTemperature, "text", m_sensorVM, "temperatureText");

    // 注意：你当前 Binding 是单向 VM->View；lineEdit 的输入不会回写 VM（除非你另写命令/双向绑定）
    Binding::BindProperty(ui->lineEdit, "text", m_userVM, "userName");

    // ---------- Command Binding（新标准：sender + signal + command）----------
    BindingCommand::BindCommand(ui->btnPublishScore,       &QAbstractButton::clicked, m_userVM->publishScoreCommand());
    BindingCommand::BindCommand(ui->btnPublishTemperature, &QAbstractButton::clicked, m_sensorVM->publishTemperatureCommand());
    BindingCommand::BindCommand(ui->btnLogin,              &QAbstractButton::clicked, m_userVM->loginCommand());
    BindingCommand::BindCommand(ui->btnLogout,             &QAbstractButton::clicked, m_userVM->logoutCommand());

    Binding::BindProperty(ui->labelLoginState, "text", m_userVM, "loginStateText");
    Binding::BindProperty(ui->labelModeValue, "text", m_userVM, "modeText");
    Binding::BindProperty(ui->labelCountValue, "text", m_userVM, "countText");
    Binding::BindProperty(ui->labelSliderValue, "text", m_sensorVM, "targetTempText");
    Binding::BindProperty(ui->labelGainValue, "text", m_sensorVM, "gainText");


    // ---------- 其它控件事件（示例：默认注释掉，避免你 VM 还没实现对应命令而编译失败）----------

    // 1) QSlider::valueChanged(int) —— 参数 int 会进 ExecuteArgs(args)
    BindingCommand::BindCommand(
        ui->horizontalSliderTemp,
        &QSlider::valueChanged,
        m_sensorVM->setTargetTemperatureCommand()   // 你需要在 SensorViewModel 里新增这个命令
    );

    // 2) QCheckBox::toggled(bool)
    BindingCommand::BindCommand(
        ui->checkBoxLoggedIn,
        &QCheckBox::toggled,
        m_userVM->toggleLoginCommand()              // 你需要在 UserViewModel 里新增这个命令
    );

    // 3) QComboBox::currentIndexChanged(int) —— 注意重载要用 QOverload
    BindingCommand::BindCommand(
        ui->comboBoxMode,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        m_userVM->modeChangedCommand()              // 你需要在 UserViewModel 里新增这个命令
    );

    // 4) QSpinBox::valueChanged(int)
    BindingCommand::BindCommand(
        ui->spinBoxCount,
        QOverload<int>::of(&QSpinBox::valueChanged),
        m_userVM->countChangedCommand()             // 你需要在 UserViewModel 里新增这个命令
    );

    // 5) QDoubleSpinBox::valueChanged(double)
    BindingCommand::BindCommand(
        ui->doubleSpinBoxGain,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        m_sensorVM->gainChangedCommand()            // 你需要在 SensorViewModel 里新增这个命令
    );

    // 6) Apply 按钮：enabled 由 slider 的值决定（通过 canExecArgsGetter 提供参数给 CanExecuteArgs）
    BindingCommand::BindCommand(
        ui->btnApply,
        &QAbstractButton::clicked,
        m_userVM->applyCommand(),
        ui->btnApply,
        [this](){ return QVariantList{ ui->horizontalSliderTemp->value() }; }
    );
}

void MainWindow::on_pushButton_clicked()
{
    auto viewer = new TraceViewer("run_trace.db", nullptr);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}
