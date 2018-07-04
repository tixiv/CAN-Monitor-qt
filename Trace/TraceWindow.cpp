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
    m_model->addNode(QModelIndex(), new CanTableNode());
    m_model->addNode(QModelIndex(), new CanTableNode());
    m_model->addNode(QModelIndex(), new CanTableNode());
    m_model->addNode(QModelIndex(), new CanTableNode());
    m_model->addNode(QModelIndex(), new CanTableNode());
    ui->tableView->setModel(m_model);

    canHandle = canHub.getNewHandle();


}

TraceWindow::~TraceWindow()
{
    delete canHandle;
    delete ui;
    delete m_model;
}
