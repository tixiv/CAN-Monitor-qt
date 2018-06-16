#include <QSerialPortInfo>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "treemodel.h"
#include "headertreenode.h"
#include "CanAdapterLawicel.h"

void populateCanBaudComboBox(QComboBox * cb){
    cb->addItem("10");
    cb->addItem("20");
    cb->addItem("50");
    cb->addItem("100");
    cb->addItem("125");
    cb->addItem("250");
    cb->addItem("500");
    cb->addItem("800");
    cb->addItem("1000");

    cb->setCurrentIndex(4);
}

void populateModeComboBox(QComboBox * cb){
    cb->addItem("Normal");
    cb->addItem("Listen only");
    cb->addItem("Loopback");
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    populateCanBaudComboBox(ui->canBaudComboBox);
    populateModeComboBox(ui->modeComboBox);

    model = new CanTreeModel();
    ui->treeView->setModel(model);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    m_canAdapter = new CanAdapterLawicel();

    connect(&m_tickTimer, SIGNAL(timeout()), this, SLOT(tickTimerTimeout()));
    m_tickTimer.setInterval(20);
    m_tickTimer.start();
}


void MainWindow::onCustomContextMenu(const QPoint &point)
{
    QMenu contextMenu(this);
    contextMenu.addAction(ui->actionAdd_Group);

    QModelIndex index = ui->treeView->indexAt(point);
    m_contextMenuContext.index = index;
    contextMenu.exec(ui->treeView->mapToGlobal(point));
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
        model->inputMessage(&cmsg);
    }
}
