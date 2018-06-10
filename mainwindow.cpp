//#include <QStandardItemModel>
//#include <QFileSystemModel>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "treemodel.h"
#include "headertreenode.h"
#include "CanAdapterLawicel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());
    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(QDir::currentPath()));
*/

    model = new TreeModel();

    TreeNode *ti = new TreeNode("Test1");

    model->rootNode()->appendChild(ti);
    ti->appendChild(new HeaderTreeNode("foo1"));
    ti->appendChild(new HeaderTreeNode("foo2"));
    ti->appendChild(new HeaderTreeNode("foo3"));
    ti->appendChild(new HeaderTreeNode("foo4"));

    model->rootNode()->appendChild(new HeaderTreeNode("bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode("bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode("bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode("bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode("bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode("bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode("bar1"));


    ui->treeView->setModel(model);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));


    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked(QModelIndex)));
    connect(&m_tickTimer, SIGNAL(timeout()), this, SLOT(tickTimerTimeout()));

    m_tickTimer.setInterval(20);
    m_tickTimer.start();

    m_canAdapter = new CanAdapterLawicel();
}

void MainWindow::onCustomContextMenu(const QPoint &point)
{
    QMenu contextMenu(this);
    contextMenu.addAction(ui->actionAdd_Group);

    QModelIndex index = ui->treeView->indexAt(point);
    m_contextMenuContext.index = index;
    contextMenu.exec(ui->treeView->mapToGlobal(point));

}

void MainWindow::clicked(const QModelIndex &index)
{
    (void) index;
    //QStandardItem *item = myStandardItemModel->itemFromIndex(index);
    // Do stuff with the item ...
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_canAdapter->open(125000);
}

void MainWindow::on_actionAdd_Group_triggered()
{
    model->insertNode(m_contextMenuContext.index, -1, new HeaderTreeNode("New Group"));
}

void MainWindow::tickTimerTimeout()
{
    can_message_t cmsg;
    while(m_canAdapter->receive(&cmsg)){
        model->insertNode(QModelIndex(), -1, new HeaderTreeNode(cmsg.id));

    }
}
