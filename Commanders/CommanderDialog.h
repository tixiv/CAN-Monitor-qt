#ifndef COMMANDERDIALOG_H
#define COMMANDERDIALOG_H

#include "ButtonEditDialog.h"
#include <QDialog>
#include <QModelIndex>

class ParameterTreeModel;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace Ui {
class CommanderDialog;
}

class CommanderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommanderDialog(QWidget *parent = 0);
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

private:
    Ui::CommanderDialog *ui;

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

        void readFromXml(QXmlStreamReader &reader);
        void writeToXml(QXmlStreamWriter &writer) const;
    };

    QList<CommanderButton> m_commanderButtons;
    int getIndexOfButton(QWidget *w);
    void insertButton(int index, CommanderButtonData d);
    void deleteButton(int index);

};

#endif // COMMANDERDIALOG_H
