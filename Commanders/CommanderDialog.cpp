#include "CommanderDialog.h"
#include "ui_CommanderDialog.h"
#include "ParameterTree/ParameterTreeModel.h"
#include "ParameterTree/ParameterNode.h"
#include "ParameterTree/ParameterGroupNode.h"
#include "ButtonEditDialog.h"
#include <QMenu>
#include <QFileDialog>
#include <QSettings>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QDebug>
#include "Tree/ComboBoxDelegate.h"

CommanderDialog::CommanderDialog(QWidget *parent, CanHub * canHub, QString name) :
    QMainWindow(parent),
    ui(new Ui::CommanderDialog)
{
    ui->setupUi(this);

    m_model = new ParameterTreeModel();
    ui->treeView->setModel(m_model);
    ui->treeView->setColumnWidth(0,180);

    auto accessDelegate = new ComboBoxDelegate(0, ParameterNode::accessStrings);
    ui->treeView->setItemDelegateForColumn(3, accessDelegate);

    //auto typeDelegate = new ComboBoxDelegate(0, )

    if(name != "")
    {
        m_name = name;
        load();
        setEditMode(false);
    }
    else
    {
        m_createdNew = true;
        m_name = "New Commander";
        setEditMode(true);
    }

    ui->treeView->expandAll();

    setWindowTitle(m_name);

    m_canHandle = canHub->getNewHandle();
    connect(m_canHandle, SIGNAL(received(can_message_t)), this, SLOT(onCanReceived(can_message_t)));

    connect(m_model, SIGNAL(newValueEdited(ParameterTreeNode*)), this, SLOT(newValueEdited(ParameterTreeNode*)));

    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onTreeViewContextMenu(const QPoint &)));

    connect(ui->customButtonGroupbox, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onButtonContextMenu(const QPoint &)));
}

CommanderDialog::~CommanderDialog()
{
    delete m_canHandle;
    m_canHandle = 0;
    delete ui;
}

void CommanderDialog::onTreeViewContextMenu(const QPoint &point)
{

    QModelIndexList selectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    m_TreeMenuContext.clickedIndex = ui->treeView->indexAt(point);

    QMenu contextMenu(this);
    contextMenu.addAction(ui->actionAddParameter);
    contextMenu.addAction(ui->actionAddGroup);
    if(!selectedIndexes.empty())
        contextMenu.addAction(ui->actionDelete);

    contextMenu.exec(ui->treeView->mapToGlobal(point));
}

int CommanderDialog::getIndexOfButton(QObject *w)
{

    for(int i=0; i< m_commanderButtons.length(); i++)
    {
        if(m_commanderButtons.at(i).button == w)
            return i;
    }
    return -1;
}

void CommanderDialog::on_actionAddParameter_triggered()
{
    m_model->addNode(m_TreeMenuContext.clickedIndex, new ParameterNode("New Parameter"));
}

void CommanderDialog::on_actionAddGroup_triggered()
{
    m_model->addNode(m_TreeMenuContext.clickedIndex, new ParameterGroupNode("New Group"));
}

void CommanderDialog::on_actionDelete_triggered()
{
    m_model->deleteBranches(ui->treeView->selectionModel()->selectedIndexes());
}

void CommanderDialog::onButtonContextMenu(const QPoint &point)
{

    QWidget * clickedItem = ui->customButtonGroupbox->childAt(point);
    int index = getIndexOfButton(clickedItem);
    m_buttonMenuContext.clickedIndex = index;

    QMenu contextMenu(this);
    contextMenu.addAction(ui->actionAddButton);
    if(index >= 0) {
        contextMenu.addAction(ui->actionEditButton);
        contextMenu.addAction(ui->actionDeleteButton);
        if(index >= 1)contextMenu.addAction(ui->actionMoveButtonUp);
        if(index < m_commanderButtons.length()-1)contextMenu.addAction(ui->actionMoveButtonDown);
    }

    contextMenu.exec(ui->customButtonGroupbox->mapToGlobal(point));
}

