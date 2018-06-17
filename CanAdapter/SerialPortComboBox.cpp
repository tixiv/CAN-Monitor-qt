#include "SerialPortComboBox.h"

#include <QSerialPortInfo>

static void updatePorts(QComboBox * _this){
    QList<QSerialPortInfo> portlist = QSerialPortInfo::availablePorts();
    QStringList portNames;
    foreach (auto port, portlist)
        portNames.append(port.portName());

    QString selectedPort = _this->currentText();
    _this->clear();
    _this->addItems(portNames);
    if(portNames.contains(selectedPort))
        _this->setCurrentText(selectedPort);
}

SerialPortComboBox::SerialPortComboBox(QWidget *parent)
    :   QComboBox(parent)
{
    updatePorts(this);
}

void SerialPortComboBox::showPopup(){
    updatePorts(this);
    QComboBox::showPopup();
}
