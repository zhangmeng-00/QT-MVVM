#pragma once

/*
 * TopicMeta
 * ============================================================
 * Topic 的“元信息定义”
 *
 * 作用：
 * - 描述一个 Topic 的语义属性
 * - Mediator / Topic 本身不关心业务
 * - 所有语义统一在这里声明
 */

/*
 * TopicKind
 * ------------------------------------------------------------
 * Topic 的基本类型
 */
enum class TopicKind {
    Event,   // 事件型 Topic（发生过）
    State    // 状态型 Topic（当前是）
};

/*
 * TopicMeta
 * ------------------------------------------------------------
 * 一个 Topic 的完整元数据描述
 */
struct TopicMeta {
    TopicKind kind;   // Topic 类型：事件 / 状态
    bool sticky;      // 是否为 Sticky（新订阅者是否回放）
    int  version;     // 版本号（用于未来演进）
};
