#pragma once

#include "ICommand.h"
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

    SimpleCommand(
        ExecuteFn exec,
        CanExecuteFn canExec,
        QObject* parent = nullptr
        )
        : ICommand(parent),
        m_execute(exec),
        m_canExecute(canExec)
    {}

    void Execute() override {
        if (!CanExecute())
            return;
        m_execute();
    }

    bool CanExecute() const override {
        return m_canExecute ? m_canExecute() : true;
    }

    void RaiseCanExecuteChanged() {
        emit CanExecuteChanged(CanExecute());
    }

private:
    ExecuteFn m_execute;
    CanExecuteFn m_canExecute;
};
