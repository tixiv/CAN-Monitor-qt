//#include <QStandardItemModel>
//#include <QFileSystemModel>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "treemodel.h"
#include "treenode.h"

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

    TreeModel *model = new TreeModel();

    QList<QVariant> columnData;
    columnData << "foo";
    columnData << "bar";

    TreeNode *ti = new TreeNode(columnData, model->rootNode());

    model->rootNode()->appendChild(ti);
    ti->appendChild(new TreeNode(columnData, ti));
    ti->appendChild(new TreeNode(columnData, ti));
    ti->appendChild(new TreeNode(columnData, ti));
    ti->appendChild(new TreeNode(columnData, ti));

    model->rootNode()->appendChild(new TreeNode(columnData, model->rootNode()));
    model->rootNode()->appendChild(new TreeNode(columnData, model->rootNode()));
    model->rootNode()->appendChild(new TreeNode(columnData, model->rootNode()));
    model->rootNode()->appendChild(new TreeNode(columnData, model->rootNode()));
    model->rootNode()->appendChild(new TreeNode(columnData, model->rootNode()));
    model->rootNode()->appendChild(new TreeNode(columnData, model->rootNode()));


     ui->treeView->setModel(model);


    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked(QModelIndex)));


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

}
