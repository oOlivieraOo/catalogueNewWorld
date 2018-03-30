#-------------------------------------------------
#
# Project created by QtCreator 2017-11-12T09:46:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = producteur
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialoglogin.cpp

HEADERS  += mainwindow.h \
    dialoglogin.h

FORMS    += mainwindow.ui \
    dialoglogin.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    prod.qrc

