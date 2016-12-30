#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort/QtSerialPort>
#include <unordered_map>
#include <string>
#include <tuple>
#include "gpio.h"

class Arduino
{
public:
    Arduino(std::unique_ptr<SerialPort> serialPort);
protected:
    std::unordered_map<std::string, GPIO> _gpio;
    std::unique_ptr<SerialPort> _serialPort;

};

#endif // DIGITALOUTPUT_H
