#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "treemodel.h"

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
    void on_pushButton_clicked();
    void clicked(const QModelIndex &index);

    void on_actionAdd_Group_triggered();

private:
    Ui::MainWindow *ui;
    TreeModel *model;

    struct{
        QModelIndex index;
    }m_contextMenuContext;
};

#endif // MAINWINDOW_H
