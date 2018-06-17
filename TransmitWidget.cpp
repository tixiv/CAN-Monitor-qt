#include "TransmitWidget.h"
#include "ui_TransmitWidget.h"
#include "WidgetUtils/HexStringValidator.h"

TransmitWidget::TransmitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransmitWidget)
{
    ui->setupUi(this);

    //ui->lineEdit->setInputMask( "Hhh" );
    //ui->lineEdit->setValidator(new HexStringValidator(0x001, 0x7ff));
}

TransmitWidget::~TransmitWidget()
{
    delete ui;
}
