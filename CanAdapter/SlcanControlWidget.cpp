#include "SlcanControlWidget.h"
#include "ui_SlcanControlWidget.h"
#include <QSettings>


void populateCanBaudComboBox(QComboBox * cb){
    cb->addItem("10");
    cb->addItem("20");
    cb->addItem("50");
    cb->addItem("100");
    cb->addItem("125");
    cb->addItem("250");
    cb->addItem("500");
    cb->addItem("800");
    cb->addItem("1000");

    cb->setCurrentText(QSettings().value("CanAdapterSlcan/canBaud", "125").toString());
}

void populateModeComboBox(QComboBox * cb){
    cb->addItem("Normal");
    cb->addItem("Listen only");
    cb->addItem("Loopback");

    cb->setCurrentIndex(QSettings().value("CanAdapterSlcan/openMode", 0).toInt());
}

SlcanControlWidget::SlcanControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlcanControlWidget)
{
    ui->setupUi(this);

    ui->comPortComboBox->setCurrentText(QSettings().value("CanAdapterSlcan/serialPort").toString());

    populateCanBaudComboBox(ui->canBaudComboBox);
    populateModeComboBox(ui->modeComboBox);

}

SlcanControlWidget::~SlcanControlWidget()
{
    delete ui;
}

void SlcanControlWidget::openOperationEnded(bool success){
    ui->openButton->setEnabled(true);
    if(success){
        m_open = true;
        ui->openButton->setText("Close");
    }
    else
    {
        ui->comPortComboBox->setEnabled(true);
        ui->canBaudComboBox->setEnabled(true);
        ui->modeComboBox->setEnabled(true);
    }
}

void SlcanControlWidget::on_openButton_clicked()
{
    if(m_open){
        m_open = false;
        ui->openButton->setText("Open");
        emit closeClicked();
        ui->comPortComboBox->setEnabled(true);
        ui->canBaudComboBox->setEnabled(true);
        ui->modeComboBox->setEnabled(true);
    } else {
        QString comportName = ui->comPortComboBox->currentText();
        QString canKBaud = ui->canBaudComboBox->currentText();
        int mode = ui->modeComboBox->currentIndex();

        QSettings().setValue("CanAdapterSlcan/serialPort", comportName);
        QSettings().setValue("CanAdapterSlcan/canBaud", canKBaud);
        QSettings().setValue("CanAdapterSlcan/openMode", mode);

        ui->openButton->setEnabled(false);
        CanAdapterLawicel::OpenMode om = (CanAdapterLawicel::OpenMode) mode;
        emit openClicked(comportName, om, canKBaud.toInt() * 1000);
        ui->comPortComboBox->setEnabled(false);
        ui->canBaudComboBox->setEnabled(false);
        ui->modeComboBox->setEnabled(false);
    }
}
