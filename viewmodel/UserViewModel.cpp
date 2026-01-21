#include "UserViewModel.h"

/*
 * 构造函数
 * -------------------------------------------------
 * - 创建 Command
 * - 绑定 CanExecute
 * - 建立“状态 → Command”通知链路
 */
UserViewModel::UserViewModel(QObject* parent)
    : BaseViewModel(parent)
{
    // 发布积分 Command（受业务状态控制）
    m_publishScoreCommand = new SimpleCommand(
        // Execute
        [this]() {
            publishCommand();   // 🔴 原逻辑，完全复用
        },
        // CanExecute
        [this]() {
            return m_canPublish;
        },
        this
        );

    // 重置积分 Command（始终可用）
    m_resetScoreCommand = new SimpleCommand(
        [this]() {
            resetCommand();
        },
        []() {
            return true;
        },
        this
        );

    // ⭐ 业务状态变化 → Command 刷新 CanExecute
    connect(this, &UserViewModel::CanPublishChanged,
            m_publishScoreCommand,
            &SimpleCommand::RaiseCanExecuteChanged);

    // Login禁用启用
    m_loginCommand = new SimpleCommand(
        [this]() {
            m_loggedIn = true;
            emit LoginStateChanged(true);
        },
        [this]() {
            return !m_loggedIn;
        },
        this
        );

    m_logoutCommand = new SimpleCommand(
        [this]() {
            m_loggedIn = false;
            emit LoginStateChanged(false);
        },
        [this]() {
            return m_loggedIn;
        },
        this
        );

}

/*
 * publishCommand
 * -------------------------------------------------
 * UI 行为：发布随机积分
 * （原有逻辑，不涉及 enable / disable）
 */
void UserViewModel::publishCommand()
{
    qDebug() << "UserViewModel::publishCommand()";

    int score = QRandomGenerator::global()->bounded(0, 500);
    qDebug() << "[VM] publish score =" << score;

    Publish("user/score", score);
}

/*
 * resetCommand
 * -------------------------------------------------
 * UI 行为：重置积分
 */
void UserViewModel::resetCommand()
{
    qDebug() << "UserViewModel::resetCommand()";
    Publish("user/score", 0);
}

/*
 * ObserveData
 * -------------------------------------------------
 * Mediator → ViewModel
 * 这是所有业务状态变化的唯一入口
 */
void UserViewModel::ObserveData(const QString& tag,
                                const QVariant& value)
{
    qDebug() << "[VM] ObserveData tag =" << tag
             << "value =" << value;

    // ===== 积分更新 =====
    if (tag == "user/score") {
        m_scoreText = QString::number(value.toInt());
        emit scoreTextChanged();
        return;
    }

    // ===== 等级更新（并解锁发布权限）=====
    if (tag == "user/level") {
        m_levelText = QString("Lv.%1").arg(value.toInt());
        emit levelTextChanged();

        m_userName = QString("Lv.%1").arg(value.toInt());
        emit userNameChanged();

        // ⭐⭐⭐ 关键：业务状态变化
        if (!m_canPublish) {
            m_canPublish = true;
            emit CanPublishChanged(true);
        }
        return;
    }
}
