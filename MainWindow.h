#pragma once

#include <QMainWindow>
#include <QThread>

// Core
#include "core/mediator/Mediator.h"
#include "core/policy/AlwaysPolicy.h"

// Models
#include "model/UserModel.h"
#include "model/SensorModel.h"

// ViewModels
#include "viewmodel/UserViewModel.h"
#include "viewmodel/SensorViewModel.h"

// Binding helpers
#include "view/binding/Binding.h"
#include "view/binding/BindingCommand.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*
 * MainWindow
 * ============================================================
 * 职责：
 * - 作为 Composition Root（装配点）
 * - 创建 Mediator / Model / ViewModel
 * - 建立订阅与绑定
 *
 * 不包含任何业务逻辑
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    void setupMediator();
    void setupModels();
    void setupViewModels();
    void setupSubscriptions();
    void setupBindings();

private:
    Ui::MainWindow* ui = nullptr;

    // Mediator（事件总线）
    Mediator* m_mediator = nullptr;
    QThread*  m_mediatorThread = nullptr;

    // Models
    UserModel*   m_userModel   = nullptr;
    SensorModel* m_sensorModel = nullptr;

    // ViewModels
    UserViewModel*   m_userVM   = nullptr;
    SensorViewModel* m_sensorVM = nullptr;
};
