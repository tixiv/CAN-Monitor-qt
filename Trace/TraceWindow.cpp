#include "TraceWindow.h"
#include "ui_TraceWindow.h"

#include "CanHub/CanHub.h"
#include "CanTable/CanTableModel.h"

TraceWindow::TraceWindow(QWidget *parent, CanHub &canHub) :
    QMainWindow(parent),
    ui(new Ui::TraceWindow)
{
    ui->setupUi(this);

    m_model = new CanTableModel();
    ui->tableView->setModel(m_model);

    m_canHandle = canHub.getNewHandle();
    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(messageReceived(can_message_t)));

}

void TraceWindow::messageReceived(can_message_t cmsg)
{
    m_model->addNode(QModelIndex(), new CanTableNode(&cmsg));
}

TraceWindow::~TraceWindow()
{
    delete m_canHandle;
    delete ui;
    delete m_model;
}
