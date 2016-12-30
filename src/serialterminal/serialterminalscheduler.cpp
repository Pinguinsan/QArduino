#include "serialterminal/serialterminalscheduler.h"

std::map<int, std::shared_ptr<std::mutex>> SerialTerminalScheduler::s_serialTerminalMutexes;

std::shared_ptr<SerialTerminal> SerialTerminalScheduler::serialTerminalByScreenIndex(int screenIndex) const
{
    using namespace ArduinoPCStrings;
    if (this->m_serialTerminals.find(screenIndex) == this->m_serialTerminals.end()) {
        throw std::runtime_error(SERIAL_TERMINAL_BY_SCREEN_INDEX_INVALID_INDEX_STRING + std::to_string(screenIndex));
    } else {
        return this->m_serialTerminals.at(screenIndex);
    }
}

void SerialTerminalScheduler::removeSerialTerminal(int screenIndex)
{
    using namespace ArduinoPCStrings;
    if (this->m_serialTerminals.find(screenIndex) == this->m_serialTerminals.end()) {
        throw std::runtime_error(REMOVE_SCREEN_SERIAL_TERMINAL_INVALID_INDEX_STRING + std::to_string(screenIndex));
    } else {
        this->m_serialTerminals.erase(screenIndex);
    }
}

int SerialTerminalScheduler::numberOfScreens() const
{
    return this->m_serialTerminals.size();
}

void SerialTerminalScheduler::addSerialTerminal(int screenIndex,
                                                std::shared_ptr<ArduinoPCIcons> apciPtr,
                                                std::shared_ptr<QDesktopWidget> qDesktopWidget,
                                                std::shared_ptr<GlobalLogger> globalLogger,
                                                std::shared_ptr<Arduino> arduino,
                                                QWidget *parent)
{
    using namespace ArduinoPCStrings;
    if (this->m_serialTerminals.find(screenIndex) != this->m_serialTerminals.end()) {
        throw std::runtime_error(ADD_REPORT_SERIAL_TERMINAL_INDEX_ALREADY_EXISTS_STRING + std::to_string(screenIndex));
    } else {
        this->m_serialTerminals.emplace(std::make_pair(screenIndex, std::make_shared<SerialTerminal>(screenIndex,
                                                                                                     apciPtr,
                                                                                                     qDesktopWidget,
                                                                                                     globalLogger,
                                                                                                     arduino,
                                                                                                     parent)));
        SerialTerminalScheduler::s_serialTerminalMutexes.emplace(screenIndex, std::make_shared<std::mutex>());
    }
}

std::vector<std::shared_ptr<SerialTerminal>> SerialTerminalScheduler::serialTerminals() const
{
    std::vector<std::shared_ptr<SerialTerminal>> returnVector;
    for (auto &it : this->m_serialTerminals) {
        returnVector.emplace_back(it.second);
    }
    return returnVector;
}

std::shared_ptr<std::mutex> SerialTerminalScheduler::serialTerminalMutexByScreenIndex(unsigned int screenIndex)
{
    using namespace ArduinoPCStrings;
    if (screenIndex > SerialTerminalScheduler::s_serialTerminalMutexes.size()-1) {
        throw std::runtime_error(FLAGGED_SERIAL_TERMINAL_MUTEX_SCREEN_INDEX_INVALID_STRING + std::to_string(screenIndex));
    } else {
        return SerialTerminalScheduler::s_serialTerminalMutexes.at(screenIndex);
    }
}
