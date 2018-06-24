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
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onTreeViewContextMenu(const QPoint &)));

    ui->customButtonGroupbox->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->customButtonGroupbox, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onButtonContextMenu(const QPoint &)));

}

CommanderDialog::~CommanderDialog()
{
    delete ui;
}

void CommanderDialog::onTreeViewContextMenu(const QPoint &point)
{

    QModelIndexList selectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    m_TreeMenuContext.clickedIndex = ui->treeView->indexAt(point);

    QMenu contextMenu(this);
    contextMenu.addAction(ui->actionAddParameter);
    if(!selectedIndexes.empty())
        contextMenu.addAction(ui->actionDelete);

    contextMenu.exec(ui->treeView->mapToGlobal(point));
}

void CommanderDialog::onButtonContextMenu(const QPoint &point)
{

    m_buttonMenuContext.clickedWidget = ui->customButtonGroupbox->childAt(point);

    QMenu contextMenu(this);
    contextMenu.addAction(ui->actionAddButton);
    if( m_buttonMenuContext.clickedWidget) {
        contextMenu.addAction(ui->actionEditButton);
        contextMenu.addAction(ui->actionDeleteButton);
    }

    contextMenu.exec(ui->customButtonGroupbox->mapToGlobal(point));
}

void CommanderDialog::on_actionAddParameter_triggered()
{
    m_model->addNode(m_TreeMenuContext.clickedIndex, new ParameterNode("New Parameter"));
}

void CommanderDialog::on_actionDelete_triggered()
{

}

void CommanderDialog::on_actionAddButton_triggered()
{
    ui->customButtonVerticalLayout->addWidget(new QPushButton("New Button"));
}

void CommanderDialog::on_actionDeleteButton_triggered()
{
    ui->customButtonVerticalLayout->removeWidget(m_buttonMenuContext.clickedWidget);
    delete m_buttonMenuContext.clickedWidget;
    m_buttonMenuContext.clickedWidget = 0;
}

void CommanderDialog::on_actionEditButton_triggered()
{

}
