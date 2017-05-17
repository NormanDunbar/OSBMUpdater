#-------------------------------------------------
#
# Project created by QtCreator 2017-05-08T12:24:07
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OSBMUpdater
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gettoolkitdialog.cpp \
    toolkitdialog.cpp \
    keyworddialog.cpp

HEADERS  += mainwindow.h \
    toolkitdialog.h \
    gettoolkitdialog.h \
    keyworddialog.h

FORMS    += mainwindow.ui \
    toolkitdialog.ui \
    gettoolkitdialog.ui \
    keyworddialog.ui

RESOURCES += \
    resources.qrc
