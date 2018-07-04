#ifndef TRACEWINDOW_H
#define TRACEWINDOW_H

#include <QMainWindow>


class CanHub;
class CanHandle;
class CanTableModel;

namespace Ui {
class TraceWindow;
}

class TraceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TraceWindow(QWidget *parent, CanHub &canHub);
    ~TraceWindow();

private:
    Ui::TraceWindow *ui;
    CanHandle * canHandle = 0;
    CanTableModel * m_model = 0;
};

#endif // TRACEWINDOW_H
