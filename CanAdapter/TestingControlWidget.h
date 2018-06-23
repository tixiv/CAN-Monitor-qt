#ifndef TESTINGCONTROLWIDGET_H
#define TESTINGCONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class TestingControlWidget;
}

class TestingControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestingControlWidget(QWidget *parent = 0);
    ~TestingControlWidget();

public slots:
    void updateMessagesPerSecond(QString text);

private:
    Ui::TestingControlWidget *ui;
};

#endif // TESTINGCONTROLWIDGET_H
