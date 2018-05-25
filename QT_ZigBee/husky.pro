#-------------------------------------------------
#
# Project created by QtCreator 2017-04-12T15:17:43
#
#-------------------------------------------------

QT       += core gui
QT       += serialport charts sql network
#CONFIG += qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = husky
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mywidget.cpp \
    searchfilter.cpp \
    imageviewer.cpp \
    frmmain.cpp \
    mythread.cpp \
    headtitle.cpp \
    qcgaugewidget.cpp \
    qcwthermometer.cpp \
    qcwmeter.cpp \
    mydht.cpp \
    mykeyboard.cpp \
    customtablemodel.cpp \
    tablewidget.cpp \
    delreconqueue.cpp \
    mytcp.cpp \
    mytcp_pic.cpp

HEADERS  += mywidget.h \
    searchfilter.h \
    imageviewer.h \
    frmmain.h \
    mythread.h \
    headtitle.h \
    qcgaugewidget.h \
    qcwthermometer.h \
    qcwmeter.h \
    mydht.h \
    mykeyboard.h \
    customtablemodel.h \
    tablewidget.h \
    delreconqueue.h \
    mytcp.h \
    mytcp_pic.h

FORMS    += mywidget.ui

RESOURCES += \
    myresource.qrc
#LIBS += -L"/home/zhenjun/Qt5.7.1/5.7/gcc_64/lib/" -lqwt
#LIBS += -L"/opt/arm-qt5.7.0/lib/" -lqwt
