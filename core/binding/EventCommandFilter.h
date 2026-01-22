#pragma once

#include <QObject>
#include <QEvent>
#include <QWidget>
#include <QAction>
#include <QVariant>
#include <QVariantList>
#include <QDebug>
#include <functional>

#include "core/command/ICommand.h"

/*
 * EventCommandFilter
 * ============================================================
 * 用 eventFilter 绑定“任何 QEvent”到 ICommand（并可把事件信息投影为参数）。
 *
 * 用法示例：
 *   using namespace BindingEvent;
 *   BindEvent(ui->widget, QEvent::MouseMove, cmd,
 *       [](QEvent* e){
 *           auto* me = static_cast<QMouseEvent*>(e);
 *           return QVariantList{ me->pos() };
 *       });
 */

namespace BindingEvent {

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

    qWarning() << "[EventCommandFilter] target has no enabled property:" << target;
}

class EventCommandFilter final : public QObject {
public:
    using Projector = std::function<QVariantList(QEvent*)>; // QEvent* -> args
    using CanExecArgsGetter = std::function<QVariantList()>; // 用于刷新 enabled

    EventCommandFilter(QEvent::Type type,
                       ICommand* command,
                       QObject* enableTarget,
                       Projector projector,
                       CanExecArgsGetter canGetter)
        : QObject(enableTarget),
          m_type(type),
          m_command(command),
          m_enableTarget(enableTarget),
          m_projector(std::move(projector)),
          m_canGetter(std::move(canGetter))
    {}

protected:
    bool eventFilter(QObject* watched, QEvent* event) override
    {
        if (!m_command || !event)
            return QObject::eventFilter(watched, event);

        if (event->type() != m_type)
            return QObject::eventFilter(watched, event);

        QVariantList args;
        if (m_projector) {
            args = m_projector(event);
        }

        m_command->ExecuteArgs(args);

        // 事件触发后刷新 enabled
        const QVariantList canArgs = m_canGetter ? m_canGetter() : QVariantList{};
        SetEnabled(m_enableTarget ? m_enableTarget : watched, m_command->CanExecuteArgs(canArgs));

        return QObject::eventFilter(watched, event);
    }

private:
    QEvent::Type m_type;
    ICommand* m_command = nullptr;
    QObject* m_enableTarget = nullptr;
    Projector m_projector;
    CanExecArgsGetter m_canGetter;
};

// ------------------------------------------------------------
// API：安装 eventFilter 并绑定 CanExecuteChanged
// ------------------------------------------------------------
inline void BindEvent(
    QObject* watched,
    QEvent::Type type,
    ICommand* command,
    EventCommandFilter::Projector projector = {},
    QObject* enableTarget = nullptr,
    EventCommandFilter::CanExecArgsGetter canExecArgsGetter = {})
{
    if (!watched || !command) {
        qWarning() << "[BindingEvent] watched or command is null";
        return;
    }

    QObject* target = enableTarget ? enableTarget : watched;

    // 初始 enabled
    {
        const QVariantList args = canExecArgsGetter ? canExecArgsGetter() : QVariantList{};
        SetEnabled(target, command->CanExecuteArgs(args));
    }

    // filter 以 target 为 parent，生命周期跟随 target
    // 注意：canExecArgsGetter 既用于 filter 内刷新，也用于 CanExecuteChanged 刷新
    auto* filter = new EventCommandFilter(type, command, target, std::move(projector), canExecArgsGetter);
    watched->installEventFilter(filter);

    // CanExecuteChanged → enabled 同步
    QObject::connect(
        command,
        &ICommand::CanExecuteChanged,
        watched,
        [command, target, canExecArgsGetter](bool /*canExecute*/) {
            const QVariantList args = canExecArgsGetter ? canExecArgsGetter() : QVariantList{};
            SetEnabled(target, command->CanExecuteArgs(args));
        }
    );
}

} // namespace BindingEvent
