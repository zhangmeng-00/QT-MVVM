#pragma once

#include <QObject>
#include <QLabel>
#include <QMetaObject>
#include <QMetaProperty>
#include <QVariant>
#include <QDebug>

/*
 * BindingHelper
 * ============================================================
 * Qt5 Property Binding 中转对象
 *
 * 作用：
 * - 接收 ViewModel 的 NOTIFY 信号
 * - 从 ViewModel 读取属性
 * - 更新 QLabel
 *
 * 注意：
 * - 必须是 QObject + slot
 * - 不能用 lambda（Qt5 反射限制）
 */
class BindingHelper : public QObject {
    Q_OBJECT
public:
    BindingHelper(QLabel* label,
                  QObject* viewModel,
                  const char* propertyName,
                  QObject* parent = nullptr)
        : QObject(parent)
        , m_label(label)
        , m_viewModel(viewModel)
        , m_propertyName(propertyName)
    {}

public slots:
    void onPropertyChanged()
    {
        if (!m_label || !m_viewModel)
            return;

        QVariant v = m_viewModel->property(m_propertyName);
        m_label->setText(v.toString());
    }

private:
    QLabel*  m_label;
    QObject* m_viewModel;
    const char* m_propertyName;
};

/*
 * Binding
 * ============================================================
 * ViewModel → View 的属性绑定
 */
namespace Binding {

/*
 * BindLabel
 * ------------------------------------------------------------
 * QLabel ← ViewModel.Q_PROPERTY
 *
 * @param label        QLabel*
 * @param viewModel    QObject*（ViewModel）
 * @param propertyName Q_PROPERTY 名称（字符串）
 */
inline void BindLabel(QLabel* label,
                      QObject* viewModel,
                      const char* propertyName)
{
    if (!label || !viewModel) {
        qWarning() << "[Binding] label or viewModel is null";
        return;
    }

    // 1️⃣ 初始同步一次
    label->setText(viewModel->property(propertyName).toString());

    // 2️⃣ 查找属性
    const QMetaObject* mo = viewModel->metaObject();
    int propIndex = mo->indexOfProperty(propertyName);
    if (propIndex < 0) {
        qWarning() << "[Binding] Property not found:" << propertyName;
        return;
    }

    QMetaProperty prop = mo->property(propIndex);
    if (!prop.hasNotifySignal()) {
        qWarning() << "[Binding] Property has no NOTIFY:" << propertyName;
        return;
    }

    // 3️⃣ 获取 NOTIFY 信号索引（Qt5 稳定方式）
    int notifyIndex = prop.notifySignalIndex();
    if (notifyIndex < 0) {
        qWarning() << "[Binding] notifySignalIndex invalid:" << propertyName;
        return;
    }

    QMetaMethod signalMethod = mo->method(notifyIndex);

    // Qt SIGNAL 字符串格式："2signalName()"
    QByteArray signalStr = QByteArray("2") + signalMethod.methodSignature();

    // 4️⃣ 创建 helper（生命周期跟随 label）
    auto* helper = new BindingHelper(
        label,
        viewModel,
        propertyName,
        label
        );

    // 5️⃣ 用 Qt5 唯一可靠的方式连接
    bool ok = QObject::connect(
        viewModel,
        signalStr.constData(),
        helper,
        SLOT(onPropertyChanged())
        );

    if (!ok) {
        qWarning() << "[Binding] connect failed for property:" << propertyName;
    }
}

} // namespace Binding
