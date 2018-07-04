#ifndef TRACEWINDOW_H
#define TRACEWINDOW_H

#include <QMainWindow>
#include <QSet>
#include "lib-slcan/can_message.h"


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

private slots:
    void messageReceived(can_message_t cmsg);
    void on_recordPushButton_clicked(bool checked);

    void on_clearButton_clicked();

private:
    Ui::TraceWindow *ui;
    CanHandle * m_canHandle = 0;
    CanTableModel * m_model = 0;

    bool m_recording;

    QSet<int> m_ids;
};

#endif // TRACEWINDOW_H
