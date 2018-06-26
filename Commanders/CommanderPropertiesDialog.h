#ifndef COMMANDERPROPERTIESDIALOG_H
#define COMMANDERPROPERTIESDIALOG_H

#include <QDialog>

class QXmlStreamReader;
class QXmlStreamWriter;

struct CommanderProperties {
    uint32_t canIdWrite = 0;
    uint32_t canIdRead = 0;

    void readFromXml(QXmlStreamReader &reader);
    void writeToXml(QXmlStreamWriter &writer) const;
};

namespace Ui {
class CommanderPropertiesDialog;
}

class CommanderPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommanderPropertiesDialog(QWidget *parent = 0);
    ~CommanderPropertiesDialog();

    CommanderProperties dialogData;

    int exec() override;

protected:
    void done(int r) override;

private:
    Ui::CommanderPropertiesDialog *ui;
};

#endif // COMMANDERPROPERTIESDIALOG_H
