#include "UserViewModel.h"
#include "AlwaysPolicy.h"
#include "Logger.h"

// 构造函数
UserViewModel::UserViewModel(QObject* parent)
    : BaseViewModel(parent)
{
}

// ========== 事件虚函数实现 ==========

void UserViewModel::onClicked(const QString& senderId)
{
    qDebug() << "[UserVM] onClicked:" << senderId;

    if (senderId == "btnPublishScore") {
        int score = QRandomGenerator::global()->bounded(0, 500);
        Publish("user/score", score);
        LOG_INFO("UserViewModel", QString("Published random score %1").arg(score));
    } else if (senderId == "btnLogin") {
        m_loggedIn = true;
        m_loginStateText = "LoggedIn";
        emit loginStateTextChanged();
        emit loggedInChanged();
        Publish("user/logged_in", true);
    } else if (senderId == "btnLogout") {
        m_loggedIn = false;
        m_loginStateText = "LoggedOut";
        emit loginStateTextChanged();
        emit loggedInChanged();
        Publish("user/logged_in", false);
    }
}

void UserViewModel::onToggled(bool checked, const QString& senderId)
{
    qDebug() << "[UserVM] onToggled:" << senderId << checked;

    if (senderId == "checkBoxLoggedIn") {
        m_loggedIn = checked;
        m_loginStateText = checked ? "LoggedIn" : "LoggedOut";
        emit loginStateTextChanged();
        emit loggedInChanged();
        Publish("user/logged_in", checked);
    }
}

void UserViewModel::onCurrentIndexChanged(int index, const QString& senderId)
{
    qDebug() << "[UserVM] onCurrentIndexChanged:" << senderId << index;

    if (senderId == "comboBoxMode") {
        m_modeText = QString::number(index);
        emit modeTextChanged();
        Publish("user/mode", index);
    }
}

void UserViewModel::onValueChanged(int value, const QString& senderId)
{
    qDebug() << "[UserVM] onValueChanged:" << senderId << value;

    if (senderId == "spinBoxCount") {
        m_countText = QString::number(value);
        emit countTextChanged();
        Publish("user/count", value);
    }
}

void UserViewModel::onTextChanged(const QString& text, const QString& senderId)
{
    qDebug() << "[UserVM] onTextChanged:" << senderId << text;

    if (senderId == "lineEdit") {
        if (text == m_userName) return;
        m_userName = text;
        emit userNameChanged();
        Publish("user/name", text);
    }
}

void UserViewModel::onTextEdited(const QString& text, const QString& senderId)
{
    // 与 onTextChanged 相同逻辑
    onTextChanged(text, senderId);
}

void UserViewModel::SetupSubscriptions()
{
    // 使用带QVariant的订阅，明确指定数据类型
    Subscribe("user/score", QVariant(0), std::make_shared<AlwaysPolicy>());
    Subscribe("user/level", QVariant(0), std::make_shared<AlwaysPolicy>());

    // 新增：UI事件发布的tag
    Subscribe("user/publish_score", QVariant(false), std::make_shared<AlwaysPolicy>());
    Subscribe("user/login", QVariant(false), std::make_shared<AlwaysPolicy>());
    Subscribe("user/logout", QVariant(false), std::make_shared<AlwaysPolicy>());
    Subscribe("user/logged_in", QVariant(false), std::make_shared<AlwaysPolicy>());
    Subscribe("user/mode", QVariant(0), std::make_shared<AlwaysPolicy>());
    Subscribe("user/count", QVariant(0), std::make_shared<AlwaysPolicy>());
    Subscribe("user/name", QString(), std::make_shared<AlwaysPolicy>());
}

// ObserveData：用于接收来自 Model / 其它模块发布的状态，更新显示用 Q_PROPERTY
void UserViewModel::ObserveData(const QString& tag, const QVariant& value)
{
    if (tag == "user/score") {
        m_scoreText = QString::number(value.toInt());
        emit scoreTextChanged();
        return;
    }

    if (tag == "user/level") {
        const int lv = value.toInt();
        m_levelText = QString("Lv.%1").arg(lv);
        emit levelTextChanged();

        // 例：收到 level 后才允许 publish_score
        if (!m_canPublish) {
            m_canPublish = true;
            emit canPublishChanged();
        }
        return;
    }

    // 处理UI事件发布的tag
    if (tag == "user/publish_score") {
        // 收到按钮点击，随机生成积分
        int score = QRandomGenerator::global()->bounded(100);
        Publish("user/score", QVariant(score));
        return;
    }

    if (tag == "user/login") {
        m_loggedIn = true;
        m_loginStateText = "LoggedIn";
        emit loginStateTextChanged();
        emit loggedInChanged();
        return;
    }

    if (tag == "user/logout") {
        m_loggedIn = false;
        m_loginStateText = "LoggedOut";
        emit loginStateTextChanged();
        emit loggedInChanged();
        return;
    }

    if (tag == "user/logged_in") {
        m_loggedIn = value.toBool();
        m_loginStateText = m_loggedIn ? "LoggedIn" : "LoggedOut";
        emit loginStateTextChanged();
        emit loggedInChanged();
        return;
    }

    if (tag == "user/mode") {
        m_modeText = QString::number(value.toInt());
        emit modeTextChanged();
        return;
    }

    if (tag == "user/count") {
        m_countText = QString::number(value.toInt());
        emit countTextChanged();
        return;
    }

    if (tag == "user/name") {
        m_userName = value.toString();
        emit userNameChanged();
        return;
    }
}
