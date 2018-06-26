#include "CommanderPropertiesDialog.h"
#include "ui_CommanderPropertiesDialog.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>


CommanderPropertiesDialog::CommanderPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommanderPropertiesDialog)
{
    ui->setupUi(this);
}

CommanderPropertiesDialog::~CommanderPropertiesDialog()
{
    delete ui;
}

int CommanderPropertiesDialog::exec(){
    ui->canIdWriteEdit->setText(QString().number(dialogData.canIdWrite, 16));
    ui->canIdReadEdit->setText(QString().number(dialogData.canIdRead, 16));
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

void CommanderPropertiesDialog::done(int r)
{
    if(QDialog::Accepted == r)  // ok was pressed
    {
        if(!checkIntLineEdit(ui->canIdWriteEdit, &dialogData.canIdWrite, 16)) return;
        if(!checkIntLineEdit(ui->canIdReadEdit, &dialogData.canIdRead, 16)) return;

        QDialog::done(r);
        return;
    }
    else    // cancel, close or exc was pressed
    {
        QDialog::done(r);
        return;
    }
}

void CommanderProperties::writeToXml(QXmlStreamWriter &writer) const
{
    writer.writeAttribute("canIdWrite", QString().number((uint)canIdWrite, 16));
    writer.writeAttribute("canIdRead", QString().number((uint)canIdRead, 16));
}

void CommanderProperties::readFromXml(QXmlStreamReader &reader)
{
    canIdWrite = reader.attributes().value("canIdWrite").toString().toInt(0,16);
    canIdRead = reader.attributes().value("canIdRead").toString().toInt(0,16);
}
