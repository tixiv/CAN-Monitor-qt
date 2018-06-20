#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "CanTree/CanTreeModel.h"
#include "CanAdapter/CanAdapter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onCustomContextMenu(const QPoint &point);

    void on_actionAdd_Group_triggered();
    void tickTimerTimeout();
    void onTransmit(can_message_t cmsg);

    void on_actionSave_Tree_triggered();

    void on_actionLoad_Tree_triggered();

    void on_actionDelete_Node_triggered();

    void on_canAdapterComboBox_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    CanTreeModel *m_model = 0;

    struct{
        QModelIndex index;
    }m_contextMenuContext;

    CanAdapter * m_canAdapter = 0;
    QWidget * m_canAdpterControlWidget = 0;

    QTimer m_tickTimer;

    QList<CanAdapter *>m_canAdapters;

    void changeCanAdpapter(CanAdapter * ca);
};

#endif // MAINWINDOW_H
