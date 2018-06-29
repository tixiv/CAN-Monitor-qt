#ifndef COMMANDERDIALOG_H
#define COMMANDERDIALOG_H

#include "ButtonEditDialog.h"
#include "CommanderPropertiesDialog.h"
#include <QMainWindow>
#include <QModelIndex>
#include "CanHub/CanHub.h"

class ParameterTreeModel;
class QXmlStreamReader;
class QXmlStreamWriter;
class ParameterTreeNode;

namespace Ui {
class CommanderDialog;
}

class CommanderDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommanderDialog(QWidget *parent, CanHub * canhub, QString name = "");
    ~CommanderDialog();

private slots:
    void onTreeViewContextMenu(const QPoint &point);
    void onButtonContextMenu(const QPoint &point);

    void newValueEdited(ParameterTreeNode* node);

    void on_actionAddParameter_triggered();

    void on_actionDelete_triggered();

    void on_actionAddButton_triggered();

    void on_actionDeleteButton_triggered();

    void on_actionEditButton_triggered();

    void on_actionMoveButtonUp_triggered();

    void on_actionMoveButtonDown_triggered();

    void on_actionSaveAs_triggered();

    void on_actionSave_Commander_triggered();

    void on_actionSetProperties_triggered();

    void onCommanderButtonClicked();
    void onCanReceived(can_message_t cmsg);
    void on_readButton_clicked();

    void on_writeButton_clicked();

private:
    Ui::CommanderDialog *ui;

    void closeEvent(QCloseEvent *event) override;

    QString m_name;

    CanHandle * m_canHandle;

    ParameterTreeModel * m_model;

    struct{
        QModelIndex clickedIndex;
    }m_TreeMenuContext;

    struct{
        int clickedIndex = -1;
    }m_buttonMenuContext;

    class CommanderButton
    {
    public:
        QPushButton * button;
        CommanderButtonData d;
    };

    bool isUserModified = false;

    CommanderProperties m_properties;
    QList<CommanderButton> m_commanderButtons;
    int getIndexOfButton(QObject *w);
    void insertButton(int index, CommanderButtonData d);
    void deleteButton(int index);

    bool saveCommander(bool interactive);

    void saveButtons(QXmlStreamWriter &writer);
    void loadButtons(QXmlStreamReader &reader);
    void load();
    void saveProperties(QXmlStreamWriter &writer);
    void loadProperties(QXmlStreamReader &reader);
    void transmitCanMessage(uint8_t command, uint8_t subCommand, int32_t value, bool write);
};

#endif // COMMANDERDIALOG_H
