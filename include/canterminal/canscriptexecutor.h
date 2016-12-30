#ifndef ARDUINOPC_CANSCRIPTEXECUTOR_H
#define ARDUINOPC_CANSCRIPTEXECUTOR_H

#include <QApplication>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <memory>
#include <functional>
#include <cstdlib>
#include <serialport.h>
#include <generalutilities.h>

#include "mainwindow.h"
#include "arduinopcstrings.h"
#include "canterminal/canscriptreader.h"
#include "serialterminal/serialcommand.h"

class CanTerminal;

class CanScriptExecutor
{
public:
    CanScriptExecutor(const std::string &scriptFilePath);
    void setScriptFilePath(const std::string &scriptFilePath);
    std::shared_ptr<CanScriptReader> scriptReader() const;

    void execute(CanTerminal *canTerminal, std::shared_ptr<Arduino> arduino,
                 const std::function<void(CanTerminal *, const std::string &)> &printRxResult,
                 const std::function<void(CanTerminal *, const std::string &)> &printTxResult,
                 const std::function<void(CanTerminal *, DelayType, int)> &printDelayResult,
                 const std::function<void(CanTerminal *, FlushType)> &printFlushResult,
                 const std::function<void(CanTerminal *, LoopType, int, int)> &printLoopResult);
private:
    std::shared_ptr<CanScriptReader> m_scriptReader;

    void doLoop( CanTerminal *canTerminal, std::shared_ptr<Arduino> arduino,
                 const std::function<void(CanTerminal *, const std::string &)> &printRxResult,
                 const std::function<void(CanTerminal *, const std::string &)> &printTxResult,
                 const std::function<void(CanTerminal *, DelayType, int)> &printDelayResult,
                 const std::function<void(CanTerminal *, FlushType)> &printFlushResult,
                 const std::function<void(CanTerminal *, LoopType, int, int)> &printLoopResult,
                 const std::vector<CanCommand> &commands,
                 int loopCount);

    void delaySecondsWithUpdate(int howLong, CanTerminal *canTerminal);
    void delayMillisecondsWithUpdate(int howLong, CanTerminal *canTerminal);
    void delayMicrosecondsWithUpdate(int howLong, CanTerminal *canTerminal);
};




#endif //ARDUINOPC_CANSCRIPTEXECUTOR_H
