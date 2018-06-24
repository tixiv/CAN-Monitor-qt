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
    void onCustomContextMenu(const QPoint &point);

    void on_actionAddParameter_triggered();

    void on_actionDelete_triggered();

private:
    Ui::CommanderDialog *ui;

    ParameterTreeModel * m_model;

    struct{
        QModelIndex clickedIndex;
    }m_contextMenuContext;

};

#endif // COMMANDERDIALOG_H
