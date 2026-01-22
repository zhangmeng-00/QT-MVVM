#pragma once

#include "ICommand.h"
#include <QVariantList>
#include <functional>

/*
 * SimpleCommand
 * ============================================================
 * 基于 lambda / function 的命令实现
 */
class SimpleCommand : public ICommand {
    Q_OBJECT
public:
    using ExecuteFn = std::function<void()>;
    using CanExecuteFn = std::function<bool()>;

    // ✅ 新增：带参数版本（任意信号参数会打包成 QVariantList 传进来）
    using ExecuteArgsFn = std::function<void(const QVariantList&)>;
    using CanExecuteArgsFn = std::function<bool(const QVariantList&)>;

    // ✅ 构造：无参命令（兼容旧代码）
    SimpleCommand(
        ExecuteFn exec,
        CanExecuteFn canExec = {},
        QObject* parent = nullptr
        )
        : ICommand(parent),
          m_execute(std::move(exec)),
          m_canExecute(std::move(canExec))
    {}

    // ✅ 构造：带参数命令（用于 valueChanged/textChanged 等）
    SimpleCommand(
        ExecuteArgsFn execArgs,
        CanExecuteArgsFn canExecArgs = {},
        QObject* parent = nullptr
        )
        : ICommand(parent),
          m_executeArgs(std::move(execArgs)),
          m_canExecuteArgs(std::move(canExecArgs))
    {}

    void Execute() override {
        if (!CanExecute())
            return;

        // 优先无参实现；没有的话退化成参数版（传空参数）
        if (m_execute) {
            m_execute();
            return;
        }
        if (m_executeArgs) {
            m_executeArgs({});
        }
    }

    void ExecuteArgs(const QVariantList& args) override {
        if (!CanExecuteArgs(args))
            return;

        // 优先参数实现；没有的话退化成无参实现
        if (m_executeArgs) {
            m_executeArgs(args);
            return;
        }
        if (m_execute) {
            m_execute();
        }
    }

    bool CanExecute() const override {
        if (m_canExecute)
            return m_canExecute();
        if (m_canExecuteArgs)
            return m_canExecuteArgs({});
        return true;
    }

    bool CanExecuteArgs(const QVariantList& args) const override {
        if (m_canExecuteArgs)
            return m_canExecuteArgs(args);
        if (m_canExecute)
            return m_canExecute();
        return true;
    }

    void RaiseCanExecuteChanged() {
        emit CanExecuteChanged(CanExecute());
    }

private:
    ExecuteFn m_execute;
    CanExecuteFn m_canExecute;

    ExecuteArgsFn m_executeArgs;
    CanExecuteArgsFn m_canExecuteArgs;
};
