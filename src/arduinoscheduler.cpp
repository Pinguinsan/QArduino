#include "arduinoscheduler.h"

std::shared_ptr<Arduino> ArduinoScheduler::arduinoByScreenIndex(int screenIndex) const
{
    using namespace ArduinoPCStrings;
    if (this->m_arduinos.find(screenIndex) == this->m_arduinos.end()) {
        throw std::runtime_error(ARDUINO_BY_SCREEN_INDEX_INVALID_INDEX_STRING + std::to_string(screenIndex));
    } else {
        return this->m_arduinos.at(screenIndex);
    }
}

void ArduinoScheduler::removeArduino(int screenIndex)
{
    using namespace ArduinoPCStrings;
    if (this->m_arduinos.find(screenIndex) == this->m_arduinos.end()) {
        throw std::runtime_error(REMOVE_SCREEN_ARDUINO_INVALID_INDEX_STRING + std::to_string(screenIndex));
    } else {
        this->m_arduinos.erase(screenIndex);
    }
}

int ArduinoScheduler::numberOfArduinos() const
{
    return this->m_arduinos.size();
}

void ArduinoScheduler::addArduino(int screenIndex, std::shared_ptr<Arduino> arduino, std::shared_ptr<GlobalLogger> globalLogger)
{
    using namespace ArduinoPCStrings;
    (void)globalLogger;
    if (this->m_arduinos.find(screenIndex) != this->m_arduinos.end()) {
        throw std::runtime_error(ADD_ARDUINO_ALREADY_EXISTS_STRING + std::to_string(screenIndex));
    } else if (!arduino){
        std::cout << NULL_ARDUINO_PASSED_TO_ADD_ARDUINO_STRING << std::endl;
    } else {
        this->m_arduinos.emplace(std::make_pair(screenIndex, arduino));
    }
}

void ArduinoScheduler::addArduino(int screenIndex, std::shared_ptr<SerialPort> serialPort, std::shared_ptr<GlobalLogger> globalLogger)
{
    using namespace ArduinoPCStrings;
    (void)globalLogger;
    if (this->m_arduinos.find(screenIndex) != this->m_arduinos.end()) {
        throw std::runtime_error(ADD_ARDUINO_ALREADY_EXISTS_STRING + std::to_string(screenIndex));
    } else {
        if (Arduino::isBluetooth(serialPort->portName())) {
            serialPort->setTimeout(Arduino::SERIAL_TIMEOUT*Arduino::bluetoothSendDelayMultiplier);
            serialPort->setRetryCount(Arduino::BLUETOOTH_RETRY_COUNT);
        } else {
            serialPort->setTimeout(Arduino::SERIAL_TIMEOUT);
        }
        this->m_arduinos.emplace(std::make_pair(screenIndex, Arduino::makeArduino(serialPort)));
    }
}

std::vector<std::shared_ptr<Arduino>> ArduinoScheduler::arduinos() const
{
    std::vector<std::shared_ptr<Arduino>> returnVector;
    for (auto &it : this->m_arduinos) {
        returnVector.emplace_back(it.second);
    }
    return returnVector;
}
