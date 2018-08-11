#include "ChinaControlWidget.h"
#include "ui_ChinaControlWidget.h"
#include <QSettings>


const QString ChinaControlWidget::m_settingsName = "CanAdapterChina";

void ChinaControlWidget::populateCanBaudComboBox(QComboBox * cb){
    cb->addItem("5");
    cb->addItem("10");
    cb->addItem("20");
    cb->addItem("50");
    cb->addItem("100");
    cb->addItem("125");
    cb->addItem("200");
    cb->addItem("250");
    cb->addItem("400");
    cb->addItem("500");
    cb->addItem("800");
    cb->addItem("1000");

    cb->setCurrentText(QSettings().value(m_settingsName + "/canBaud", "125").toString());
}


void ChinaControlWidget::populateModeComboBox(QComboBox * cb){
    cb->addItem("Normal");
    cb->addItem("Listen only");
    cb->addItem("Loopback");

    cb->setCurrentIndex(QSettings().value(m_settingsName + "/openMode", 0).toInt());
}

ChinaControlWidget::ChinaControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChinaControlWidget)
{
    ui->setupUi(this);

    ui->comPortComboBox->setCurrentText(QSettings().value(m_settingsName + "/serialPort").toString());

    populateCanBaudComboBox(ui->canBaudComboBox);
    populateModeComboBox(ui->modeComboBox);

}

ChinaControlWidget::~ChinaControlWidget()
{
    delete ui;
}

void ChinaControlWidget::openOperationEnded(bool success){
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

void ChinaControlWidget::on_openButton_clicked()
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

        QSettings().setValue(m_settingsName + "/serialPort", comportName);
        QSettings().setValue(m_settingsName + "/canBaud", canKBaud);
        QSettings().setValue(m_settingsName + "/openMode", mode);

        ui->openButton->setEnabled(false);
        CanAdapterChina::OpenMode om = (CanAdapterChina::OpenMode) mode;
        emit openClicked(comportName, om, canKBaud.toInt() * 1000);
        ui->comPortComboBox->setEnabled(false);
        ui->canBaudComboBox->setEnabled(false);
        ui->modeComboBox->setEnabled(false);

    }
}
