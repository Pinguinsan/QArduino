#include "arduinomega.h"

const std::string ArduinoMega::CAN_MESSAGE_WRITE_HEADER{"cwrite"};
const std::string ArduinoMega::CAN_MESSAGE_READ_HEADER{"cread"};
const std::string ArduinoMega::DIGITAL_READ_HEADER{"dread"};
const std::string ArduinoMega::ANALOG_READ_HEADER{"aread"};
const std::string ArduinoMega::DIGITAL_WRITE_HEADER{"dwrite"};
const std::string ArduinoMega::DIGITAL_WRITE_HEADER{"awrite"};

ArduinoMega::ArduinoMega(std::unique_ptr<QSerialPort> serialPort) :
    _serialPort{std::move(serialPort)}
{
    //connect(this->_serialPort.get(), SIGNAL(readyToRead()), this, SLOT(onSerialDataReady()));
}

void ArduinoMega::onSerialDataReady()
{
    std::cout << this->_serialPort->popAndReturnCurrentBuffer() << std::endl;
}

virtual LogicState ArduinoMega::digitalRead(int pinNumber)
{
    using namespace GeneralUtilities;
    this->_serialPort->sendString(DIGITAL_READ_HEADER + ":" + toString(pinNumber));
    connect(this->_serialPort.get(), SIGNAL(readyToRead()), this, SLOT(onSerialDataReady()));
    auto startTime{std::chrono::high_resolution_clock::now()};
    auto endTime{std::chrono::high_resolution_clock::now()};
    auto elapsedTime = duration_cast<milliseconds>(endTime-startTime).count();
    std::string returnString{""};
    do {
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = duration_cast<milliseconds>(endTime-startTime).count();
        returnString = this->_serialPort->popAndReturnCurrentBuffer();
        if (returnString.find(DIGITAL_READ_HEADER) != std::string::npos) {
            if (returnString.substr(returnString.find(DIGITAL_READ_HEADER)) == "1") {
                return LogicState::HIGH;
            } else {
                return LogicState::LOW;
            }
        }
    } while (elapsedTime < SERIAL_TIMEOUT);
    throw std::runtime_error("Did not receive response from Arduino");
}

virtual LogicState ArduinoMega::digitalRead(AnalogPin pinNumber)
{
    using namespace GeneralUtilities;
}

virtual double ArduinoMega::analogRead(AnalogPin pinNumber)
{
    using namespace GeneralUtilities;
}

virtual WriteStatus ArduinoMega::digitalWrite(int pinNumber, LogicState logicState)
{
    using namespace GeneralUtilities;
}

virtual WriteStatus ArduinoMega::digitalWrite(AnalogPin pinNumber, LogicState logicState)
{
    using namespace GeneralUtilities;
}

virtual WriteStatus ArduinoMega::analogWrite(AnalogPin pinNumber, double value)
{
    using namespace GeneralUtilities;
}

virtual std::string ArduinoMega::readCanMessage()
{
    using namespace GeneralUtilities;
}

virtual void ArduinoMega::queueCanMessage(uint32_t id, uint8_t frame, uint8_t length, CanDataPacket data)
{
    this->_canBuffer.emplace(id, frame, length, data);
}

virtual void ArduinoMega::flushCanBuffer()
{
    using namespace GeneralUtilities;
    while (this->_canBuffer.size() != 0) {
        CanMessage msg{this->_canBuffer.front()};
        this->_canBuffer.pop();
        unsigned char dataPacket[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        msg.data().toBasicArray(dataPacket);
        std::string dataString{""};
        for (int i = 0; i < 8; i++) {
            dataString += toString(dataPacket[i]);
            if ((i + 1) != 8) {
                dataString += ",";
            }
        }
        this->_serialPort->sendString(CAN_MESSAGE_WRITE_HEADER + ":" + msg.id() + ":" + msg.frame() + ":" + msg.length() + ":" + dataString);
    }
}
