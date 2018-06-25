#ifndef BUTTONEDITDIALOG_H
#define BUTTONEDITDIALOG_H

#include <QDialog>

namespace Ui {
class ButtonEditDialog;
}

struct CommanderButtonData {
    QString text;
    uint32_t canId = 0;
    uint8_t command = 0;
    uint8_t subCommand = 0;
    int32_t value = 0;
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


private:
    Ui::ButtonEditDialog *ui;
};

#endif // BUTTONEDITDIALOG_H
