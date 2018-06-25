#include "ButtonEditDialog.h"
#include "ui_ButtonEditDialog.h"

ButtonEditDialog::ButtonEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ButtonEditDialog)
{
    ui->setupUi(this);
}

ButtonEditDialog::~ButtonEditDialog()
{
    delete ui;
}

int ButtonEditDialog::exec(){
    ui->textEdit->setText(dialogData.text);
    ui->CanIdEdit->setText(QString().number(dialogData.canId, 16));
    ui->CommandEdit->setText(QString().number(dialogData.command, 16));
    ui->SubCommandEdit->setText(QString().number(dialogData.subCommand, 16));
    ui->ValueEdit->setText(QString().number(dialogData.value, 16));
    return QDialog::exec();
}

template <typename T> static bool checkIntLineEdit(QLineEdit * le, T * result, int radix = 10)
{
    bool ok;
    int r = le->text().toInt(&ok,radix);
    if(!ok){
        le->setFocus();
        le->selectAll();
    } else {
        *result = r;
    }
    return ok;
}

void ButtonEditDialog::done(int r)
{
    if(QDialog::Accepted == r)  // ok was pressed
    {
        dialogData.text = ui->textEdit->text();
        if(!checkIntLineEdit(ui->CanIdEdit, &dialogData.canId, 16)) return;
        if(!checkIntLineEdit(ui->CommandEdit, &dialogData.command, 16)) return;
        if(!checkIntLineEdit(ui->SubCommandEdit, &dialogData.subCommand, 16)) return;
        if(!checkIntLineEdit(ui->ValueEdit, &dialogData.value, 10)) return;

        QDialog::done(r);
        return;
    }
    else    // cancel, close or exc was pressed
    {
        QDialog::done(r);
        return;
    }
}
