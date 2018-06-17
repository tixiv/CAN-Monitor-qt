#include "SlcanControlWidget.h"
#include "ui_SlcanControlWidget.h"


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

    cb->setCurrentIndex(4);
}

void populateModeComboBox(QComboBox * cb){
    cb->addItem("Normal");
    cb->addItem("Listen only");
    cb->addItem("Loopback");
}

SlcanControlWidget::SlcanControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlcanControlWidget)
{
    ui->setupUi(this);
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
}

void SlcanControlWidget::on_openButton_clicked()
{
    if(m_open){
        m_open = false;
        ui->openButton->setText("Open");
        emit closeClicked();
    } else {
        ui->openButton->setEnabled(false);
        CanAdapterLawicel::OpenMode om = (CanAdapterLawicel::OpenMode) ui->modeComboBox->currentIndex();
        emit openClicked(ui->comPortComboBox->currentText(), om, ui->canBaudComboBox->currentText().toInt() * 1000);
    }
}
