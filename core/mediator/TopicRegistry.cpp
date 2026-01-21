#include "TopicRegistry.h"

/*
 * 获取单例实例
 */
TopicRegistry& TopicRegistry::instance()
{
    static TopicRegistry inst;
    return inst;
}

/*
 * 注册 Topic 元信息
 */
void TopicRegistry::Register(const QString& tag, const TopicMeta& meta)
{
    m_registry.insert(tag, meta);
}

/*
 * 查询 TopicMeta
 * ------------------------------------------------------------
 * 默认策略：
 * - 未注册的 Topic 视为 Event + 非 Sticky
 * - 这是最安全的降级方式
 */
TopicMeta TopicRegistry::MetaOf(const QString& tag) const
{
    return m_registry.value(
        tag,
        TopicMeta{ TopicKind::Event, false, 1 }
        );
}

/*
 * 是否 Sticky
 */
bool TopicRegistry::IsSticky(const QString& tag) const
{
    return MetaOf(tag).sticky;
}

/*
 * 是否 State Topic
 */
bool TopicRegistry::IsState(const QString& tag) const
{
    return MetaOf(tag).kind == TopicKind::State;
}
