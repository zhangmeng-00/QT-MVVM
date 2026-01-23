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
 * 目标：把 “任意 sender 的任意 signal” 绑定到 ICommand
 *
 * 特性：
 * 1) signal 参数会自动打包成 QVariantList（signalArgs）
 * 2) 可选 argMapper：把 signalArgs 映射成你真正想传给 command 的 args
 *    - 例如你统一约定 args = [tag, payload]
 * 3) 可选 canExecArgsGetter：用于计算 enabled（CanExecuteArgs）时提供参数
 *    - 如果不提供，默认用“最终 args”去计算 enabled
 * 4) 可选 enableTarget：指定哪个对象需要 setEnabled（默认 sender）
 *
 * 注意：
 * - 使用了“泛型 lambda 参数 (auto&&...)”，要求 C++14+（Qt 5.15 + MSVC2019 没问题）
 */

namespace BindingCommand {

// ------------------------------------------------------------
// 内部工具：通用 setEnabled（QWidget/QAction/拥有 enabled 属性的 QObject）
// ------------------------------------------------------------
inline void SetEnabled(QObject* target, bool enabled)
{
    if (!target) return;

    if (auto w = qobject_cast<QWidget*>(target)) {
        w->setEnabled(enabled);
        return;
    }
    if (auto act = qobject_cast<QAction*>(target)) {
        act->setEnabled(enabled);
        return;
    }

    const int idx = target->metaObject()->indexOfProperty("enabled");
    if (idx >= 0) {
        target->setProperty("enabled", enabled);
        return;
    }

    qWarning() << "[BindingCommand] target has no enabled property:" << target;
}

inline QVariantList EmptyArgs() { return {}; }

// ------------------------------------------------------------
// 核心：任意 sender + 任意 signal -> ICommand
// ------------------------------------------------------------
template <typename Sender, typename Signal>
inline void BindCommand(
    Sender* sender,
    Signal signal,
    ICommand* command,
    QObject* enableTarget = nullptr,
    std::function<QVariantList()> canExecArgsGetter = {},
    std::function<QVariantList(const QVariantList& signalArgs)> argMapper = {})
{
    if (!sender || !command) {
        qWarning() << "[BindingCommand] sender or command is null";
        return;
    }

    QObject* target = enableTarget ? enableTarget : sender;

    // ---------- 初始 enabled ----------
    {
        // 如果提供了 canExecArgsGetter，就用它；否则用空参数（或者也可用最终 args，这里用空参数更安全）
        const QVariantList canArgs = canExecArgsGetter ? canExecArgsGetter() : EmptyArgs();
        SetEnabled(target, command->CanExecuteArgs(canArgs));
    }

    // ---------- 1) signal -> ExecuteArgs(finalArgs) ----------
    QObject::connect(
        sender,
        signal,
        command, // context：当 command 析构时自动断开连接
        [command, target, canExecArgsGetter, argMapper](auto&&... a) {

            // 1.1 打包 signal 参数 -> signalArgs
            QVariantList signalArgs;
            signalArgs.reserve(sizeof...(a));
            using expander = int[];
            (void)expander{0, (((signalArgs << QVariant::fromValue(std::forward<decltype(a)>(a)))), 0)...};

            // 1.2 生成最终 args（默认=signalArgs，若提供 argMapper 则用映射后的）
            const QVariantList finalArgs = argMapper ? argMapper(signalArgs) : signalArgs;

            // 1.3 执行命令
            command->ExecuteArgs(finalArgs);

            // 1.4 执行后刷新 enabled：
            //     - 若提供 canExecArgsGetter：用它
            //     - 否则默认用 finalArgs 来评估（适合你的 [tag,payload] 模式）
            const QVariantList canArgs = canExecArgsGetter ? canExecArgsGetter() : finalArgs;
            SetEnabled(target, command->CanExecuteArgs(canArgs));
        }
        );

    // ---------- 2) Command CanExecuteChanged -> 刷新 enabled ----------
    QObject::connect(
        command,
        &ICommand::CanExecuteChanged,
        sender,  // context：sender 析构时自动断开
        [command, target, canExecArgsGetter](bool /*unused*/) {
            const QVariantList canArgs = canExecArgsGetter ? canExecArgsGetter() : EmptyArgs();
            SetEnabled(target, command->CanExecuteArgs(canArgs));
        }
        );
}

} // namespace BindingCommand
