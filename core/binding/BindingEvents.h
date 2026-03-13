#pragma once

#include <QObject>
#include <QVariant>
#include <QVariantList>
#include <QString>
#include <functional>
#include <type_traits>

/*
 * BindingEvents
 * ============================================================
 * 目标：把 "任意控件的任意signal" 直接绑定到ViewModel并Publish到Mediator
 *
 * 特性：
 * 1) 控件signal -> ViewModel::Publish(tag, payload)
 * 2) 可选 payloadMapper：把signal参数映射成你想要的QVariant
 * 3) 不需要ICommand中间层，直接Publish
 *
 * 使用方式：
 *   BindingEvents::BindEventToPublish(ui->btn, &QPushButton::clicked,
 *       viewModel, "user/action", ConstPayload(true));
 *
 *   BindingEvents::BindEventToPublish(ui->lineEdit, &QLineEdit::textChanged,
 *       viewModel, "user/name", FromString());
 */

namespace BindingEvents {

// ------------------------------------------------------------
// 核心：任意sender的任意signal -> ViewModel的slot
// ------------------------------------------------------------
template <typename Sender, typename Signal, typename Receiver, typename Slot>
inline void BindEvent(Sender* sender, Signal signal, Receiver* receiver, Slot slot)
{
    if (!sender || !receiver) {
        qWarning() << "[BindingEvents] sender or receiver is null";
        return;
    }
    QObject::connect(sender, signal, receiver, slot);
}

// ------------------------------------------------------------
// 带参数映射的版本：signal -> ViewModel::Publish(tag, payload)
// payloadMapper: signalArgs -> QVariant
// ------------------------------------------------------------
template <typename Sender, typename Signal, typename Receiver>
inline void BindEventToPublish(
    Sender* sender,
    Signal signal,
    Receiver* viewModel,
    const QString& tag,
    std::function<QVariant(const QVariantList&)> payloadMapper = {})
{
    if (!sender || !viewModel) {
        qWarning() << "[BindingEvents] sender or viewModel is null";
        return;
    }

    // 使用 lambda 捕获 signal 参数并转换为 QVariantList
    QObject::connect(
        sender,
        signal,
        viewModel,
        [viewModel, tag, payloadMapper](auto&&... args) {
            // 打包 signal 参数 -> signalArgs
            QVariantList signalArgs;
            signalArgs.reserve(sizeof...(args));

            // C++14 折叠表达式展开参数
            using expander = int[];
            (void)expander{0,
                ((signalArgs << QVariant::fromValue(std::forward<decltype(args)>(args))), 0)...};

            // 生成 payload
            QVariant payload;
            if (payloadMapper) {
                payload = payloadMapper(signalArgs);
            } else if (!signalArgs.isEmpty()) {
                payload = signalArgs.value(0);
            }

            // 直接调用 ViewModel 的 Publish 方法
            viewModel->Publish(tag, payload);
        }
    );
}

// ------------------------------------------------------------
// 简化版本：无参数映射，直接传递 signal 的第一个参数
// ------------------------------------------------------------
template <typename Sender, typename Signal, typename Receiver>
inline void BindEventToPublishSimple(Sender* sender, Signal signal, Receiver* viewModel, const QString& tag)
{
    BindEventToPublish(sender, signal, viewModel, tag, {});
}

} // namespace BindingEvents
