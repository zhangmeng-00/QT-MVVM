#pragma once

#include "core/command/ICommand.h"
#include <functional>

/*
 * DispatcherCommand
 * ============================================================
 * 目标：让 ViewModel 只维护“一个 ICommand”
 *
 * 设计原则：
 * - Command 不包含任何业务逻辑
 * - Command 不调用 Publish / Mediator（保持纯粹）
 * - Command 的职责只有一个：把参数转发给 ViewModel（或任何 handler）
 *
 * 用法：
 *   m_uiCommand = new DispatcherCommand(
 *       [this](const QVariantList& args){ OnUICommand(args); },
 *       [this](const QVariantList& args){ return CanUICommand(args); },
 *       this
 *   );
 */
class DispatcherCommand : public ICommand {
    Q_OBJECT
public:
    using ExecuteArgsFn = std::function<void(const QVariantList&)>;
    using CanExecuteArgsFn = std::function<bool(const QVariantList&)>;

    explicit DispatcherCommand(
        ExecuteArgsFn execArgs,
        CanExecuteArgsFn canExecArgs = {},
        QObject* parent = nullptr)
        : ICommand(parent),
        m_execArgs(std::move(execArgs)),
        m_canExecArgs(std::move(canExecArgs))
    {}

    // 无参接口：退化为参数接口（传空参数）
    void Execute() override {
        ExecuteArgs({});
    }

    // 默认无参可执行：如果提供了参数版判断，则用空参数判断；否则恒 true
    bool CanExecute() const override {
        if (m_canExecArgs) return m_canExecArgs({});
        return true;
    }

    // 带参执行：直接转发给 handler（通常就是 ViewModel::OnUICommand）
    void ExecuteArgs(const QVariantList& args) override {
        if (!CanExecuteArgs(args)) return;
        if (m_execArgs) m_execArgs(args);
    }

    // 带参可执行：用于根据 tag 做不同按钮的 enable/disable
    bool CanExecuteArgs(const QVariantList& args) const override {
        if (m_canExecArgs) return m_canExecArgs(args);
        return true;
    }

    /*
     * RaiseCanExecuteChanged
     * --------------------------------------------------------
     * 通知 UI “请重新评估 CanExecute”
     *
     * 说明：
     * - ICommand::CanExecuteChanged(bool) 的 bool 在你当前 BindingCommand 实现里并不会直接使用，
     *   BindingCommand 会自己再次调用 command->CanExecuteArgs(...) 去计算 enabled。
     * - 因此这里 emit 一个信号即可触发刷新。
     */
    void RaiseCanExecuteChanged() {
        emit CanExecuteChanged(true);
    }

private:
    ExecuteArgsFn m_execArgs;
    CanExecuteArgsFn m_canExecArgs;
};
