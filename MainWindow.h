#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QModelIndex>
#include "CanAdapter/CanAdapter.h"
#include "CanHub/CanHub.h"

namespace Ui {
class MainWindow;
}

class QSortFilterProxyModel;
class CanTreeModel;
class CanAdapter;
class CommanderDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    CanTreeModel *m_model = 0;
    QSortFilterProxyModel * m_proxyModel = 0;
    const bool useProxyModel = true;

    struct{
        QModelIndex clickedIndex;
    }m_contextMenuContext;

    int m_lastSortIndex = -1;
    enum SortEnum{
        sortNone = 0,
        sortAscending = 1,
        sortDescending = 2,
    } m_nextSortMode = sortAscending;

    CanAdapter * m_canAdapter = 0;
    QWidget * m_canAdpterControlWidget = 0;

    CanHub m_canHub;
    CanHandle * m_guiCanHandle;

    QList<CommanderDialog *> m_openCommanders;

    void changeCanAdpapter(CanAdapter * ca);

    void loadTree(QString &filename);
    bool saveTreeInteractive();

    void openCommander(QString name);
    void populateCommanders();

signals:
    void canReceived(can_message_t cmsg);

public slots:
    void canTransmit(can_message_t cmsg);

private slots:
    void onCustomContextMenu(const QPoint &point);

    void on_actionAdd_Group_triggered();
    void headerSectionClicked(int index);

    void on_actionSave_Tree_triggered();
    void on_actionLoad_Tree_triggered();
    void on_canAdapterComboBox_currentTextChanged(const QString &arg1);
    void on_actionDeleteTreeNodes_triggered();

    void on_actionAbout_triggered();

    void on_actionSort_Items_Live_triggered(bool checked);

    void on_actionEnableID_triggered(bool checked);
    void on_actionEnableDLC_triggered(bool checked);
    void on_actionEnableCount_triggered(bool checked);
    void on_actionEnablePeriod_triggered(bool checked);
    void on_actionEnableRaw_Data_triggered(bool checked);
    void on_actionEnableDecoded_Data_triggered(bool checked);
    void on_actionEnableFormat_triggered(bool checked);

    void on_actionSetCommanderDirectory_triggered();

    void on_actionNewCommander_triggered();

    void actionCommanderTriggered();
    void commanderWindowClosed(QObject *foo);
};

#endif // MAINWINDOW_H
