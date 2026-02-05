#pragma once

#include <QWidget>

namespace Ui {
class LogListView;
}

class LogListViewModel;

class LogListView : public QWidget {
    Q_OBJECT
public:
    explicit LogListView(QWidget* parent = nullptr);
    ~LogListView();

private:
    Ui::LogListView* m_ui = nullptr;
    LogListViewModel* m_viewModel = nullptr;
};
