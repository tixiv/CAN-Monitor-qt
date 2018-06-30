#include "CanAdapterTritium.h"

#include <QThread>
#include <QMessageBox>
#include "lib-slcan/slcan.h"
#include "SlcanControlWidget.h"

CanAdapterTritium::CanAdapterTritium(CanHub &canHub)
{
    m_canHandle = canHub.getNewHandle(CanHub::f_isCanAdapter);

    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(transmit(can_message_t)));
}

CanAdapterTritium::~CanAdapterTritium(){
    close();
    delete m_canHandle;
}

bool CanAdapterTritium::open()
{
    return true;
}

void CanAdapterTritium::close()
{
}

void CanAdapterTritium::transmit(can_message_t cmsg)
{
}

bool CanAdapterTritium::isOpen()
{
    return true;
}

QWidget * CanAdapterTritium::getControlWidget(QWidget *parent){
    return 0;
}

void CanAdapterTritium::openClicked(){
    open();
}

void CanAdapterTritium::closeClicked(){
    close();
}
