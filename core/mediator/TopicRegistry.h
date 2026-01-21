#pragma once

#include <QMap>
#include <QString>
#include "TopicMeta.h"

/*
 * TopicRegistry
 * ============================================================
 * Topic 元信息注册表（系统语言定义处）
 *
 * 设计目标：
 * - 所有 Topic 的语义只在这里声明
 * - Mediator / Topic 永远不写业务判断
 * - 修改 / 新增 Topic 不需要改基础设施代码
 *
 * 这是“系统级单例”，不是业务单例
 */
class TopicRegistry {
public:
    /*
     * 获取全局唯一实例
     */
    static TopicRegistry& instance();

    /*
     * 注册一个 Topic 的元信息
     * - 通常在系统初始化阶段调用
     */
    void Register(const QString& tag, const TopicMeta& meta);

    /*
     * 查询 Topic 的完整元信息
     * - 如果未注册，返回默认 Event / 非 Sticky
     */
    TopicMeta MetaOf(const QString& tag) const;

    /*
     * 快捷判断：是否 Sticky
     */
    bool IsSticky(const QString& tag) const;

    /*
     * 快捷判断：是否状态型 Topic
     */
    bool IsState(const QString& tag) const;

private:
    TopicRegistry() = default;

private:
    // tag -> TopicMeta
    QMap<QString, TopicMeta> m_registry;
};
