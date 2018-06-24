#ifndef COMMANDERDIALOG_H
#define COMMANDERDIALOG_H

#include <QDialog>

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

private:
    Ui::CommanderDialog *ui;

    ParameterTreeModel * m_model;
};

#endif // COMMANDERDIALOG_H
