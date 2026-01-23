#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QRandomGenerator>
#include <QDebug>
#include <QHash>
#include <functional>

#include "viewmodel/BaseViewModel.h"
#include "core/command/ICommand.h"
#include "core/command/DispatcherCommand.h"

/*
 * UserViewModel
 * ============================================================
 * - UI 逻辑层：Q_PROPERTY + 一个统一 ICommand
 * - 所有 UI 事件（按钮/编辑/选择变化）都通过 uiCommand() 进入 OnUICommand()
 * - Publish 永远只写在 ViewModel（符合你的要求）
 */
class UserViewModel : public BaseViewModel {
    Q_OBJECT
    Q_PROPERTY(QString scoreText READ scoreText NOTIFY scoreTextChanged)
    Q_PROPERTY(QString levelText READ levelText NOTIFY levelTextChanged)
    Q_PROPERTY(QString userName  READ userName  NOTIFY userNameChanged)
    Q_PROPERTY(QString loginStateText READ loginStateText NOTIFY loginStateTextChanged)
    Q_PROPERTY(QString modeText       READ modeText       NOTIFY modeTextChanged)
    Q_PROPERTY(QString countText      READ countText      NOTIFY countTextChanged)

public:
    explicit UserViewModel(QObject* parent = nullptr);

    QString scoreText() const { return m_scoreText; }
    QString levelText() const { return m_levelText; }
    QString userName()  const { return m_userName; }
    QString loginStateText() const { return m_loginStateText; }
    QString modeText() const { return m_modeText; }
    QString countText() const { return m_countText; }


    // ✅ UI 唯一入口：一个命令解决所有控件事件
    ICommand* uiCommand() const { return m_uiCommand; }

signals:
    void scoreTextChanged();
    void levelTextChanged();
    void userNameChanged();
    void loginStateTextChanged();
    void modeTextChanged();
    void countTextChanged();
    // 可选：如果你 UI 需要显示登录状态/联动其它控件
    void LoginStateChanged(bool loggedIn);

protected:
    // Mediator → ViewModel 的唯一入口
    void ObserveData(const QString& tag, const QVariant& value) override;

private:
    // 统一入口：由 DispatcherCommand 调用
    void OnUICommand(const QVariantList& args);

    // 统一可执行判断：由 DispatcherCommand 调用（用于 enable/disable）
    bool CanUICommand(const QVariantList& args) const;

    // 注册所有 UI action（tag → handler）
    void RegisterActions();

    // 当内部状态变化时，通知 UI 刷新 enabled
    void RefreshCommandStates();
    void SetupSubscriptions() override;

private:
    // ===== UI 显示数据 =====
    QString m_scoreText;
    QString m_levelText;
    QString m_userName;
    QString m_loginStateText = "LoggedOut";
    QString m_modeText = "0";
    QString m_countText = "0";

    // ===== 业务状态（用于 CanExecute）=====
    bool m_canPublish = false;
    bool m_loggedIn = false;

    // ===== 统一命令 =====
    DispatcherCommand* m_uiCommand = nullptr;

    // ===== Action 表：tag → 执行/可执行 =====
    struct Action {
        std::function<void(const QVariantList& args)> exec;
        std::function<bool(const QVariantList& args)> can;
    };
    QHash<QString, Action> m_actions;
};
