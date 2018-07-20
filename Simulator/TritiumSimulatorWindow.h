#ifndef TRITIUMSIMULATORWINDOW_H
#define TRITIUMSIMULATORWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "CanHub/CanHub.h"

namespace Ui {
class TritiumSimulatorWindow;
}

class TritiumSimulatorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TritiumSimulatorWindow(QWidget *parent, CanHub &canHub);
    ~TritiumSimulatorWindow();

private slots:
    void messageReceived(can_message_t cmsg);
    void tickTimerTimeout();
private:
    Ui::TritiumSimulatorWindow *ui;
    CanHandle * m_canHandle;

    int m_baseId = 0x400;
    QTimer m_tickTimer;
};

#endif // TRITIUMSIMULATORWINDOW_H
