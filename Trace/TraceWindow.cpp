#include "TraceWindow.h"
#include "ui_TraceWindow.h"

#include "CanHub/CanHub.h"
#include "CanTable/CanTableModel.h"
#include "util/rangeParse.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

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
    m_recording = false;
    if(checked)
        m_ids = rangeParse(ui->identifiersEdit->text());

    m_recording = checked;
}

void TraceWindow::on_clearButton_clicked()
{
    m_model->deleteAll();
}

void TraceWindow::on_actionSave_Log_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                           tr("Save Log"), QString(),
                                           tr("CSV files (*.csv)"));

    if(filename == "")
        return;

    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        m_model->rootNode()->for_tree([&stream](TreeNode * tn)
        {
            auto ctn = static_cast<CanTableNode*>(tn);
            ctn->writeToStream(stream);
        });

        file.close();
    }else{
        QMessageBox::warning(0, tr("CAN Monitor"),
                             tr("The file\"") + filename + tr("\"could not be opened.\n") +
                             tr("The error message was: ") + file.errorString(),
                             QMessageBox::Ok);
    }
}
