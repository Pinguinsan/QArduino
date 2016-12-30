#ifndef ARDUINOPC_SERIALTERMINALSCHEDULER_H
#define ARDUINOPC_SERIALTERMINALSCHEDULER_H


#include <QDesktopWidget>
#include <memory>
#include <map>
#include <mutex>

#include "serialterminal/serialterminal.h"

#include "arduinopcstrings.h"
#include "globallogger.h"

class SerialTerminal;

class SerialTerminalScheduler
{
public:
    std::shared_ptr<SerialTerminal> serialTerminalByScreenIndex(int screenIndex) const;
    void removeSerialTerminal(int screenIndex);
    int numberOfScreens() const;
    void addSerialTerminal(int screenIndex,
                           std::shared_ptr<ArduinoPCIcons> apciPtr,
                           std::shared_ptr<QDesktopWidget> qDesktopWidget,
                           std::shared_ptr<GlobalLogger> globalLogger,
                           std::shared_ptr<Arduino> arduino,
                           QWidget *parent = 0);
    std::vector<std::shared_ptr<SerialTerminal>> serialTerminals() const;
    static std::shared_ptr<std::mutex> serialTerminalMutexByScreenIndex(unsigned int screenIndex);

private:
    std::map<int, std::shared_ptr<SerialTerminal>> m_serialTerminals;
    static std::map<int, std::shared_ptr<std::mutex>> s_serialTerminalMutexes;
};

#endif //ARDUINOPC_SerialTerminalSCHEDULER_H
