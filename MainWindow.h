#pragma once

#include <QMainWindow>

// Core
#include "AlwaysPolicy.h"
#include "ValueChangedPolicy.h"

// Models
#include "UserModel.h"
#include "SensorModel.h"

// ViewModels
#include "UserViewModel.h"
#include "SensorViewModel.h"
#include "LogListViewModel.h"

// Binding helpers
#include "Binding.h"

// ViewModel base
#include "BaseViewModel.h"

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
    LogListViewModel* m_logListVM = nullptr;
};
