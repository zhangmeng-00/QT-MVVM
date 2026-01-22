#pragma once

#include <QObject>
#include <QWidget>
#include <QAction>
#include <QVariant>
#include <QVariantList>
#include <QDebug>
#include <functional>

#include "core/command/ICommand.h"

/*
 * BindingCommand
 * ============================================================
 * 目标：把“任何控件/对象的任何 signal 或事件”绑定到 ICommand
 *
 * 1) signal 绑定：
 *    BindCommand(sender, &Sender::signal, command)
 *    - signal 的参数会自动打包成 QVariantList，传给 command->ExecuteArgs(args)
 *
 * 2) enabled 同步：
 *    - 默认同步 sender 的 enabled
 *    - 也可指定 enableTarget（例如：多个控件共用一个命令）
 *
 * 3) 兼容旧写法：
 *    BindCommand(QAbstractButton*, ICommand*) / BindCommand(QAction*, ICommand*)
 */

namespace BindingCommand {

// ------------------------------------------------------------
// 内部工具：通用 setEnabled
// ------------------------------------------------------------
inline void SetEnabled(QObject* target, bool enabled)
{
    if (!target) return;

    // QWidget
    if (auto w = qobject_cast<QWidget*>(target)) {
        w->setEnabled(enabled);
        return;
    }

    // QAction
    if (auto act = qobject_cast<QAction*>(target)) {
        act->setEnabled(enabled);
        return;
    }

    // 任何拥有 enabled 属性的 QObject（Qt 大量类都支持）
    const int idx = target->metaObject()->indexOfProperty("enabled");
    if (idx >= 0) {
        target->setProperty("enabled", enabled);
        return;
    }

    qWarning() << "[BindingCommand] target has no enabled property:" << target;
}

inline QVariantList EmptyArgs() { return {}; }

// ------------------------------------------------------------
// 核心：任意 sender + 任意 signal → ICommand（带参数）
// ------------------------------------------------------------
template <typename Sender, typename Signal>
inline void BindCommand(
    Sender* sender,
    Signal signal,
    ICommand* command,
    QObject* enableTarget = nullptr,
    std::function<QVariantList()> canExecArgsGetter = {})
{
    if (!sender || !command) {
        qWarning() << "[BindingCommand] sender or command is null";
        return;
    }

    QObject* target = enableTarget ? enableTarget : sender;

    // 初始 enabled
    {
        const QVariantList args = canExecArgsGetter ? canExecArgsGetter() : EmptyArgs();
        SetEnabled(target, command->CanExecuteArgs(args));
    }

    // 1) signal → ExecuteArgs(args)
    QObject::connect(
        sender,
        signal,
        command,
        [command, target, canExecArgsGetter](auto&&... a) {
            // 打包参数
            QVariantList args;
            args.reserve(sizeof...(a));
            //(args << QVariant::fromValue(std::forward<decltype(a)>(a)), ...);
            using expander = int[];
            (void)expander{0, ( (args << QVariant::fromValue(std::forward<decltype(a)>(a))), 0 )...};
            command->ExecuteArgs(args);

            // 触发后顺便刷新 enabled（常见场景：执行后状态变化）
            const QVariantList canArgs = canExecArgsGetter ? canExecArgsGetter() : EmptyArgs();
            SetEnabled(target, command->CanExecuteArgs(canArgs));
        }
    );

    // 2) Command 的 CanExecuteChanged → enabled 同步
    QObject::connect(
        command,
        &ICommand::CanExecuteChanged,
        sender,
        [command, target, canExecArgsGetter](bool /*canExecute*/) {
            const QVariantList args = canExecArgsGetter ? canExecArgsGetter() : EmptyArgs();
            SetEnabled(target, command->CanExecuteArgs(args));
        }
    );
}
}
