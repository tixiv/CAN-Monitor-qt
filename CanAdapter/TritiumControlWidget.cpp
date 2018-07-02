#include "TritiumControlWidget.h"
#include "ui_TritiumControlWidget.h"

TritiumControlWidget::TritiumControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TritiumControlWidget)
{
    ui->setupUi(this);
}

TritiumControlWidget::~TritiumControlWidget()
{
    delete ui;
}



void TritiumControlWidget::displayStatus(QString text)
{
    ui->statusLabel->setText(text);
}

void TritiumControlWidget::on_openCloseButton_clicked()
{
    if(!m_isOpen)
    {
        m_isOpen = true;
        ui->openCloseButton->setText("Close");
        emit(openClicked());
    }
    else
    {
        m_isOpen = false;
        ui->openCloseButton->setText("Open");
        emit(closeClicked());
    }
}
