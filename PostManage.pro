#-------------------------------------------------
#
# Project created by QtCreator 2016-08-24T17:39:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PostManage
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    adduser.cpp \
    serial.cpp \
    addpost.cpp

HEADERS  += mainwindow.h \
    adduser.h \
    datatype.h \
    serial.h \
    addpost.h

FORMS    += mainwindow.ui \
    adduser.ui \
    addpost.ui
QT       += serialport

TRANSLATIONS+=cn.ts
DISTFILES += \
    cn.ts

RESOURCES += \
    logo.qrc
RC_FILE = res.rc
