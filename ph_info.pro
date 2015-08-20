#-------------------------------------------------
#
# Project created by QtCreator 2015-08-14T22:53:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ph_info
TEMPLATE = app

# Подключаем libkexiv через pkgconfig
CONFIG += link_pkgconfig
PKGCONFIG += libkexiv2
#PKGCONFIG += exiv2

#LIBS += /usr/lib/libexiv2.so.12

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
