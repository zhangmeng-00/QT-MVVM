#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "core/app/AppContext.h"
#include "core/app/Bootstrap.h"

#include "view/TraceViewer.h"
#include "core/binding/BindingCommandHelpers.h"

#include <QAbstractButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QLineEdit>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ✅ 装配全局组件（只做一次）
    Bootstrap::InstallAll(AppContext::instance());

    setupViewModels();
    setupBindings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupViewModels()
{
    m_userVM   = new UserViewModel(this);
    m_sensorVM = new SensorViewModel(this);

    AppContext::instance().ConnectObserve(m_userVM);
    AppContext::instance().ConnectObserve(m_sensorVM);
}

void MainWindow::setupBindings()
{
    // ---------- Property Binding（VM -> View）----------
    Binding::BindProperty(ui->labelScore, "text", m_userVM, "scoreText");
    Binding::BindProperty(ui->labelLevel, "text", m_userVM, "levelText");
    Binding::BindProperty(ui->labelTemperature, "text", m_sensorVM, "temperatureText");

    Binding::BindProperty(ui->lineEdit, "text", m_userVM, "userName"); // 单向 VM->View（输入回写靠命令）

    Binding::BindProperty(ui->labelLoginState, "text", m_userVM, "loginStateText");
    Binding::BindProperty(ui->labelModeValue,  "text", m_userVM, "modeText");
    Binding::BindProperty(ui->labelCountValue, "text", m_userVM, "countText");

    Binding::BindProperty(ui->labelSliderValue,"text", m_sensorVM, "targetTempText");
    Binding::BindProperty(ui->labelGainValue,  "text", m_sensorVM, "gainText");

    // =========================================================
    // Command Binding
    // 约定：UserViewModel::uiCommand 接收 args = [tag, payload]
    // =========================================================


    // ---------- 用户相关：统一走 m_userVM->uiCommand() ----------
    // 1) 发布随机积分
    using namespace BindingCommandHelpers;
    BindingCommand::BindCommand(
        ui->btnPublishScore,
        &QAbstractButton::clicked,
        m_userVM->uiCommand(),
        ui->btnPublishScore,
        CanArgsConst("user/publish_score", true),
        ToTagConst("user/publish_score", true)
        );

    // 2) 登录
    BindingCommand::BindCommand(
        ui->btnLogin,
        &QAbstractButton::clicked,
        m_userVM->uiCommand(),
        ui->btnLogin,
        CanArgsConst("user/login", true),
        ToTagConst("user/login", true)
        );

    // 3) 登出
    BindingCommand::BindCommand(
        ui->btnLogout,
        &QAbstractButton::clicked,
        m_userVM->uiCommand(),
        ui->btnLogout,
        CanArgsConst("user/logout", true),
        ToTagConst("user/logout", true)
        );

    // 4) CheckBox：toggled(bool) -> user/logged_in
    BindingCommand::BindCommand(
        ui->checkBoxLoggedIn,
        &QCheckBox::toggled,
        m_userVM->uiCommand(),
        ui->checkBoxLoggedIn,
        //[this](){ return QVariantList{ "user/logged_in", ui->checkBoxLoggedIn->isChecked() }; },
        CanArgsDynamic("user/logged_in", [this]()->bool { return ui->checkBoxLoggedIn->isChecked(); }),
        ToTagPayload("user/logged_in")
        );

    // 5) ComboBox：currentIndexChanged(int) -> user/mode
    BindingCommand::BindCommand(
        ui->comboBoxMode,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        m_userVM->uiCommand(),
        ui->comboBoxMode,
        //[this](){ return QVariantList{ "user/mode", ui->comboBoxMode->currentIndex() }; },
        CanArgsDynamic("user/mode",      [this]()->int  { return ui->comboBoxMode->currentIndex(); }),
        ToTagPayload("user/mode")
        );

    // 6) SpinBox：valueChanged(int) -> user/count
    BindingCommand::BindCommand(
        ui->spinBoxCount,
        QOverload<int>::of(&QSpinBox::valueChanged),
        m_userVM->uiCommand(),
        ui->spinBoxCount,
        CanArgsDynamic("user/count",     [this]()->int  { return ui->spinBoxCount->value(); }),
        ToTagPayload("user/count")
        );

    // 7) LineEdit：textEdited(QString) -> user/name
    // 说明：用 textEdited（用户输入才触发），避免 BindProperty VM->View 写回导致环路
    BindingCommand::BindCommand(
        ui->lineEdit,
        &QLineEdit::textEdited,
        m_userVM->uiCommand(),
        ui->lineEdit,
        CanArgsDynamic("user/name",      [this]()->QString { return ui->lineEdit->text(); }),
        ToTagPayload("user/name")
        );

    // ---------- 传感器相关：你原来命令写法保留 ----------
    // 发布温度（按钮）
    BindingCommand::BindCommand(
        ui->btnPublishTemperature,
        &QAbstractButton::clicked,
        m_sensorVM->publishTemperatureCommand()
        );

    // Slider：valueChanged(int) -> setTargetTemperatureCommand（它自己解析 args[0] 即可）
    BindingCommand::BindCommand(
        ui->horizontalSliderTemp,
        &QSlider::valueChanged,
        m_sensorVM->setTargetTemperatureCommand()
        );

    // DoubleSpinBox：valueChanged(double) -> gainChangedCommand（它自己解析 args[0] 即可）
    BindingCommand::BindCommand(
        ui->doubleSpinBoxGain,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        m_sensorVM->gainChangedCommand()
        );

    // ---------- Apply 示例（如果你 UserVM 还没实现 user/apply，就先别绑）----------
    // 如果你想做：点击 Apply 把 slider 当前值发布出去
    // 那就用下面这段（前提：UserViewModel 注册了 "user/apply" action）
    /*
    BindingCommand::BindCommand(
        ui->btnApply,
        &QAbstractButton::clicked,
        m_userVM->uiCommand(),
        ui->btnApply,
        [this](){ return QVariantList{ "user/apply", ui->horizontalSliderTemp->value() }; },
        [this](const QVariantList&){ return QVariantList{ "user/apply", ui->horizontalSliderTemp->value() }; }
    );
    */
}

void MainWindow::on_pushButton_clicked()
{
    auto viewer = new TraceViewer("run_trace.db", nullptr);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}
