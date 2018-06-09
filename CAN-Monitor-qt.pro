#-------------------------------------------------
#
# Project created by QtCreator 2018-06-09T14:02:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAN-Monitor-qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treemodel.cpp \
    treenode.cpp \
    headertreenode.cpp \
    messagetreenode.cpp

HEADERS  += mainwindow.h \
    treemodel.h \
    treenode.h \
    headertreenode.h \
    messagetreenode.h

FORMS    += mainwindow.ui
