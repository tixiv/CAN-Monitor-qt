#include "CommanderDialog.h"
#include "ui_CommanderDialog.h"
#include "ParameterTree/ParameterTreeModel.h"

CommanderDialog::CommanderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommanderDialog)
{
    ui->setupUi(this);

    m_model = new ParameterTreeModel();
    ui->treeView->setModel(m_model);

}

CommanderDialog::~CommanderDialog()
{
    delete ui;
}