void CommanderDialog::insertButton(int index, CommanderButtonData d)
{
    if(index < 0)
        index = m_commanderButtons.count();
    CommanderButton b;
    b.d = d;
    b.button = new QPushButton(b.d.text);
    m_commanderButtons.insert(index, b);
    ui->customButtonVerticalLayout->insertWidget(index, b.button);
    connect(b.button, SIGNAL(clicked(bool)), this, SLOT(onCommanderButtonClicked()));
}

void CommanderDialog::on_actionAddButton_triggered()
{
    auto dialog = new ButtonEditDialog();
    int res = dialog->exec();
    if(res == QDialog::Accepted){
        int index = m_buttonMenuContext.clickedIndex;
        if(index != -1) index++;
        insertButton(index, dialog->dialogData);
        isUserModified = true;
    }
}

void CommanderDialog::deleteButton(int index)
{
    if(index < 0)
        return;
    auto b = m_commanderButtons.at(index);
    ui->customButtonVerticalLayout->removeWidget(b.button);
    delete b.button;
    m_commanderButtons.removeAt(index);
}

void CommanderDialog::on_actionDeleteButton_triggered()
{
    deleteButton(m_buttonMenuContext.clickedIndex);
    isUserModified = true;
}

void CommanderDialog::on_actionEditButton_triggered()
{
    auto dialog = new ButtonEditDialog();
    dialog->dialogData = m_commanderButtons.at(m_buttonMenuContext.clickedIndex).d;
    int res = dialog->exec();
    if(res == QDialog::Accepted){
        CommanderButton * b = &m_commanderButtons[m_buttonMenuContext.clickedIndex];
        b->d= dialog->dialogData;
        b->button->setText(b->d.text);
        isUserModified = true;
    }
}

void CommanderDialog::on_actionMoveButtonUp_triggered()
{
    int index = m_buttonMenuContext.clickedIndex;
    auto b = m_commanderButtons.at(index);
    deleteButton(index);
    insertButton(index-1, b.d);
    isUserModified = true;
}

void CommanderDialog::on_actionMoveButtonDown_triggered()
{
    int index = m_buttonMenuContext.clickedIndex;
    auto b = m_commanderButtons.at(index);
    deleteButton(index);
    insertButton(index+1, b.d);
    isUserModified = true;
}

void CommanderDialog::transmitCanMessage(uint8_t command, uint8_t subCommand, int32_t value, bool write)
{
    can_message_t cmsg;
    cmsg.IDE = 0;
    cmsg.RTR = 0;
    cmsg.dlc = 8;
    cmsg.id = m_properties.canIdWrite;
    cmsg.data[0] = 0x37;
    cmsg.data[1] = 0x13;
    if(write)
        cmsg.data[2] = command;
    else
        cmsg.data[2] = command + 1;
    cmsg.data[3] = subCommand;
    cmsg.data[4] = value >>  0;
    cmsg.data[5] = value >>  8;
    cmsg.data[6] = value >> 16;
    cmsg.data[7] = value >> 24;
    m_canHandle->transmit(cmsg);
}

void CommanderDialog::onCommanderButtonClicked()
{
    int index = getIndexOfButton(sender());
    auto b = m_commanderButtons.at(index);
    if(b.d.asksAreYouSure)
    {
        int res = QMessageBox::question(this, m_name,
                              b.d.text + ":\n" + tr("Are you sure ?"),
                              QMessageBox::Yes | QMessageBox::No);
        if(res != QMessageBox::Yes)
            return;
    }

    transmitCanMessage(b.d.command, b.d.subCommand, b.d.value, true);

    if(b.d.isSaveButton)
    {
        auto saveCommands = b.d.getSaveCommands();
        m_model->rootNode()->for_tree([&saveCommands](TreeNode * node){
            auto pn = dynamic_cast<ParameterNode*>(node);
            if(pn)
            {
                auto pd = pn->getParameterData();
                if(saveCommands.contains(pd.command))
                    pn->setSaved();
            }
        });
    }
}


