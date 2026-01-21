#pragma once

#include <QObject>

/*
 * ICommand
 * ============================================================
 * MVVM Command 抽象
 *
 * - Execute()     : 执行动作
 * - CanExecute()  : 是否允许执行
 * - CanExecuteChanged : 状态变化通知
 */
class ICommand : public QObject {
    Q_OBJECT
public:
    explicit ICommand(QObject* parent = nullptr) : QObject(parent) {}

    virtual void Execute() = 0;
    virtual bool CanExecute() const = 0;

signals:
    void CanExecuteChanged(bool canExecute);
};
