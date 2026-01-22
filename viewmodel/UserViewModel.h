#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
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
 */
class UserViewModel : public BaseViewModel {
    Q_OBJECT
    Q_PROPERTY(QString scoreText READ scoreText NOTIFY scoreTextChanged)
    Q_PROPERTY(QString levelText READ levelText NOTIFY levelTextChanged)
    Q_PROPERTY(QString userName  READ userName  NOTIFY userNameChanged)

    // ✅ 新增：给“其它控件事件”演示用
    Q_PROPERTY(QString loginStateText READ loginStateText NOTIFY loginStateTextChanged)
    Q_PROPERTY(QString modeText READ modeText NOTIFY modeTextChanged)
    Q_PROPERTY(QString countText READ countText NOTIFY countTextChanged)

public:
    explicit UserViewModel(QObject* parent = nullptr);

    QString scoreText() const { return m_scoreText; }
    QString levelText() const { return m_levelText; }
    QString userName()  const { return m_userName; }

    QString loginStateText() const { return m_loginStateText; }
    QString modeText() const { return m_modeText; }
    QString countText() const { return m_countText; }

    // ✅ UI 唯一入口：Command
    ICommand* publishScoreCommand() const { return m_publishScoreCommand; }
    ICommand* resetScoreCommand()   const { return m_resetScoreCommand; }
    ICommand* loginCommand()  const { return m_loginCommand; }
    ICommand* logoutCommand() const { return m_logoutCommand; }

    // ✅ 新增：参数命令（供 BindCommand(sender, &signal, cmd) 使用）
    ICommand* userNameEditedCommand() const { return m_userNameEditedCommand; }
    ICommand* toggleLoginCommand() const { return m_toggleLoginCommand; }
    ICommand* modeChangedCommand() const { return m_modeChangedCommand; }
    ICommand* countChangedCommand() const { return m_countChangedCommand; }
    ICommand* applyCommand() const { return m_applyCommand; }

signals:
    void scoreTextChanged();
    void levelTextChanged();
    void userNameChanged();

    void loginStateTextChanged();
    void modeTextChanged();
    void countTextChanged();

    void LoginStateChanged(bool loggedIn);
    void CanPublishChanged(bool canPublish);

public slots:
    void publishCommand();
    void resetCommand();
    void SetupSubscriptions() override;

protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override;

private:
    // ===== UI 显示数据 =====
    QString m_scoreText;
    QString m_levelText;
    QString m_userName;

    QString m_loginStateText = "Logged Out";
    QString m_modeText = "Mode A";
    QString m_countText = "10";

    // ===== 业务状态 =====
    bool m_canPublish = false;
    bool m_loggedIn = false;

    // mode / count 存储（可用于 Apply）
    QStringList m_modes { "Mode A", "Mode B", "Mode C" };
    int m_modeIndex = 0;
    int m_count = 10;

    // ===== Command =====
    SimpleCommand* m_publishScoreCommand = nullptr;
    SimpleCommand* m_resetScoreCommand   = nullptr;

    SimpleCommand* m_loginCommand  = nullptr;
    SimpleCommand* m_logoutCommand = nullptr;

    // ✅ 新增参数命令
    SimpleCommand* m_userNameEditedCommand = nullptr;
    SimpleCommand* m_toggleLoginCommand    = nullptr;
    SimpleCommand* m_modeChangedCommand    = nullptr;
    SimpleCommand* m_countChangedCommand   = nullptr;
    SimpleCommand* m_applyCommand          = nullptr;
};
