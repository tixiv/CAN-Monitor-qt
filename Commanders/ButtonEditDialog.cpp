#include "ButtonEditDialog.h"
#include "ui_ButtonEditDialog.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

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

void CommanderButtonData::writeToXml(QXmlStreamWriter &writer) const
{
    writer.writeAttribute("text", text);
    writer.writeAttribute("canId", QString().number((uint)canId, 16));
    writer.writeAttribute("command", QString().number((uint)command, 16));
    writer.writeAttribute("subCommand", QString().number((uint)subCommand, 16));
    writer.writeAttribute("value", QString().number(value));
    writer.writeAttribute("saveRange", saveRange);
}

void CommanderButtonData::readFromXml(QXmlStreamReader &reader)
{
    text = reader.attributes().value("text").toString();
    canId = reader.attributes().value("canId").toString().toInt(0,16);
    command = reader.attributes().value("command").toString().toInt(0,16);
    subCommand = reader.attributes().value("subCommand").toString().toInt(0,16);
    value = reader.attributes().value("value").toString().toInt();
    saveRange = reader.attributes().value("saveRange").toString();
}