void CommanderDialog::saveProperties(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Properties");
    m_properties.writeToXml(writer);
    writer.writeEndElement();
}

void CommanderDialog::loadProperties(QXmlStreamReader &reader)
{
    if (!reader.readNextStartElement())
        return;
    if (reader.name() != "Properties")
        return;
    m_properties.readFromXml(reader);
}

void CommanderDialog::saveButtons(QXmlStreamWriter &writer)
{
    writer.writeStartElement("commanderButtons");
    foreach(auto b, m_commanderButtons)
    {
        writer.writeStartElement("Button");
        b.d.writeToXml(writer);
        writer.writeEndElement();
    }
    writer.writeEndElement();
}

void CommanderDialog::loadButtons(QXmlStreamReader &reader)
{
    if (!reader.readNextStartElement())
        return;
    if (reader.name() != "commanderButtons")
        return;
    while(reader.readNextStartElement())
    {
        if(reader.name() == "Button")
        {
            CommanderButtonData d;
            d.readFromXml(reader);
            insertButton(-1, d);
        }
    }
}

void CommanderDialog::load()
{
    QString path = QSettings().value("commanders/path").toString();
    path += "/" + m_name + ".xml";

    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        QXmlStreamReader reader(&file);
        if(reader.readNextStartElement() && reader.name() == "Commander")
        {
            m_model->readTreeFromXml(reader);
            loadButtons(reader);
            loadProperties(reader);
        }
        file.close();
    }
}

bool CommanderDialog::saveCommander(bool interactive)
{
    QString path = QSettings().value("commanders/path").toString();
    if(path == "") interactive = true;
    path += "/" + m_name + ".xml";

    QString filename;
    if(interactive)
    {
        filename = QFileDialog::getSaveFileName(this,
                                       tr("Save Tree"), path,
                                       tr("Xml files (*.xml)"));
        if(filename == "")
            return false;
    }
    else
    {
        filename = path;
    }

    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        m_name = QFileInfo(file).baseName();
        setWindowTitle(m_name);

        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("Commander");
        m_model->writeTreeToXml(writer);
        saveButtons(writer);
        saveProperties(writer);
        writer.writeEndElement();
        writer.writeEndDocument();
        file.close();
        m_model->isUserModified = false;
        isUserModified = false;
        return true;
    }else{
        QMessageBox::warning(this, tr("CAN Monitor"),
                             tr("The file\"") + filename + tr("\"could not be opened.\n") +
                             tr("The error message was: ") + file.errorString(),
                             QMessageBox::Ok);
        return false;
    }
}

void CommanderDialog::on_actionSaveAs_triggered()
{
    saveCommander(true);
}

void CommanderDialog::on_actionSave_Commander_triggered()
{
    if(m_createdNew)
    {
        if(saveCommander(true))
            m_createdNew = false;
    }
    else
    {
        saveCommander(false);
    }
}

void CommanderDialog::on_actionSetProperties_triggered()
{
    auto dialog = new CommanderPropertiesDialog();
    dialog->dialogData = m_properties;
    int res = dialog->exec();
    if(res == QDialog::Accepted){
        m_properties = dialog->dialogData;
        isUserModified = true;
    }
}

void CommanderDialog::onCanReceived(can_message_t cmsg)
{
    if(cmsg.id == m_properties.canIdRead && cmsg.dlc == 8 &&
            cmsg.data[0] == 0x37 && cmsg.data[1] == 0x13)
    {
        int dw = 0;
        dw |= ((int)cmsg.data[4]) <<  0;
        dw |= ((int)cmsg.data[5]) <<  8;
        dw |= ((int)cmsg.data[6]) << 16;
        dw |= ((int)cmsg.data[7]) << 24;

        m_model->inputMessage(cmsg.data[2]-1, cmsg.data[3], dw);
    }
}

