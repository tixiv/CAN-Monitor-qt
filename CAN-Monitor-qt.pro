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
    lib-slcan/slcan.c \
    SerialPortComboBox.cpp

HEADERS  += MainWindow.h \
    CanTree/TreeModel.h \
    CanTree/CanTreeModel.h \
    CanTree/TreeNode.h \
    CanTree/HeaderTreeNode.h \
    CanTree/MessageTreeNode.h \
    CanAdapter/CanAdapter.h \
    CanAdapter/CanAdapterLawicel.h \
    lib-slcan/can_message.h \
    lib-slcan/slcan.h \
    SerialPortComboBox.h

FORMS    += MainWindow.ui

RC_ICONS = canMonitorIcon.ico
