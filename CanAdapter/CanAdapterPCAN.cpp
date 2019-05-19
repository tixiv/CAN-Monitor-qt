#include "CanAdapterPCAN.h"
#include "CanHub/CanHub.h"
#include <QThread>
#include <QMessageBox>
#include <QLibrary>
#include <iostream>


#define LPSTR  char*
#define UINT64 uint64_t
#define DWORD  uint32_t
#define WORD   uint16_t
#define BYTE   uint8_t
#include "third_party/peak_system/PCANBasic.h"

struct LibraryCalls
{


    typeof(CAN_Initialize) * CAN_Initialize;
    typeof(CAN_Write) * CAN_Write;
    typeof(CAN_Read) * CAN_Read;
    typeof(CAN_GetErrorText) * CAN_GetErrorText;



};

bool CanAdapterPCAN::loadPcanLibrary()
{
    m_lib.setFileName("PCANBasic");
    if(!m_lib.load())
        goto load_failed;

    delete calls;
    calls = new LibraryCalls();

#define RESOLVE_LIB_CALL(c) calls->c = (typeof(c)*)m_lib.resolve(#c); if(!calls->c) goto load_failed;

    RESOLVE_LIB_CALL(CAN_Initialize);
    RESOLVE_LIB_CALL(CAN_Write);
    RESOLVE_LIB_CALL(CAN_Read);
    RESOLVE_LIB_CALL(CAN_GetErrorText);

    return true;

load_failed:



    return false;
}

bool CanAdapterPCAN::initialize()
{
    TPCANStatus status;
    status = calls->CAN_Initialize(PCAN_USBBUS1, PCAN_BAUD_125K, 0, 0, 0);

    if(status == PCAN_ERROR_OK)
        return true;

    char text[1024];
    calls->CAN_GetErrorText(status, 0, text);

    QMessageBox::warning(0, tr("CAN Monitor"),
                         tr("The PCAN hardware could not be initialized: ") + QString(text),
                         QMessageBox::Ok);


    return false;
}

CanAdapterPCAN::CanAdapterPCAN(CanHub &canHub)
{
    m_canHandle = canHub.getNewHandle(CanHub::f_isCanAdapter);

    if(!loadPcanLibrary())
    {
        QMessageBox::warning(0, tr("CAN Monitor"),
                             tr("PCANBasic.dll could not be loaded. The device will not work. Please install PCANBasic from http://www.peak-system.com"),
                             QMessageBox::Ok);
        return;
    }

    if(!initialize())
    {
        return;
    }

    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(transmit(can_message_t)));
    connect(&m_tickTimer, SIGNAL(timeout()), this, SLOT(tickTimerTimeout()));
    m_tickTimer.setInterval(10);
    m_tickTimer.start();
}

CanAdapterPCAN::~CanAdapterPCAN(){
    delete m_canHandle;
    delete calls;
}

void CanAdapterPCAN::tickTimerTimeout()
{
    TPCANStatus status = PCAN_ERROR_OK;

    while(status == PCAN_ERROR_OK){
        TPCANMsg pmsg;
        TPCANTimestamp timestamp;

        status = calls->CAN_Read(PCAN_USBBUS1, &pmsg, &timestamp);

        if(status == PCAN_ERROR_OK)
        {
            can_message_t cmsg;

            cmsg.id = pmsg.ID;
            cmsg.dlc = pmsg.LEN;
            cmsg.IDE = pmsg.MSGTYPE & PCAN_MESSAGE_EXTENDED ? 1:0;
            cmsg.RTR = pmsg.MSGTYPE & PCAN_MESSAGE_RTR ? 1:0;
            memcpy(cmsg.data, pmsg.DATA, 8);

            m_canHandle->transmit(cmsg);
        }
        else if(status == PCAN_ERROR_QRCVEMPTY)
        {
            // No error, the Q ist just empty
        }
        else
        {
            std::cout << status << std::endl;
            // TODO: report error
        }
    }
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
    TPCANStatus status;
    TPCANMsg pmsg;
    pmsg.ID = cmsg.id;
    pmsg.LEN = cmsg.dlc;
    memcpy(pmsg.DATA, cmsg.data, 8);

    pmsg.MSGTYPE = 0;
    if(cmsg.IDE) pmsg.MSGTYPE |= PCAN_MESSAGE_EXTENDED;
    if(cmsg.RTR) pmsg.MSGTYPE |= PCAN_MESSAGE_RTR;

    status = calls->CAN_Write(PCAN_USBBUS1, &pmsg);

    if(status != PCAN_ERROR_OK)
    {
        std::cout << status << std::endl;
        // TODO: report error
    }
}

bool CanAdapterPCAN::isOpen()
{
    return false;
}

QWidget * CanAdapterPCAN::getControlWidget(QWidget *parent){
    (void) parent;
    //auto controlWidget = new SlcanControlWidget(parent);
    //connect(controlWidget, SIGNAL(openClicked(QString, CanAdapterLawicel::OpenMode, int)), this, SLOT(openClicked(QString, CanAdapterLawicel::OpenMode, int)));
    //connect(controlWidget, SIGNAL(closeClicked()), this, SLOT(closeClicked()));
    //connect(this, SIGNAL(openOperationEnded(bool)), controlWidget, SLOT(openOperationEnded(bool)));

    //return controlWidget;
    return 0;
}
