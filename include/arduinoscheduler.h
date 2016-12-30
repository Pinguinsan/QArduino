#ifndef ARDUINOPC_ARDUINOSCHEDULER_H
#define ARDUINOPC_ARDUINOSCHEDULER_H

#include <memory>
#include <map>
#include <serialport.h>
#include <arduino.h>

#include "globallogger.h"
#include "arduinopcstrings.h"
#include "configurationfilereader.h"

class MainWindow;
class Arduino;

class ArduinoScheduler
{
public:
    std::shared_ptr<Arduino> arduinoByScreenIndex(int screenIndex) const;
    void removeArduino(int screenIndex);
    int numberOfArduinos() const;
    std::vector<std::shared_ptr<Arduino>> arduinos() const;
    void addArduino(int screenIndex, std::shared_ptr<Arduino> arduino, std::shared_ptr<GlobalLogger> globalLogger = nullptr);
    void addArduino(int screenIndex, std::shared_ptr<SerialPort> serialPort, std::shared_ptr<GlobalLogger> globalLogger = nullptr);

private:
    std::map<int, std::shared_ptr<Arduino>> m_arduinos;
};

#endif //ARDUINOPC_ARDUINOSCHEDULER_H
