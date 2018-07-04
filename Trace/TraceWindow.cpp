#include "TraceWindow.h"
#include "ui_TraceWindow.h"

#include "CanHub/CanHub.h"
#include "CanTable/CanTableModel.h"
#include "util/rangeParse.h"

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
    if(m_recording && (m_ids.empty() || m_ids.contains(cmsg.id)))
        m_model->addNode(QModelIndex(), new CanTableNode(&cmsg));
}

TraceWindow::~TraceWindow()
{
    delete m_canHandle;
    delete ui;
    delete m_model;
}

void TraceWindow::on_recordPushButton_clicked(bool checked)
{
    if(checked)
    {
        m_ids = rangeParse(ui->identifiersEdit->text());
        if(!m_ids.empty())
            m_recording = true;
    }
    else
        m_recording = false;
}

void TraceWindow::on_clearButton_clicked()
{
    m_model->deleteAll();
}
