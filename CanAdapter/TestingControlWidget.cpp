#include "TestingControlWidget.h"
#include "ui_TestingControlWidget.h"

TestingControlWidget::TestingControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestingControlWidget)
{
    ui->setupUi(this);
}

TestingControlWidget::~TestingControlWidget()
{
    delete ui;
}

void TestingControlWidget::updateMessagesPerSecond(QString text)
{
    ui->label->setText(text);
}
