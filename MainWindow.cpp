#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "app/AppContext.h"
#include "app/Bootstrap.h"

#include "view/TraceViewer.h"
#include "view/LogListView.h"
#include "core/binding/BindingEvents.h"
#include "core/binding/BindingEventsHelpers.h"

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
    using namespace BindingEvents;
    using namespace BindingEventsHelpers;

    // =========================================================
    // Property Binding（VM → View）- 保留原有单向绑定
    // =========================================================
    Binding::BindProperty(ui->labelScore, "text", m_userVM, "scoreText");
    Binding::BindProperty(ui->labelLevel, "text", m_userVM, "levelText");
    Binding::BindProperty(ui->labelTemperature, "text", m_sensorVM, "temperatureText");

    // LineEdit 初始值从 VM 获取（但不反向绑定，使用下面的事件绑定）
    Binding::BindProperty(ui->lineEdit, "text", m_userVM, "userName");

    Binding::BindProperty(ui->labelLoginState, "text", m_userVM, "loginStateText");
    Binding::BindProperty(ui->labelModeValue,  "text", m_userVM, "modeText");
    Binding::BindProperty(ui->labelCountValue, "text", m_userVM, "countText");

    Binding::BindProperty(ui->labelSliderValue,"text", m_sensorVM, "targetTempText");
    Binding::BindProperty(ui->labelGainValue,  "text", m_sensorVM, "gainText");

    // =========================================================
    // Event Binding（View → VM → Mediator）
    // 新架构：控件事件直接绑定到 ViewModel::Publish
    // =========================================================

    // ---------- 用户相关 ----------

    // 1) 发布随机积分 - 点击按钮直接发布
    BindEventToPublish(ui->btnPublishScore, &QAbstractButton::clicked,
        m_userVM, "user/publish_score", ConstPayload(true));

    // 2) 登录 - 点击按钮发布
    BindEventToPublish(ui->btnLogin, &QAbstractButton::clicked,
        m_userVM, "user/login", ConstPayload(true));

    // 3) 登出 - 点击按钮发布
    BindEventToPublish(ui->btnLogout, &QAbstractButton::clicked,
        m_userVM, "user/logout", ConstPayload(true));

    // 4) CheckBox：toggled(bool) -> user/logged_in
    BindEventToPublish(ui->checkBoxLoggedIn, &QCheckBox::toggled,
        m_userVM, "user/logged_in", FromBool());

    // 5) ComboBox：currentIndexChanged(int) -> user/mode
    BindEventToPublish(ui->comboBoxMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
        m_userVM, "user/mode", FromInt());

    // 6) SpinBox：valueChanged(int) -> user/count
    BindEventToPublish(ui->spinBoxCount, QOverload<int>::of(&QSpinBox::valueChanged),
        m_userVM, "user/count", FromInt());

    // 7) LineEdit：textEdited(QString) -> user/name
    // 用 textEdited（用户输入才触发），避免和上面的 BindProperty 形成环路
    BindEventToPublish(ui->lineEdit, &QLineEdit::textEdited,
        m_userVM, "user/name", FromString());

    // ---------- 传感器相关 ----------

    // 发布温度（按钮）
    BindEventToPublish(ui->btnPublishTemperature, &QAbstractButton::clicked,
        m_sensorVM, "sensor/publish_temperature", ConstPayload(true));

    // Slider：valueChanged(int) -> sensor/target_temperature
    BindEventToPublish(ui->horizontalSliderTemp, &QSlider::valueChanged,
        m_sensorVM, "sensor/target_temperature", FromInt());

    // DoubleSpinBox：valueChanged(double) -> sensor/gain
    BindEventToPublish(ui->doubleSpinBoxGain, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        m_sensorVM, "sensor/gain", FromDouble());
}

void MainWindow::on_pushButton_clicked()
{
    auto viewer = new TraceViewer("run_trace.db", nullptr);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}
