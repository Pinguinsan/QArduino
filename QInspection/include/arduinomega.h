#ifndef ARDUINO_H
#define ARDUINO_H

#include <unordered_map>
#include <string>
#include <memory>
#include <queue>
#include <tuple>

enum LogicState { LOW=0x0, HIGH=0x1 };
enum class WriteStatus { WRITE_SUCCESSFUL, WRITE_FAILURE };
enum class IOType { DIGITAL_INPUT, DIGITAL_OUTPUT, ANALOG_INPUT, ANALOG_OUTPUT };
enum class AnalogPin { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15 };

class ArduinoMega : public QObject
{
    Q_OBJECT

public:
    ArduinoMega(std::unique_ptr<QSerialPort> serialPort);
    virtual LogicState digitalRead(int pinNumber);
    virtual LogicState digitalRead(AnalogPin pinNumber);

    virtual double analogRead(AnalogPin pinNumber);

    virtual WriteStatus digitalWrite(int pinNumber, LogicState logicState);
    virtual WriteStatus digitalWrite(AnalogPin pinNumber, LogicState logicState);

    virtual WriteStatus analogWrite(AnalogPin pinNumber, double value);

    virtual std::string readCanMessage();
    virtual void queueCanMessage(uint32_t id, uint8_t frame, uint8_t length, CanDataPacket data);
    virtual void flushCanBuffer();

    static const std::string CAN_MESSAGE_WRITE_HEADER;
    static const std::string CAN_MESSAGE_READ_HEADER;
    static const std::string DIGITAL_READ_HEADER;
    static const std::string ANALOG_READ_HEADER;
    static const std::string DIGITAL_WRITE_HEADER;
    static const std::string ANALOG_WRITE_HEADER;
    static const int SERIAL_TIMEOUT;
protected:
    std::unique_ptr<SerialPort> _serialPort;
    std::queue<CanMessage> _canBuffer;

public slots:
    void onSerialDataReady();
};

#endif // ARDUINOMEGA_H
