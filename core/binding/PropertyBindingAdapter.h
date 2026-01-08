#pragma once

#include <QObject>
#include <QLabel>
#include <QMetaProperty>

/*
 * PropertyBindingAdapter
 * ============================================================
 * Qt5 用的中转适配器：
 * - 接收 notify signal
 * - 从 ViewModel 读取 property
 * - 写入 QLabel
 *
 * 这是 Qt5 下唯一“合法 + 稳定”的方案
 */
class PropertyBindingAdapter : public QObject {
    Q_OBJECT
public:
    PropertyBindingAdapter(QObject* viewModel,
                           const QMetaProperty& property,
                           QLabel* label)
        : QObject(label)          // 让 adapter 跟随 label 生命周期
        , m_viewModel(viewModel)
        , m_property(property)
        , m_label(label)
    {
        // 初始化 UI
        update();
    }

public slots:
    void update()
    {
        if (!m_viewModel || !m_label)
            return;

        QVariant v = m_property.read(m_viewModel);
        m_label->setText(v.toString());
    }

private:
    QObject*       m_viewModel;
    QMetaProperty  m_property;
    QLabel*        m_label;
};
