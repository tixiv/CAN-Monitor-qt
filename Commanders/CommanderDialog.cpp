#include "CommanderDialog.h"
#include "ui_CommanderDialog.h"

CommanderDialog::CommanderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommanderDialog)
{
    ui->setupUi(this);
}

CommanderDialog::~CommanderDialog()
{
    delete ui;
}
