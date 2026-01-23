#include "UserViewModel.h"
#include "core/policy/AlwaysPolicy.h"

// 构造函数：创建统一命令 + 注册所有 action + （可选）订阅状态
UserViewModel::UserViewModel(QObject* parent)
    : BaseViewModel(parent)
{
    // 1) 注册 tag -> handler（和 MainWindow 里发出的 tag 一一对应）
    RegisterActions();

    // 2) 创建统一命令：Command 本身不包含业务，只回调 OnUICommand/CanUICommand
    m_uiCommand = new DispatcherCommand(
        [this](const QVariantList& args) { OnUICommand(args); },
        [this](const QVariantList& args) { return CanUICommand(args); },
        this
        );


}

void UserViewModel::RegisterActions()
{
    // =========================================================
    // 1) user/publish_score  <--- btnPublishScore
    // args = ["user/publish_score", true]
    // =========================================================
    m_actions.insert("user/publish_score", Action{
                                               [this](const QVariantList&) {
                                                   // 业务逻辑：产生随机分数，然后发布
                                                   int score = QRandomGenerator::global()->bounded(0, 500);
                                                   qDebug() << "[UserVM] publish random score =" << score;

                                                   Publish("user/score", score);
                                               },
                                               [this](const QVariantList&) -> bool {
                                                   // 例：必须先收到 user/level 才允许发布
                                                   return m_canPublish;
                                               }
                                           });

    // =========================================================
    // 2) user/login <--- btnLogin
    // args = ["user/login", true]
    // =========================================================
    m_actions.insert("user/login", Action{
                                       [this](const QVariantList&) {
                                           if (m_loggedIn) return;

                                           m_loggedIn = true;

                                           // 更新 UI 文本（对应 labelLoginState <- loginStateText）
                                           m_loginStateText = "LoggedIn";
                                           emit loginStateTextChanged();

                                           // 发布状态（给其它模块/Model 用）
                                           Publish("user/logged_in", true);
                                           RefreshCommandStates(); // 刷新 login/logout/publish 按钮 enable
                                       },
                                       [this](const QVariantList&) -> bool {
                                           return !m_loggedIn;
                                       }
                                   });

    // =========================================================
    // 3) user/logout <--- btnLogout
    // args = ["user/logout", true]
    // =========================================================
    m_actions.insert("user/logout", Action{
                                        [this](const QVariantList&) {
                                            if (!m_loggedIn) return;

                                            m_loggedIn = false;

                                            m_loginStateText = "LoggedOut";
                                            emit loginStateTextChanged();

                                            Publish("user/logged_in", false);

                                            RefreshCommandStates();
                                        },
                                        [this](const QVariantList&) -> bool {
                                            return m_loggedIn;
                                        }
                                    });

    // =========================================================
    // 4) user/logged_in <--- checkBoxLoggedIn.toggled(bool)
    // args = ["user/logged_in", bool]
    // =========================================================
    m_actions.insert("user/logged_in", Action{
                                           [this](const QVariantList& args) {
                                               bool on = args.value(1).toBool();

                                               m_loggedIn = on;
                                               m_loginStateText = on ? "LoggedIn" : "LoggedOut";
                                               emit loginStateTextChanged();

                                               Publish("user/logged_in", on);
                                               RefreshCommandStates();
                                           },
                                           nullptr
                                       });

    // =========================================================
    // 5) user/mode <--- comboBoxMode.currentIndexChanged(int)
    // args = ["user/mode", int]
    // =========================================================
    m_actions.insert("user/mode", Action{
                                      [this](const QVariantList& args) {
                                          int idx = args.value(1).toInt();

                                          // 更新 UI 文本（对应 labelModeValue <- modeText）
                                          m_modeText = QString::number(idx);
                                          emit modeTextChanged();

                                          Publish("user/mode", idx);
                                      },
                                      nullptr
                                  });

    // =========================================================
    // 6) user/count <--- spinBoxCount.valueChanged(int)
    // args = ["user/count", int]
    // =========================================================
    m_actions.insert("user/count", Action{
                                       [this](const QVariantList& args) {
                                           int v = args.value(1).toInt();

                                           // 更新 UI 文本（对应 labelCountValue <- countText）
                                           m_countText = QString::number(v);
                                           emit countTextChanged();

                                           Publish("user/count", v);
                                       },
                                       nullptr
                                   });

    // =========================================================
    // 7) user/name <--- lineEdit.textEdited(QString)
    // args = ["user/name", QString]
    // =========================================================
    m_actions.insert("user/name", Action{
                                      [this](const QVariantList& args) {
                                          QString name = args.value(1).toString();

                                          if (name == m_userName) return;
                                          m_userName = name;
                                          emit userNameChanged();

                                          Publish("user/name", name);
                                      },
                                      nullptr
                                  });
}

void UserViewModel::OnUICommand(const QVariantList& args)
{
    if (args.isEmpty()) {
        qWarning() << "[UserVM] OnUICommand: empty args";
        return;
    }

    const QString tag = args[0].toString();
    if (tag.isEmpty()) {
        qWarning() << "[UserVM] OnUICommand: empty tag";
        return;
    }

    auto it = m_actions.find(tag);
    if (it == m_actions.end() || !it->exec) {
        qWarning() << "[UserVM] OnUICommand: no handler, tag =" << tag << "args =" << args;
        return;
    }

    it->exec(args);
}

bool UserViewModel::CanUICommand(const QVariantList& args) const
{
    if (args.isEmpty()) return true;

    const QString tag = args[0].toString();
    if (tag.isEmpty()) return false;

    auto it = m_actions.find(tag);
    if (it != m_actions.end() && it->can) {
        return it->can(args);
    }
    return true;
}

void UserViewModel::RefreshCommandStates()
{
    if (m_uiCommand) {
        m_uiCommand->RaiseCanExecuteChanged();
    }
}

void UserViewModel::SetupSubscriptions()
{
    Subscribe("user/score", std::make_shared<AlwaysPolicy>());
    Subscribe("user/level", std::make_shared<AlwaysPolicy>());
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
            RefreshCommandStates();
        }
        return;
    }
}
