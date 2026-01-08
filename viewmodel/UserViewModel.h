#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QRandomGenerator>   // ✅ 必须加
#include <qdebug.h>

#include "viewmodel/BaseViewModel.h"

class UserViewModel : public BaseViewModel {
    Q_OBJECT
    Q_PROPERTY(QString scoreText READ scoreText NOTIFY scoreTextChanged)
    Q_PROPERTY(QString levelText READ levelText NOTIFY levelTextChanged)
    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)

public:
    explicit UserViewModel(QObject* parent = nullptr)
        : BaseViewModel(parent)
    {}

    QString scoreText() const { return m_scoreText; }
    QString levelText() const { return m_levelText; }
    QString userName() const { return m_userName; }

signals:
    void scoreTextChanged();
    void levelTextChanged();
    void userNameChanged();
public slots:
    /*
     * publishCommand
     * --------------------------------------------------------
     * UI 命令：发布随机积分
     */
    void publishCommand()
    {
        qDebug() << "UserViewModel::publishCommand()";
        int score = QRandomGenerator::global()->bounded(0, 500);
        qDebug() << "[VM] publish score =" << score;
        Publish("user/score", score);
    }


    /*
     * resetCommand
     * --------------------------------------------------------
     * UI 命令：重置积分
     */
    void resetCommand()
    {
        Publish("user/score", 0);
    }

protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override
    {
        qDebug() << "[VM] ObserveData tag =" << tag << "value =" << value;
        if (tag == "user/score") {
            m_scoreText = QString::number(value.toInt());
            emit scoreTextChanged();
        }
        else if (tag == "user/level") {
            m_levelText = QString("Lv.%1").arg(value.toInt());
            emit levelTextChanged();
            m_userName = QString("Lv.%1").arg(value.toInt());
            emit userNameChanged();
        }
    }

private:
    QString m_scoreText;
    QString m_levelText;
    QString m_userName;
};
