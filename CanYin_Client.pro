#-------------------------------------------------
#
# Project created by QtCreator 2014-11-10T11:02:22
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CanYin_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loginview.cpp \
    uporderform.cpp \
    yf.cpp

HEADERS  += mainwindow.h \
    loginview.h \
    uporderform.h \
    yf.h

FORMS    += mainwindow.ui \
    loginview.ui \
    uporderform.ui \
    test.ui

RESOURCES += \
    qrc.qrc
RC_FILE = icon.rc
include(YF/YF.pri)
