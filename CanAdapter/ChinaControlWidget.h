#ifndef SLCANCONTROLWIDGET_H
#define SLCANCONTROLWIDGET_H

#include "CanAdapterChina.h"
#include <QWidget>

class QComboBox;

namespace Ui {
    class ChinaControlWidget;
}

class ChinaControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChinaControlWidget(QWidget *parent = 0);
    ~ChinaControlWidget();

signals:
    void openClicked(QString portName, CanAdapterChina::OpenMode om, int baud);
    void closeClicked();

public slots:
    void openOperationEnded(bool success);

private slots:
    void on_openButton_clicked();

private:
    Ui::ChinaControlWidget *ui;
    bool m_open = false;

    const static QString m_settingsName;
    void populateCanBaudComboBox(QComboBox *cb);
    void populateModeComboBox(QComboBox *cb);
};

#endif // SLCANCONTROLWIDGET_H
