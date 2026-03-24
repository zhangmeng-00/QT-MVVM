#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QRandomGenerator>
#include <QDebug>

#include "BaseViewModel.h"

/*
 * UserViewModel
 * ============================================================
 * - UI 逻辑层：Q_PROPERTY + 事件虚函数
 * - 使用 BaseViewModel 的事件虚函数处理 UI 事件
 * - Publish 永远只写在 ViewModel
 */
class UserViewModel : public BaseViewModel {
    Q_OBJECT
    Q_PROPERTY(QString scoreText READ scoreText NOTIFY scoreTextChanged)
    Q_PROPERTY(QString levelText READ levelText NOTIFY levelTextChanged)
    Q_PROPERTY(QString userName  READ userName  NOTIFY userNameChanged)
    Q_PROPERTY(QString loginStateText READ loginStateText NOTIFY loginStateTextChanged)
    Q_PROPERTY(QString modeText       READ modeText       NOTIFY modeTextChanged)
    Q_PROPERTY(QString countText      READ countText      NOTIFY countTextChanged)
    // 新增：按钮启用状态属性
    Q_PROPERTY(bool canPublish READ canPublish NOTIFY canPublishChanged)
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY loggedInChanged)

public:
    explicit UserViewModel(QObject* parent = nullptr);

    QString scoreText() const { return m_scoreText; }
    QString levelText() const { return m_levelText; }
    QString userName()  const { return m_userName; }
    QString loginStateText() const { return m_loginStateText; }
    QString modeText() const { return m_modeText; }
    QString countText() const { return m_countText; }

    bool canPublish() const { return m_canPublish; }
    bool loggedIn() const { return m_loggedIn; }

signals:
    void scoreTextChanged();
    void levelTextChanged();
    void userNameChanged();
    void loginStateTextChanged();
    void modeTextChanged();
    void countTextChanged();
    void canPublishChanged();
    void loggedInChanged();

public:
    // 事件虚函数重写
    Q_INVOKABLE void onClicked(const QString& senderId) override;
    Q_INVOKABLE void onToggled(bool checked, const QString& senderId) override;
    Q_INVOKABLE void onCurrentIndexChanged(int index, const QString& senderId) override;
    Q_INVOKABLE void onValueChanged(int value, const QString& senderId) override;
    Q_INVOKABLE void onTextChanged(const QString& text, const QString& senderId) override;
    Q_INVOKABLE void onTextEdited(const QString& text, const QString& senderId) override;

protected:
    // Mediator → ViewModel 的唯一入口
    void ObserveData(const QString& tag, const QVariant& value) override;

private:
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
    bool m_canPublish = true;
    bool m_loggedIn = false;
};
