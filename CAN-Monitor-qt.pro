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
    WidgetUtils/HexStringValidator.cpp \
    WidgetUtils/CustomLineEdit.cpp \
    lib-slcan/slcan.c \
    TransmitWidget.cpp \
    CanAdapter/CanAdapterLoopback.cpp \
    CanAdapter/CanAdapterFactory.cpp \
    CanAdapter/CanAdapterTesting.cpp \
    WidgetUtils/MenuOpenKeeper.cpp \
    CanAdapter/TestingControlWidget.cpp

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
    WidgetUtils/HexStringValidator.h \
    WidgetUtils/CustomLineEdit.h \
    lib-slcan/can_message.h \
    lib-slcan/slcan.h \
    TransmitWidget.h \
    CanAdapter/CanAdapterLoopback.h \
    CanAdapter/CanAdapterFactory.h \
    CanAdapter/CanAdapterTesting.h \
    WidgetUtils/MenuOpenKeeper.h \
    CanAdapter/TestingControlWidget.h



FORMS    += MainWindow.ui \
    CanAdapter/SlcanControlWidget.ui \
    TransmitWidget.ui \
    CanAdapter/TestingControlWidget.ui

RC_ICONS = canMonitorIcon.ico
