#pragma once

#include <QObject>
#include <QPushButton>
#include <QDebug>

/*
 * BindingCommand
 * ============================================================
 * Command Binding（View → ViewModel）
 */
namespace BindingCommand {

/*
 * BindCommand
 * ------------------------------------------------------------
 * QPushButton.clicked → ViewModel slot
 */
template<typename T>
inline void BindCommand(QPushButton* button,
                        T* viewModel,
                        void (T::*slot)())
{
    if (!button || !viewModel) {
        qWarning() << "[BindingCommand] button or viewModel is null";
        return;
    }

    bool ok = QObject::connect(
        button,
        &QPushButton::clicked,
        viewModel,
        slot
        );

    if (!ok) {
        qWarning() << "[BindingCommand] connect failed";
    }
}

} // namespace BindingCommand
