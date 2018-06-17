#include "TransmitWidget.h"
#include "ui_TransmitWidget.h"
#include "WidgetUtils/HexStringValidator.h"

#include <QLabel>

static CustomLineEdit * addField(QHBoxLayout * hb, const QString &label, int minimum, int maximum, const QString &str){
    CustomLineEdit * le = new CustomLineEdit();
    auto vbl = new QVBoxLayout();
    le->setAlignment(Qt::AlignRight);
    le->setValidator(new HexStringValidator(minimum, maximum));
    le->setText(str);
    le->setFixedWidth(20);

    auto idLabel = new QLabel(label);
    idLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    vbl->addWidget(idLabel);
    vbl->addWidget(le);
    hb->addLayout(vbl);

    return le;
}

TransmitWidget::TransmitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransmitWidget)
{
    ui->setupUi(this);

    m_idEdit = addField(ui->messageHorizontalLayout, "ID (hex)", 1,0x7ff, "001");
    m_idEdit->setFixedWidth(70);

    m_dlcEdit = addField(ui->messageHorizontalLayout, "DLC", 0,8, "8");
    ui->messageHorizontalLayout->addSpacing(20);
    for(int i=0; i<8; i++)
    {
        auto name = QString().sprintf("D %d",i);
        m_dataEdits.append(addField(ui->messageHorizontalLayout, name, 0, 0xff, "00"));
    }
    ui->messageHorizontalLayout->addStretch(100);

}

TransmitWidget::~TransmitWidget()
{
    delete ui;
}

void TransmitWidget::on_transmitButton_clicked()
{
    can_message_t cmsg;
    cmsg.IDE = ui->IDEcheckBox->isChecked();
    cmsg.RTR = ui->RTRcheckBox->isChecked();
    cmsg.id = m_idEdit->text().toInt(0,16);
    cmsg.dlc = m_dlcEdit->text().toInt();
    for(int i=0; i<8; i++)
    {
        cmsg.data[i] = m_dataEdits[i]->text().toInt(0,16);
    }
    emit onTransmit(cmsg);
}

void TransmitWidget::on_IDEcheckBox_toggled(bool checked)
{
    if(checked)
        m_idEdit->setValidator(new HexStringValidator(1, 0x1fffffff));
    else
        m_idEdit->setValidator(new HexStringValidator(1, 0x7ff));

    QString tmp = m_idEdit->text();
    m_idEdit->validator()->fixup(tmp);
    m_idEdit->setText(tmp);
}
