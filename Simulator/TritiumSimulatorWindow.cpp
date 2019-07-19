#include "TritiumSimulatorWindow.h"
#include "ui_TritiumSimulatorWindow.h"

TritiumSimulatorWindow::TritiumSimulatorWindow(QWidget *parent, CanHub &canHub) :
    QMainWindow(parent),
    ui(new Ui::TritiumSimulatorWindow)
{
    ui->setupUi(this);

    m_canHandle = canHub.getNewHandle();
    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(messageReceived(can_message_t)));

    connect(&m_tickTimer, SIGNAL(timeout()), this, SLOT(tickTimerTimeout()));
    m_tickTimer.start(1000);
}

TritiumSimulatorWindow::~TritiumSimulatorWindow()
{
    delete ui;
}

void TritiumSimulatorWindow::tickTimerTimeout()
{
    can_message_t cmsg;
    cmsg.IDE = 0; cmsg.RTR = 0; cmsg.dlc = 8;
    cmsg.id = m_baseId;
    memcpy(cmsg.data, "T088", 4);
    uint32_t serNum = 1234;
    memcpy(cmsg.data+4, &serNum, 4);
    m_canHandle->transmit(cmsg);
}

void TritiumSimulatorWindow::messageReceived(can_message_t cmsg)
{
    if(cmsg.IDE || cmsg.RTR || cmsg.dlc != 8)
        return;

    if(cmsg.id == m_baseId + 0x12)
    {
        //07 00 00 02 08 00 00 01
        //01 00 07 00 FA 00 05 00
        const uint8_t msg1[] = {0x01,0x00,0x07,0x00,0xFA,0x00,0x05,0x00};
        if(memcmp(cmsg.data, &msg1, 8) == 0)
        {
            can_message_t cmsg;
            cmsg.IDE = 0; cmsg.RTR = 0; cmsg.dlc = 8;
            cmsg.id = m_baseId + 0x12;
            const uint8_t msg2[] = {0x03,0x00,0x07,0x00,0xFA,0x00,0x05,0x00};
            memcpy(cmsg.data, &msg2, 8);
            m_canHandle->transmit(cmsg);

            cmsg.id = m_baseId + 0x14;
            const uint8_t msg3[] = {0x00,0x70,0x00,0x02,0x00,0x35,0x11,0x00};
            memcpy(cmsg.data, &msg3, 8);
            m_canHandle->transmit(cmsg);
        }
        const uint8_t msg4[] = {0x04,0,0,0,0,0,0,0};
        if(memcmp(cmsg.data, &msg4, 8) == 0)
        {
            can_message_t cmsg;
            cmsg.IDE = 0; cmsg.RTR = 0; cmsg.dlc = 8;
            cmsg.id = m_baseId + 0x14;
            const uint8_t msg5[] = {0x80,0,0,0,0,0,0,0};
            memcpy(cmsg.data, &msg5, 8);
            m_canHandle->transmit(cmsg);
        }
        const uint8_t msg6[] = {0x02,0,0,0,0,0,0,0};
        if(memcmp(cmsg.data, &msg6, 8) == 0)
        {
            can_message_t cmsg;
            cmsg.IDE = 0; cmsg.RTR = 0; cmsg.dlc = 8;
            cmsg.id = m_baseId + 0x12;
            const uint8_t msg7[] = {0x02,0,0x02,0,0,0,0,0};
            memcpy(cmsg.data, &msg7, 8);
            m_canHandle->transmit(cmsg);
        }
    }
}
