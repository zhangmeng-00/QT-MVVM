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
 * Bind
 * ------------------------------------------------------------
 * 任意 QObject 的 signal → ViewModel 的 slot
 */
template<typename Sender,
         typename Signal,
         typename Receiver,
         typename Slot>
inline void Bind(Sender* sender,
                 Signal signal,
                 Receiver* receiver,
                 Slot slot)
{
    if (!sender || !receiver) {
        qWarning() << "[BindingCommand] sender or receiver is null";
        return;
    }

    bool ok = QObject::connect(sender, signal, receiver, slot);

    if (!ok) {
        qWarning() << "[BindingCommand] connect failed";
    }
}

} // namespace BindingCommand
