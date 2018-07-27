#-------------------------------------------------
#
# Project created by QtCreator 2016-03-03T13:55:43
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = new_zip
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    data_base.cpp \
    form.cpp \
    noeditcolumn.cpp

HEADERS  += mainwindow.h \
    data_base.h \
    form.h \
    noeditcolumn.h

FORMS    += mainwindow.ui \
    form.ui

RESOURCES += \
    icons.qrc
