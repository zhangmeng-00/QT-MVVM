#pragma once

#include <QObject>
#include <QPushButton>
#include <QDebug>

#include "core/command/ICommand.h"

/*
 * BindingCommand
 * ============================================================
 * UI → ViewModel 行为绑定（最终态）
 *
 * 规则：
 * - UI 只能绑定 ICommand
 * - UI 不直接 connect 到 ViewModel slot
 */
namespace BindingCommand {

inline void BindCommand(QPushButton* button,
                        ICommand* command)
{
    if (!button || !command) {
        qWarning() << "[BindingCommand] button or command is null";
        return;
    }

    // 1️⃣ 点击 → 执行命令
    QObject::connect(
        button,
        &QPushButton::clicked,
        command,
        &ICommand::Execute
        );

    // 2️⃣ 初始 enabled
    button->setEnabled(command->CanExecute());

    // 3️⃣ CanExecute 改变 → 自动 enable / disable
    QObject::connect(
        command,
        &ICommand::CanExecuteChanged,
        button,
        [button, command]() {
            button->setEnabled(command->CanExecute());
        }
        );
}

} // namespace BindingCommand
