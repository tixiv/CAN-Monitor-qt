#ifndef COMMANDERDIALOG_H
#define COMMANDERDIALOG_H

#include "ButtonEditDialog.h"
#include <QMainWindow>
#include <QModelIndex>

class ParameterTreeModel;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace Ui {
class CommanderDialog;
}

class CommanderDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommanderDialog(QWidget *parent = 0, QString name = "");
    ~CommanderDialog();

private slots:
    void onTreeViewContextMenu(const QPoint &point);
    void onButtonContextMenu(const QPoint &point);

    void on_actionAddParameter_triggered();

    void on_actionDelete_triggered();

    void on_actionAddButton_triggered();

    void on_actionDeleteButton_triggered();

    void on_actionEditButton_triggered();

    void on_actionMoveButtonUp_triggered();

    void on_actionMoveButtonDown_triggered();

    void on_actionSaveAs_triggered();

private:
    Ui::CommanderDialog *ui;

    void closeEvent(QCloseEvent *event) override;

    QString m_name;

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

    QList<CommanderButton> m_commanderButtons;
    int getIndexOfButton(QWidget *w);
    void insertButton(int index, CommanderButtonData d);
    void deleteButton(int index);

    bool saveCommander(bool interactive);

    void saveButtons(QXmlStreamWriter &writer);
    void loadButtons(QXmlStreamReader &reader);
    void load();
};

#endif // COMMANDERDIALOG_H
