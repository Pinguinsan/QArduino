#ifndef ARDUINOPC_SERIALSCRIPTEXECUTOR_H
#define ARDUINOPC_SERIALSCRIPTEXECUTOR_H

#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <memory>
#include <functional>
#include <cstdlib>
#include <serialport.h>
#include <generalutilities.h>
#include <mathutilities.h>

#include "mainwindow.h"
#include "arduinopcstrings.h"
#include "serialterminal/serialscriptreader.h"
#include "serialterminal/serialcommanderrormessage.h"

class SerialTerminal;

class SerialScriptExecutor
{
public:
    SerialScriptExecutor(const std::string &scriptFilePath);
    void setScriptFilePath(const std::string &scriptFilePath);
    std::shared_ptr<SerialScriptReader> scriptReader() const;

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }
    QString toQString(const QString &qstr) { return qstr; }

    void execute(SerialTerminal *serialTerminal, std::shared_ptr<Arduino> arduino,
                 const std::function<void(SerialTerminal *, const std::string &)> &printRxResult,
                 const std::function<void(SerialTerminal *, const std::string &)> &printTxResult,
                 const std::function<void(SerialTerminal *, DelayType, int)> &printDelayResult,
                 const std::function<void(SerialTerminal *, FlushType)> &printFlushResult,
                 const std::function<void(SerialTerminal *, LoopType, int, int)> &printLoopResult);
private:
    std::shared_ptr<SerialScriptReader> m_scriptReader;

    void doLoop( SerialTerminal *serialTerminal, std::shared_ptr<Arduino> arduino,
                 const std::function<void(SerialTerminal *, const std::string &)> &printRxResult,
                 const std::function<void(SerialTerminal *, const std::string &)> &printTxResult,
                 const std::function<void(SerialTerminal *, DelayType, int)> &printDelayResult,
                 const std::function<void(SerialTerminal *, FlushType)> &printFlushResult,
                 const std::function<void(SerialTerminal *, LoopType, int, int)> &printLoopResult,
                 const std::vector<SerialCommand> &commands,
                 int loopCount);

    void delaySecondsWithUpdate(int howLong, SerialTerminal *serialTerminal);
    void delayMillisecondsWithUpdate(int howLong, SerialTerminal *serialTerminal);
    void delayMicrosecondsWithUpdate(int howLong, SerialTerminal *serialTerminal);
};




#endif //ARDUINOPC_SERIALSCRIPTEXECUTOR_H
