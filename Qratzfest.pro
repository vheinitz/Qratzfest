#-------------------------------------------------
#
# Project created by QtCreator 2014-08-01T10:24:34
#
#-------------------------------------------------

QT       += core gui network

TARGET = Qratzfest
TEMPLATE = app


SOURCES += main.cpp\
        qratzfest.cpp\
		comport/comport.cpp \
		persistence.cpp\

HEADERS  += qratzfest.h\
		comport/comport.h \
		persistence.h\

FORMS    += qratzfest.ui
