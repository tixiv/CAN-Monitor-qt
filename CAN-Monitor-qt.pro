#-------------------------------------------------
#
# Project created by QtCreator 2018-06-09T14:02:49
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAN-Monitor-qt
TEMPLATE = app


SOURCES += main.cpp\
    MainWindow.cpp \
    CanTree/TreeModel.cpp \
    CanTree/CanTreeModel.cpp \
    CanTree/TreeNode.cpp \
    CanTree/HeaderTreeNode.cpp \
    CanTree/MessageTreeNode.cpp \
    CanAdapter/CanAdapter.cpp \
    CanAdapter/CanAdapterLawicel.cpp \
    CanAdapter/SlcanControlWidget.cpp \
    CanAdapter/SerialPortComboBox.cpp \
    Widgetutils/HexStringValidator.cpp \
    Widgetutils/CustomLineEdit.cpp \
    lib-slcan/slcan.c \
    TransmitWidget.cpp \

HEADERS  += MainWindow.h \
    CanTree/TreeModel.h \
    CanTree/CanTreeModel.h \
    CanTree/TreeNode.h \
    CanTree/HeaderTreeNode.h \
    CanTree/MessageTreeNode.h \
    CanAdapter/CanAdapter.h \
    CanAdapter/CanAdapterLawicel.h \
    CanAdapter/SlcanControlWidget.h \
    CanAdapter/SerialPortComboBox.h \
    Widgetutils/HexStringValidator.h \
    Widgetutils/CustomLineEdit.h \
    lib-slcan/can_message.h \
    lib-slcan/slcan.h \
    TransmitWidget.h \



FORMS    += MainWindow.ui \
    CanAdapter/SlcanControlWidget.ui \
    TransmitWidget.ui

RC_ICONS = canMonitorIcon.ico
