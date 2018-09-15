#ifndef CANSOLEIDDIALOG_H
#define CANSOLEIDDIALOG_H

#include <QDialog>

namespace Ui {
class CansoleIdDialog;
}

class CansoleIdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CansoleIdDialog(QWidget *parent = 0);
    ~CansoleIdDialog();

    int exec() override;

    int cansoleId;

protected:
    void done(int r) override;

private:
    Ui::CansoleIdDialog *ui;
};

#endif // CANSOLEIDDIALOG_H
