#pragma once

#include <QMainWindow>

// Core
#include "core/policy/AlwaysPolicy.h"
#include "core/policy/ValueChangedPolicy.h"

// Models
#include "model/UserModel.h"
#include "model/SensorModel.h"

// ViewModels
#include "viewmodel/UserViewModel.h"
#include "viewmodel/SensorViewModel.h"

// Binding helpers
#include "core/binding/Binding.h"
#include "core/binding/BindingCommand.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    void setupViewModels();
    void setupBindings();

private:
    Ui::MainWindow* ui = nullptr;

    // Models
    UserModel*   m_userModel   = nullptr;
    SensorModel* m_sensorModel = nullptr;

    // ViewModels
    UserViewModel*   m_userVM   = nullptr;
    SensorViewModel* m_sensorVM = nullptr;
};
