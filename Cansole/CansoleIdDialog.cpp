#include "CansoleIdDialog.h"
#include "ui_CansoleIdDialog.h"
#include "util/DialogUtil.h"

CansoleIdDialog::CansoleIdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CansoleIdDialog)
{
    ui->setupUi(this);
}

CansoleIdDialog::~CansoleIdDialog()
{
    delete ui;
}

int CansoleIdDialog::exec()
{
    ui->idEdit->setText(QString().number(cansoleId, 16));
    return QDialog::exec();
}

void CansoleIdDialog::done(int r)
{
    if(QDialog::Accepted == r)  // ok was pressed
    {
        if(!DialogUtil::checkIntLineEdit(ui->idEdit, &cansoleId, 16)) return;

        QDialog::done(r);
        return;
    }
    else    // cancel, close or exc was pressed
    {
        QDialog::done(r);
        return;
    }
}
