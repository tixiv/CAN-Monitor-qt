#ifndef SLCANCONTROLWIDGET_H
#define SLCANCONTROLWIDGET_H

#include "CanAdapterLawicel.h"
#include <QWidget>

namespace Ui {
    class SlcanControlWidget;
}

class SlcanControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SlcanControlWidget(QWidget *parent = 0);
    ~SlcanControlWidget();

signals:
    void openClicked(QString portName, CanAdapterLawicel::OpenMode om, int baud);
    void closeClicked();

public slots:
    void openOperationEnded(bool success);

private slots:
    void on_openButton_clicked();

private:
    Ui::SlcanControlWidget *ui;
    bool m_open = false;
};

#endif // SLCANCONTROLWIDGET_H
