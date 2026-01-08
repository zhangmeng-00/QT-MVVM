#include "TraceViewer.h"

#include <QVBoxLayout>
#include <QSqlError>
#include <QHeaderView>   // ✅ 必须
#include <QDebug>


TraceViewer::TraceViewer(const QString& dbPath, QWidget* parent)
    : QWidget(parent)
{
    initDb(dbPath);
    initUi();

    m_model = new QSqlTableModel(this, m_db);
    m_model->setTable("trace");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->select();

    m_view->setModel(m_model);
    m_view->setSortingEnabled(true);

    // 定时刷新（500ms）
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout,
            this, &TraceViewer::refresh);
    m_timer->start(500);
}

TraceViewer::~TraceViewer()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void TraceViewer::initDb(const QString& dbPath)
{
    QString connName =
        QString("trace_viewer_%1")
            .arg(reinterpret_cast<quintptr>(this));

    m_db = QSqlDatabase::addDatabase("QSQLITE", connName);
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qDebug() << "[TraceViewer] DB open failed:"
                 << m_db.lastError().text();
    }
}

void TraceViewer::initUi()
{
    m_view = new QTableView(this);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setAlternatingRowColors(true);
    m_view->horizontalHeader()->setStretchLastSection(true);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_view);
    setLayout(layout);

    setWindowTitle("Trace Viewer");
    resize(900, 500);
}

void TraceViewer::refresh()
{
    if (!m_model)
        return;

    m_model->select();
}