void CommanderDialog::newValueEdited(ParameterTreeNode* node)
{
    ParameterNode * pn;
    if(ui->autoWriteCheckBox->isChecked() &&
            (pn = dynamic_cast<ParameterNode*>(node)) )
    {
        auto pd = pn->getParameterData();
        if(pd.newValueSet)
        {
            transmitCanMessage(pd.command, pd.subCommand, pd.newValue, true); // write the parameter
            requestValue(pd); // and read it back
        }
    }
}

void CommanderDialog::requestValue(ParameterNode::ParameterData & pd)
{
    if(pd.accessMode != ParameterNode::a_writeOnly)
        transmitCanMessage(pd.command, pd.subCommand, 0, false); // request value
}

void CommanderDialog::on_readButton_clicked()
{
    m_model->rootNode()->for_tree([this](TreeNode * node){
        auto pn = dynamic_cast<ParameterNode*>(node);
        if(pn)
        {
            auto pd = pn->getParameterData();
            requestValue(pd);
        }
    });
}

void CommanderDialog::on_writeButton_clicked()
{
    m_model->rootNode()->for_tree([this](TreeNode * node){
        auto pn = dynamic_cast<ParameterNode*>(node);
        if(pn)
        {
            auto pd = pn->getParameterData();
            if(pd.newValueSet &&
                    (!pd.valueRead || pd.value != pd.newValue) )
            {
                transmitCanMessage(pd.command, pd.subCommand, pd.newValue, true); // write the parameter
                requestValue(pd); // and read it back
            }
        }
    });
}

void CommanderDialog::closeEvent(QCloseEvent *event)
 {
    if(m_model->isUserModified || isUserModified){
        QMessageBox msgBox;
        msgBox.setText("The Commander has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
            case QMessageBox::Save:
                if (!saveCommander(false))
                    goto ignore;
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                goto ignore;
                break;
        }
    }

    {
        QStringList parmNames;
        foreach (auto b, m_commanderButtons) {
            if(b.d.isSaveButton)
            {
                auto saveCommands = b.d.getSaveCommands();
                m_model->rootNode()->for_tree([&saveCommands,&parmNames](TreeNode * node){
                    auto pn = dynamic_cast<ParameterNode*>(node);
                    if(pn)
                    {
                        auto pd = pn->getParameterData();
                        if(pd.needsSave && saveCommands.contains(pd.command))
                            parmNames.append(pn->getData(pcf_name,Qt::DisplayRole).toString());
                    }
                });
            }
        }
        if(!parmNames.empty())
        {
            QString text = "The following parameters have been modified and not saved:\n";
            foreach (auto parm, parmNames) {
                text += parm + "\n";
            }
            QMessageBox msgBox;
            msgBox.setText(text);
            msgBox.setInformativeText("Do you really want to quit the commander?");
            msgBox.setStandardButtons(QMessageBox::Discard | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            int ret = msgBox.exec();

            switch (ret) {
                case QMessageBox::Discard:
                    break;
                case QMessageBox::Cancel:
                    goto ignore;
                    break;
            }
        }
    }

    event->accept();
    deleteLater();
    return;

ignore:
    event->ignore();
    return;

}


void CommanderDialog::setEditMode(bool active)
{
    ui->actionEditMode->setChecked(active);
    if(active)
    {
        ui->customButtonGroupbox->setContextMenuPolicy(Qt::CustomContextMenu);
        ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    }
    else
    {
        ui->customButtonGroupbox->setContextMenuPolicy(Qt::NoContextMenu);
        ui->treeView->setContextMenuPolicy(Qt::NoContextMenu);
    }
    ui->treeView->setColumnHidden(1,!active);
    ui->treeView->setColumnHidden(2,!active);
    ui->treeView->setColumnHidden(3,!active);
    ui->treeView->setColumnHidden(4,!active);
    m_model->setEditMode(active);
}


void CommanderDialog::on_actionEditMode_triggered(bool checked)
{
    setEditMode(checked);
}
