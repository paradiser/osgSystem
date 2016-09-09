
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
    mainInterface.cpp \
    Communication.cpp

HEADERS  += \
    mainInterface.h \
    Communication.h

FORMS    += \
    mainInterface.ui \
    Communication.ui
