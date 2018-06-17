#ifndef TRANSMITWIDGET_H
#define TRANSMITWIDGET_H

#include <QWidget>

namespace Ui {
class TransmitWidget;
}

class TransmitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransmitWidget(QWidget *parent = 0);
    ~TransmitWidget();

private:
    Ui::TransmitWidget *ui;
};

#endif // TRANSMITWIDGET_H
