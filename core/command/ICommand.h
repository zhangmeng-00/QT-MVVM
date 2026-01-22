#pragma once

#include <QObject>
#include <QVariant>
#include <QVariantList>

/*
 * ICommand
 * ============================================================
 * MVVM Command 抽象
 *
 * - Execute()     : 执行动作
 * - CanExecute()  : 是否允许执行
 * - ExecuteArgs() : 带参数执行（用于 valueChanged/textChanged 等）
 * - CanExecuteArgs(): 带参数可执行判断
 * - CanExecuteChanged : 状态变化通知
 */
class ICommand : public QObject {
    Q_OBJECT
public:
    explicit ICommand(QObject* parent = nullptr) : QObject(parent) {}

    virtual void Execute() = 0;
    virtual bool CanExecute() const = 0;

    // ✅ 通用参数版（默认：忽略参数，退化为无参 Execute/CanExecute）
    virtual void ExecuteArgs(const QVariantList& args) {
        Q_UNUSED(args);
        Execute();
    }

    virtual bool CanExecuteArgs(const QVariantList& args) const {
        Q_UNUSED(args);
        return CanExecute();
    }

signals:
    void CanExecuteChanged(bool canExecute);
};
