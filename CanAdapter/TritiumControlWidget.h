#ifndef TRITIUMCONTROLWIDGET_H
#define TRITIUMCONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class TritiumControlWidget;
}

class TritiumControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TritiumControlWidget(QWidget *parent = 0);
    ~TritiumControlWidget();

signals:
    void openClicked();
    void closeClicked();

public slots:
    void displayStatus(QString text);

private slots:
    void on_openCloseButton_clicked();

private:
    Ui::TritiumControlWidget *ui;
    bool m_isOpen = 0;
};

#endif // TRITIUMCONTROLWIDGET_H
