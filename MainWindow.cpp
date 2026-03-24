#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AppContext.h"
#include "Bootstrap.h"

#include "TraceViewer.h"
#include "LogListView.h"
#include "Binding.h"

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
    m_userVM     = new UserViewModel(this);
    m_sensorVM   = new SensorViewModel(this);
    // LogListView 内部已经创建了 ViewModel，这里直接获取
    m_logListVM  = ui->logListView->viewModel();

    AppContext::instance().ConnectObserve(m_userVM);
    AppContext::instance().ConnectObserve(m_sensorVM);
    AppContext::instance().ConnectObserve(m_logListVM);
}

void MainWindow::setupBindings()
{
    // =========================================================
    // Property Binding（VM → View）- 单向绑定
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

    // 注意：LogListView 内部的 tableView model 绑定已在 LogListView 构造函数中完成

    // ========== 按钮启用状态绑定 ==========
    Binding::BindProperty(ui->btnPublishScore, "enabled", m_userVM, "canPublish");
    auto syncAuthButtons = [this]() {
        const bool loggedIn = m_userVM->loggedIn();
        ui->btnLogin->setEnabled(!loggedIn);
        ui->btnLogout->setEnabled(loggedIn);
    };
    syncAuthButtons();
    QObject::connect(m_userVM, &UserViewModel::loggedInChanged, this, syncAuthButtons);

    // =========================================================
    // Event Binding（View → VM）
    // 直接绑定控件信号到 ViewModel 的虚函数
    // =========================================================

    // ---------- 用户相关 ----------

    // 按钮点击 - 使用 lambda 传递 senderId
    QObject::connect(ui->btnPublishScore, &QAbstractButton::clicked,
        m_userVM, [=]() { m_userVM->onClicked("btnPublishScore"); });
    QObject::connect(ui->btnLogin, &QAbstractButton::clicked,
        m_userVM, [=]() { m_userVM->onClicked("btnLogin"); });
    QObject::connect(ui->btnLogout, &QAbstractButton::clicked,
        m_userVM, [=]() { m_userVM->onClicked("btnLogout"); });

    // CheckBox toggled
    QObject::connect(ui->checkBoxLoggedIn, &QCheckBox::toggled,
        m_userVM, [=](bool checked) { m_userVM->onToggled(checked, "checkBoxLoggedIn"); });

    // ComboBox currentIndexChanged
    QObject::connect(ui->comboBoxMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
        m_userVM, [=](int index) { m_userVM->onCurrentIndexChanged(index, "comboBoxMode"); });

    // SpinBox valueChanged
    QObject::connect(ui->spinBoxCount, QOverload<int>::of(&QSpinBox::valueChanged),
        m_userVM, [=](int value) { m_userVM->onValueChanged(value, "spinBoxCount"); });

    // LineEdit textEdited
    QObject::connect(ui->lineEdit, &QLineEdit::textEdited,
        m_userVM, [=](const QString& text) { m_userVM->onTextEdited(text, "lineEdit"); });

    // ---------- 传感器相关 ----------

    // 按钮点击
    QObject::connect(ui->btnPublishTemperature, &QAbstractButton::clicked,
        m_sensorVM, [=]() { m_sensorVM->onClicked("btnPublishTemperature"); });

    // Slider valueChanged
    QObject::connect(ui->horizontalSliderTemp, &QSlider::valueChanged,
        m_sensorVM, [=](int value) { m_sensorVM->onValueChanged(value, "horizontalSliderTemp"); });

    // DoubleSpinBox valueChanged
    QObject::connect(ui->doubleSpinBoxGain, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        m_sensorVM, [=](double value) { m_sensorVM->onValueChangedDouble(value, "doubleSpinBoxGain"); });
}

void MainWindow::on_pushButton_clicked()
{
    auto viewer = new TraceViewer("run_trace.db", nullptr);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->show();
}
