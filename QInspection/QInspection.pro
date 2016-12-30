#-------------------------------------------------
#
# Project created by QtCreator 2016-08-29T07:31:25
#
#-------------------------------------------------
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += core \
            gui \
            serialport \
            widgets


TARGET = QInspection

TEMPLATE = app

CONFIG += c++14

INCLUDEPATH += include/

SOURCES +=  src/main.cpp\
            src/mainwindow.cpp \
            src/allindicators.cpp \
            src/candatapacket.cpp \
            src/combinedlightingitems.cpp \
            src/daytimeillumination.cpp \
            src/led1.cpp \
            src/led2.cpp \
            src/led3.cpp \
            src/led4.cpp \
            src/nighttimeillumination.cpp \
            src/nolighting.cpp \
            src/pn75K308.cpp \
            src/pn75K309.cpp \
            src/pn75K313.cpp \
            src/pn75K314.cpp \
            src/serialport.cpp \
            src/arduinomega.cpp \
    src/canmessage.cpp

HEADERS  += \
            include/serialport.h \
            include/u55xswitch.h \
            include/pn75K308.h \
            include/pn75K309.h \
            include/pn75K313.h \
            include/pn75K314.h \
            include/nolighting.h \
            include/nighttimeillumination.h \
            include/lightingitem.h \
            include/led1.h \
            include/led2.h \
            include/led3.h \
            include/led4.h \
            include/inspectionitem.h \
            include/daytimeillumination.h \
            include/combinedlightingitems.h \
            include/candatapacket.h \
            include/allindicators.h \
            include/mainwindow.h \
    include/arduinomega.h \
    include/canmessage.h

FORMS    += forms/mainwindow.ui
