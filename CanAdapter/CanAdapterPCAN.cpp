#include "CanAdapterPCAN.h"

#include <QThread>
#include <QMessageBox>
#include "CanHub/CanHub.h"


CanAdapterPCAN::CanAdapterPCAN(CanHub &canHub)
{
    m_canHandle = canHub.getNewHandle(CanHub::f_isCanAdapter);

    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(transmit(can_message_t)));
}

CanAdapterPCAN::~CanAdapterPCAN(){
    delete m_canHandle;
}

bool CanAdapterPCAN::open()
{
    return true;
}

void CanAdapterPCAN::close()
{
}

void CanAdapterPCAN::transmit(can_message_t cmsg)
{
}

bool CanAdapterPCAN::isOpen()
{
    return false;
}

QWidget * CanAdapterPCAN::getControlWidget(QWidget *parent){
    //auto controlWidget = new SlcanControlWidget(parent);
    //connect(controlWidget, SIGNAL(openClicked(QString, CanAdapterLawicel::OpenMode, int)), this, SLOT(openClicked(QString, CanAdapterLawicel::OpenMode, int)));
    //connect(controlWidget, SIGNAL(closeClicked()), this, SLOT(closeClicked()));
    //connect(this, SIGNAL(openOperationEnded(bool)), controlWidget, SLOT(openOperationEnded(bool)));

    //return controlWidget;
    return 0;
}
