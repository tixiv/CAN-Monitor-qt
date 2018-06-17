#ifndef SERIALPORTCOMBOBOX_H
#define SERIALPORTCOMBOBOX_H

#include <QComboBox>

class SerialPortComboBox : public QComboBox
{
public:
    SerialPortComboBox(QWidget *parent = Q_NULLPTR);
    void showPopup() override;

};

#endif // SERIALPORTCOMBOBOX_H
