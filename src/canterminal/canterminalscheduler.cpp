#include "canterminal/canterminalscheduler.h"

std::map<int, std::shared_ptr<std::mutex>> CanTerminalScheduler::s_canTerminalMutexes;

std::shared_ptr<CanTerminal> CanTerminalScheduler::canTerminalByScreenIndex(int screenIndex) const
{
    using namespace ArduinoPCStrings;
    if (this->m_canTerminals.find(screenIndex) == this->m_canTerminals.end()) {
        throw std::runtime_error(CAN_TERMINAL_BY_SCREEN_INDEX_INVALID_INDEX_STRING+ std::to_string(screenIndex));
    } else {
        return this->m_canTerminals.at(screenIndex);
    }
}

void CanTerminalScheduler::removeCanTerminal(int screenIndex)
{
    using namespace ArduinoPCStrings;
    if (this->m_canTerminals.find(screenIndex) == this->m_canTerminals.end()) {
        throw std::runtime_error(REMOVE_SCREEN_CAN_TERMINAL_INVALID_INDEX_STRING + std::to_string(screenIndex));
    } else {
        this->m_canTerminals.erase(screenIndex);
    }
}

int CanTerminalScheduler::numberOfScreens() const
{
    return this->m_canTerminals.size();
}

void CanTerminalScheduler::addCanTerminal(int screenIndex,
                                          std::shared_ptr<ArduinoPCIcons> apciPtr,
                                          std::shared_ptr<QDesktopWidget> qDesktopWidget,
                                          std::shared_ptr<GlobalLogger> globalLogger,
                                          std::shared_ptr<Arduino> arduino,
                                          QWidget *parent)
{
    using namespace ArduinoPCStrings;
    if (this->m_canTerminals.find(screenIndex) != this->m_canTerminals.end()) {
        throw std::runtime_error(ADD_REPORT_CAN_TERMINAL_INDEX_ALREADY_EXISTS_STRING + std::to_string(screenIndex));
    } else {
        this->m_canTerminals.emplace(screenIndex, std::make_shared<CanTerminal>(screenIndex,
                                                                                apciPtr,
                                                                                qDesktopWidget,
                                                                                globalLogger,
                                                                                arduino,
                                                                                parent));
        CanTerminalScheduler::s_canTerminalMutexes.emplace(screenIndex, std::make_shared<std::mutex>());
    }
}

std::vector<std::shared_ptr<CanTerminal>> CanTerminalScheduler::canTerminals() const
{
    std::vector<std::shared_ptr<CanTerminal>> returnVector;
    for (auto &it : this->m_canTerminals) {
        returnVector.emplace_back(it.second);
    }
    return returnVector;
}

std::shared_ptr<std::mutex> CanTerminalScheduler::canTerminalMutexByScreenIndex(unsigned int screenIndex)
{
    using namespace ArduinoPCStrings;
    if (screenIndex > CanTerminalScheduler::s_canTerminalMutexes.size()-1) {
        throw std::runtime_error(FLAGGED_CAN_TERMINAL_MUTEX_SCREEN_INDEX_INVALID_STRING + std::to_string(screenIndex));
    } else {
        return CanTerminalScheduler::s_canTerminalMutexes.at(screenIndex);
    }
}
