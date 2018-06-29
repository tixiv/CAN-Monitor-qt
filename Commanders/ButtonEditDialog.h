#ifndef BUTTONEDITDIALOG_H
#define BUTTONEDITDIALOG_H

#include <QDialog>

class QXmlStreamReader;
class QXmlStreamWriter;

namespace Ui {
class ButtonEditDialog;
}

struct CommanderButtonData {
    QString text;
    uint8_t command = 0;
    uint8_t subCommand = 0;
    int32_t value = 0;
    bool isSaveButton = false;
    QString saveRange;

    void readFromXml(QXmlStreamReader &reader);
    void writeToXml(QXmlStreamWriter &writer) const;

public:
    QByteArray getSaveCommands();
};

class ButtonEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ButtonEditDialog(QWidget *parent = 0);
    ~ButtonEditDialog();

    CommanderButtonData dialogData;

    int exec() override;

protected:
    void done(int r) override;


private slots:
    void on_isSaveButtonCheckBox_toggled(bool checked);

private:
    Ui::ButtonEditDialog *ui;
};

#endif // BUTTONEDITDIALOG_H
