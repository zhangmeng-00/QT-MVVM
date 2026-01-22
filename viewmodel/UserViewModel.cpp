#include "UserViewModel.h"

UserViewModel::UserViewModel(QObject* parent)
    : BaseViewModel(parent)
{
    // 发布积分 Command（受业务状态控制）
    m_publishScoreCommand = new SimpleCommand(
        [this]() { publishCommand(); },
        [this]() { return m_canPublish; },
        this
        );

    // 重置积分 Command（始终可用）
    m_resetScoreCommand = new SimpleCommand(
        [this]() { resetCommand(); },
        []() { return true; },
        this
        );

    // 业务状态变化 → 刷新 publishScoreCommand
    connect(this, &UserViewModel::CanPublishChanged,
            m_publishScoreCommand, &SimpleCommand::RaiseCanExecuteChanged);

    // ✅ Apply 命令（演示：要求已登录 + 用户名非空）
    m_applyCommand = new SimpleCommand(
        [this]() {
            qDebug() << "[VM] Apply:"
                     << "userName=" << m_userName
                     << "loggedIn=" << m_loggedIn
                     << "modeIndex=" << m_modeIndex
                     << "count=" << m_count;

            // 你想接入 Mediator 的话可以 Publish：
            Publish("user/name", m_userName);
            Publish("user/mode", m_modeIndex);
            Publish("user/count", m_count);
            Publish("user/logged_in", m_loggedIn);
        },
        [this]() {
            return m_loggedIn && !m_userName.trimmed().isEmpty();
        },
        this
        );

    // ✅ Login / Logout（注意：状态改变后要让两个按钮都刷新 enabled）
    m_loginCommand = new SimpleCommand(
        [this]() {
            m_loggedIn = true;
            m_loginStateText = "Logged In";
            emit loginStateTextChanged();
            emit LoginStateChanged(true);

            // 刷新 login/logout/apply 的 enabled
            m_loginCommand->RaiseCanExecuteChanged();
            m_logoutCommand->RaiseCanExecuteChanged();
            m_applyCommand->RaiseCanExecuteChanged();
        },
        [this]() { return !m_loggedIn; },
        this
        );

    m_logoutCommand = new SimpleCommand(
        [this]() {
            m_loggedIn = false;
            m_loginStateText = "Logged Out";
            emit loginStateTextChanged();
            emit LoginStateChanged(false);

            m_loginCommand->RaiseCanExecuteChanged();
            m_logoutCommand->RaiseCanExecuteChanged();
            m_applyCommand->RaiseCanExecuteChanged();
        },
        [this]() { return m_loggedIn; },
        this
        );

    // ✅ lineEdit::textEdited(QString) → 更新 userName（参数：args[0]=QString）
    m_userNameEditedCommand = new SimpleCommand(
        [this](const QVariantList& args) {
            const QString text = args.value(0).toString();
            if (m_userName == text) return;
            m_userName = text;
            emit userNameChanged();
            m_applyCommand->RaiseCanExecuteChanged();
        },
        nullptr,
        this
        );

    // ✅ checkBox::toggled(bool) → 直接设置登录状态（参数：args[0]=bool）
    // （演示用：你也可以不用这个，直接用 login/logout 按钮）
    m_toggleLoginCommand = new SimpleCommand(
        [this](const QVariantList& args) {
            const bool on = args.value(0).toBool();
            if (m_loggedIn == on) return;

            m_loggedIn = on;
            m_loginStateText = on ? "Logged In" : "Logged Out";
            emit loginStateTextChanged();
            emit LoginStateChanged(on);

            m_loginCommand->RaiseCanExecuteChanged();
            m_logoutCommand->RaiseCanExecuteChanged();
            m_applyCommand->RaiseCanExecuteChanged();
        },
        nullptr,
        this
        );

    // ✅ comboBox::currentIndexChanged(int)（参数：args[0]=int）
    m_modeChangedCommand = new SimpleCommand(
        [this](const QVariantList& args) {
            const int idx = args.value(0).toInt();
            m_modeIndex = idx;

            const QString text = (idx >= 0 && idx < m_modes.size()) ? m_modes[idx] : QString("Mode %1").arg(idx);
            if (m_modeText != text) {
                m_modeText = text;
                emit modeTextChanged();
            }
            m_applyCommand->RaiseCanExecuteChanged();
        },
        nullptr,
        this
        );

    // ✅ spinBox::valueChanged(int)（参数：args[0]=int）
    m_countChangedCommand = new SimpleCommand(
        [this](const QVariantList& args) {
            const int v = args.value(0).toInt();
            m_count = v;

            const QString text = QString::number(v);
            if (m_countText != text) {
                m_countText = text;
                emit countTextChanged();
            }
            m_applyCommand->RaiseCanExecuteChanged();
        },
        nullptr,
        this
        );
}

void UserViewModel::publishCommand()
{
    qDebug() << "UserViewModel::publishCommand()";
    int score = QRandomGenerator::global()->bounded(0, 500);
    qDebug() << "[VM] publish score =" << score;
    Publish("user/score", score);
}

void UserViewModel::resetCommand()
{
    qDebug() << "UserViewModel::resetCommand()";
    Publish("user/score", 0);
}

void UserViewModel::ObserveData(const QString& tag, const QVariant& value)
{
    qDebug() << "[VM] ObserveData tag =" << tag << "value =" << value;

    if (tag == "user/score") {
        m_scoreText = QString::number(value.toInt());
        emit scoreTextChanged();
        return;
    }

    if (tag == "user/level") {
        m_levelText = QString("Lv.%1").arg(value.toInt());
        emit levelTextChanged();

        // 这里只是你原来的演示逻辑，我保留
        m_userName = QString("Lv.%1").arg(value.toInt());
        emit userNameChanged();
        m_applyCommand->RaiseCanExecuteChanged();

        if (!m_canPublish) {
            m_canPublish = true;
            emit CanPublishChanged(true);
        }
        return;
    }
}
