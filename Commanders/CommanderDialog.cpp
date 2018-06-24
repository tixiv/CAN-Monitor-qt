#include "CommanderDialog.h"
#include "ui_CommanderDialog.h"
#include "ParameterTree/ParameterTreeModel.h"
#include "ParameterTree/ParameterNode.h"
#include <QMenu>

CommanderDialog::CommanderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommanderDialog)
{
    ui->setupUi(this);

    m_model = new ParameterTreeModel();
    ui->treeView->setModel(m_model);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

}

CommanderDialog::~CommanderDialog()
{
    delete ui;
}

void CommanderDialog::onCustomContextMenu(const QPoint &point)
{

    QModelIndexList selectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    m_contextMenuContext.clickedIndex = ui->treeView->indexAt(point);

    QMenu contextMenu(this);
    contextMenu.addAction(ui->actionAddParameter);
    if(!selectedIndexes.empty())
        contextMenu.addAction(ui->actionDelete);

    contextMenu.exec(ui->treeView->mapToGlobal(point));
}

void CommanderDialog::on_actionAddParameter_triggered()
{
    m_model->addNode(m_contextMenuContext.clickedIndex, new ParameterNode());
}

void CommanderDialog::on_actionDelete_triggered()
{

}
