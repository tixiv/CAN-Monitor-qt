#ifndef COMMANDERDIALOG_H
#define COMMANDERDIALOG_H

#include <QDialog>
#include <QModelIndex>

class ParameterTreeModel;

namespace Ui {
class CommanderDialog;
}

class CommanderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommanderDialog(QWidget *parent = 0);
    ~CommanderDialog();

private slots:
    void onTreeViewContextMenu(const QPoint &point);
    void onButtonContextMenu(const QPoint &point);

    void on_actionAddParameter_triggered();

    void on_actionDelete_triggered();

    void on_actionAddButton_triggered();

    void on_actionDeleteButton_triggered();

    void on_actionEditButton_triggered();

private:
    Ui::CommanderDialog *ui;

    ParameterTreeModel * m_model;

    struct{
        QModelIndex clickedIndex;
    }m_TreeMenuContext;

    struct{
        QWidget * clickedWidget;
    }m_buttonMenuContext;

};

#endif // COMMANDERDIALOG_H
