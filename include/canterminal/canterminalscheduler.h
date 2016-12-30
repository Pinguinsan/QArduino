#ifndef ARDUINOPC_CANTERMINALSCHEDULER_H
#define ARDUINOPC_CANTERMINALSCHEDULER_H

#include <memory>
#include <map>
#include <arduino.h>

#include "canterminal/canterminal.h"
#include "arduinopcstrings.h"
#include "arduinopcicons.h"
#include "globallogger.h"

class CanTerminal;

class CanTerminalScheduler
{
public:
    std::shared_ptr<CanTerminal> canTerminalByScreenIndex(int screenIndex) const;
    void removeCanTerminal(int screenIndex);
    int numberOfScreens() const;
    void addCanTerminal(int screenIndex,
                        std::shared_ptr<ArduinoPCIcons> apciPtr,
                        std::shared_ptr<QDesktopWidget> qDesktopWidget,
                        std::shared_ptr<GlobalLogger> globalLogger,
                        std::shared_ptr<Arduino> arduino,
                        QWidget *parent = 0);
    std::vector<std::shared_ptr<CanTerminal>> canTerminals() const;
    static std::shared_ptr<std::mutex> canTerminalMutexByScreenIndex(unsigned int screenIndex);

private:
    std::map<int, std::shared_ptr<CanTerminal>> m_canTerminals;
    static std::map<int, std::shared_ptr<std::mutex>> s_canTerminalMutexes;
};

#endif //ARDUINOPC_CANTERMINALSCHEDULER_H
