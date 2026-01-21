#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QRandomGenerator>
#include <QDebug>

#include "viewmodel/BaseViewModel.h"
#include "core/command/ICommand.h"
#include "core/command/SimpleCommand.h"

/*
 * UserViewModel
 * -------------------------------------------------
 * - 业务状态源
 * - Command 的拥有者
 * - 不直接控制 UI enable / disable
 */
class UserViewModel : public BaseViewModel {
    Q_OBJECT
    Q_PROPERTY(QString scoreText READ scoreText NOTIFY scoreTextChanged)
    Q_PROPERTY(QString levelText READ levelText NOTIFY levelTextChanged)
    Q_PROPERTY(QString userName  READ userName  NOTIFY userNameChanged)

public:
    explicit UserViewModel(QObject* parent = nullptr);

    QString scoreText() const { return m_scoreText; }
    QString levelText() const { return m_levelText; }
    QString userName()  const { return m_userName; }

    // ✅ UI 唯一入口：Command
    ICommand* publishScoreCommand() const { return m_publishScoreCommand; }
    ICommand* resetScoreCommand()   const { return m_resetScoreCommand; }
    ICommand* loginCommand()  const { return m_loginCommand; }
    ICommand* logoutCommand() const { return m_logoutCommand; }

signals:
    void scoreTextChanged();
    void levelTextChanged();
    void userNameChanged();
    void LoginStateChanged(bool loggedIn);
    // ⭐ 业务状态变化 → Command
    void CanPublishChanged(bool canPublish);

public slots:
    // 🔴 原业务逻辑（保持不变）
    void publishCommand();
    void resetCommand();

protected:
    // 🔴 Mediator → ViewModel 的唯一入口
    void ObserveData(const QString& tag,
                     const QVariant& value) override;

private:
    // ===== UI 显示数据 =====
    QString m_scoreText;
    QString m_levelText;
    QString m_userName;

    // ===== 业务状态 =====
    bool m_canPublish = false;

    // ===== Command =====
    SimpleCommand* m_publishScoreCommand = nullptr;
    SimpleCommand* m_resetScoreCommand   = nullptr;
    bool m_loggedIn = false;

    SimpleCommand* m_loginCommand  = nullptr;
    SimpleCommand* m_logoutCommand = nullptr;
};
