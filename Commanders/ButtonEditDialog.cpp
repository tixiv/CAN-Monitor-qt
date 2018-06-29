#include "ButtonEditDialog.h"
#include "ui_ButtonEditDialog.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QRegularExpression>

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
    ui->CommandEdit->setText(QString().number(dialogData.command, 16));
    ui->SubCommandEdit->setText(QString().number(dialogData.subCommand, 16));
    ui->ValueEdit->setText(QString().number(dialogData.value, 16));
    ui->isSaveButtonCheckBox->setChecked(dialogData.isSaveButton);
    ui->saveCommandRangeEdit->setText(dialogData.saveRange);
    return QDialog::exec();
}

void ButtonEditDialog::on_isSaveButtonCheckBox_toggled(bool checked)
{
    ui->saveCommandRangeEdit->setEnabled(checked);
    ui->saveCommandRangeLabel->setEnabled(checked);
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
        if(!checkIntLineEdit(ui->CommandEdit, &dialogData.command, 16)) return;
        if(!checkIntLineEdit(ui->SubCommandEdit, &dialogData.subCommand, 16)) return;
        if(!checkIntLineEdit(ui->ValueEdit, &dialogData.value, 10)) return;
        dialogData.isSaveButton = ui->isSaveButtonCheckBox->isChecked();
        dialogData.saveRange = ui->saveCommandRangeEdit->text();
        if(dialogData.isSaveButton)
        {
            if(dialogData.getSaveCommands().size() == 0)
            {
                auto le = ui->saveCommandRangeEdit;
                le->setFocus();
                le->selectAll();
                return;
            }
        }

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
    writer.writeAttribute("command", QString().number((uint)command, 16));
    writer.writeAttribute("subCommand", QString().number((uint)subCommand, 16));
    writer.writeAttribute("value", QString().number(value));
    writer.writeAttribute("isSaveButton", QString().number(isSaveButton));
    writer.writeAttribute("saveRange", saveRange);
}

void CommanderButtonData::readFromXml(QXmlStreamReader &reader)
{
    text = reader.attributes().value("text").toString();
    command = reader.attributes().value("command").toString().toInt(0,16);
    subCommand = reader.attributes().value("subCommand").toString().toInt(0,16);
    value = reader.attributes().value("value").toString().toInt();
    isSaveButton = reader.attributes().value("isSaveButton").toString().toInt();
    saveRange = reader.attributes().value("saveRange").toString();
    reader.readElementText();
}

QByteArray CommanderButtonData::getSaveCommands()
{
    auto strs = saveRange.split(',');
    QByteArray ba;

    bool invalid=false;
    foreach (auto str, strs) {
        QRegularExpression re("([0-9a-fA-F]+)[-|:]?([0-9a-fA-F]*)");
        QRegularExpressionMatch match = re.match(str);
        if(match.hasMatch())
        {
            int begin = match.captured(1).toInt(0,16);
            int end = begin;
            if(match.captured(2) != "")
                end = match.captured(2).toInt(0,16);
            if(end < begin)
                invalid = true;
            else
            {
                for(int i=begin; i<=end; i+=2)
                {
                    ba.append((char)i);
                }
            }
        }
        else
            invalid = true;
    }

    if(!invalid)
        return ba;
    else
        return QByteArray();

}
