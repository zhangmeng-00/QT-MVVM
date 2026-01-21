#pragma once

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include <QTimer>

class TraceViewer : public QWidget {
    Q_OBJECT
public:
    explicit TraceViewer(const QString& dbPath,
                         QWidget* parent = nullptr);
    ~TraceViewer();

private slots:
    void refresh();

private:
    void initDb(const QString& dbPath);
    void initUi();

private:
    QSqlDatabase      m_db;
    QSqlTableModel*  m_model{nullptr};
    QTableView*      m_view{nullptr};
    QTimer*          m_timer{nullptr};
};
