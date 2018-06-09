//#include <QStandardItemModel>
//#include <QFileSystemModel>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "treemodel.h"
#include "headertreenode.h"

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

    TreeNode *ti = new TreeNode(model->rootNode(), "Test1");

    model->rootNode()->appendChild(ti);
    ti->appendChild(new HeaderTreeNode(ti, "foo1"));
    ti->appendChild(new HeaderTreeNode(ti, "foo2"));
    ti->appendChild(new HeaderTreeNode(ti, "foo3"));
    ti->appendChild(new HeaderTreeNode(ti, "foo4"));

    model->rootNode()->appendChild(new HeaderTreeNode(model->rootNode(), "bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode(model->rootNode(), "bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode(model->rootNode(), "bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode(model->rootNode(), "bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode(model->rootNode(), "bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode(model->rootNode(), "bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode(model->rootNode(), "bar1"));
    model->rootNode()->appendChild(new HeaderTreeNode(model->rootNode(), "bar1"));


    ui->treeView->setModel(model);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));


    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked(QModelIndex)));


}

void MainWindow::onCustomContextMenu(const QPoint &point)
{
    QMenu contextMenu(this);
    contextMenu.addAction(ui->actionAdd_Group);

    QModelIndex index = ui->treeView->indexAt(point);
    if (index.isValid() && index.row() % 2 == 0) {
        contextMenu.exec(ui->treeView->mapToGlobal(point));
    }
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
    model->insertNode(model->rootNode(),0,new HeaderTreeNode(model->rootNode(), "Foobar"));
}

void MainWindow::on_actionAdd_Group_triggered()
{

}
