#-------------------------------------------------
#
# Project created by QtCreator 2016-09-25T10:11:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += -ltjlutils

CONFIG += c++14

TARGET = ArduinoPC
TEMPLATE = app

INCLUDEPATH += include/

SOURCES +=  src/main.cpp\
            src/mainwindow.cpp\
            src/arduinopcicons.cpp \
            src/programloadingwidget.cpp \
            src/arduinopcstrings.cpp \
            src/configurationfilereader.cpp \
            src/custompushbutton.cpp \
            src/customaction.cpp \
            src/customcombobox.cpp \
            src/arduinoscheduler.cpp \
            \
            src/serialterminal/serialscriptreader.cpp \
            src/serialterminal/serialscriptexecutor.cpp \
            src/serialterminal/serialcommand.cpp \
            src/serialterminal/serialterminal.cpp \
            src/serialterminal/serialterminalscheduler.cpp \
            src/serialterminal/serialterminallineedit.cpp \
            src/serialterminal/smartserialreportfuture.cpp \
            src/serialterminal/serialcommandhistoryaction.cpp \
            src/serialterminal/serialcommanderrormessage.cpp \
            \
            src/manualscreen/iostatusview.cpp \
            src/manualscreen/diostatepushbutton.cpp \
            src/manualscreen/manualscreen.cpp \
            src/manualscreen/smartioreportfuture.cpp \
            src/manualscreen/manualscreenscheduler.cpp \
            src/manualscreen/analogoutputentrybox.cpp\
            src/manualscreen/pullupstatepushbutton.cpp \
            src/manualscreen/analogiorawstatepushbutton.cpp \
            src/manualscreen/diostatelabel.cpp \
            src/manualscreen/aliaschangedialog.cpp \
            \
            src/canterminal/canscriptreader.cpp \
            src/canterminal/canscriptexecutor.cpp \
            src/canterminal/cancommand.cpp \
            src/canterminal/canterminal.cpp \
            src/canterminal/canterminalscheduler.cpp \
            src/canterminal/canterminallineedit.cpp \
            src/canterminal/smartcanreportfuture.cpp \
            src/canterminal/cancommandhistoryaction.cpp \
            src/globallogger.cpp

HEADERS  += include/mainwindow.h\
            include/arduinopcicons.h \
            include/programloadingwidget.h \
            include/arduinopcstrings.h \
            include/configurationfilereader.h \
            include/custompushbutton.h \
            include/customaction.h \
            include/customcombobox.h \
            include/globallogger.h \
            include/arduinoscheduler.h \
            \
            include/serialterminal/serialscriptreader.h \
            include/serialterminal/serialscriptexecutor.h \
            include/serialterminal/serialcommand.h \
            include/serialterminal/serialterminal.h \
            include/serialterminal/serialterminalscheduler.h \
            include/serialterminal/serialterminallineedit.h \
            include/serialterminal/smartserialreportfuture.h \
            include/serialterminal/serialcommandhistoryaction.h \
            include/serialterminal/serialcommanderrormessage.h \
            \
            include/manualscreen/iostatusview.h \
            include/manualscreen/diostatepushbutton.h \
            include/manualscreen/manualscreen.h \
            include/manualscreen/smartioreportfuture.h \
            include/manualscreen/manualscreenscheduler.h \
            include/manualscreen/analogoutputentrybox.h\
            include/manualscreen/pullupstatepushbutton.h \
            include/manualscreen/analogiorawstatepushbutton.h \
            include/manualscreen/diostatelabel.h \
            include/manualscreen/aliaschangedialog.h \
            \
            include/canterminal/canscriptreader.h \
            include/canterminal/canscriptexecutor.h \
            include/canterminal/cancommand.h \
            include/canterminal/canterminal.h \
            include/canterminal/canterminalscheduler.h \
            include/canterminal/canterminallineedit.h \
            include/canterminal/smartcanreportfuture.h \
            include/canterminal/cancommandhistoryaction.h

FORMS    += forms/mainwindow.ui \
            forms/programloadingwidget.ui \
            forms/serialterminal.ui \
            forms/manualscreen.ui \
            forms/canterminal.ui \
            forms/aliaschangedialog.ui

RESOURCES += resources/icons.qrc

if (win32) {
    DISTFILES += resources/arduinopc.rc
}
