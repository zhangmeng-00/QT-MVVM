#include "LogListView.h"
#include "ui_LogListView.h"

#include "app/AppContext.h"
#include "core/binding/Binding.h"
#include "core/viewmodel/LogListViewModel.h"

#include <QHeaderView>

LogListView::LogListView(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui::LogListView)
    , m_viewModel(new LogListViewModel(this))
{
    m_ui->setupUi(this);

    m_ui->tableViewLogs->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_ui->tableViewLogs->setSelectionMode(QAbstractItemView::SingleSelection);
    m_ui->tableViewLogs->setAlternatingRowColors(true);
    m_ui->tableViewLogs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_ui->tableViewLogs->horizontalHeader()->setStretchLastSection(true);
    m_ui->tableViewLogs->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    AppContext::instance().ConnectObserve(m_viewModel);

    Binding::BindProperty(m_ui->tableViewLogs, "model", m_viewModel, "tableModel");
}

LogListView::~LogListView()
{
    delete m_ui;
}
