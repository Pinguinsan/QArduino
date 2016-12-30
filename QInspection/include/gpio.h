#ifndef GPIO_H
#define GPIO_H

enum LogicState { LOW=0x0, HIGH=0x1 };
enum class WriteStatus { WRITE_SUCCESSFUL, WRITE_FAILURE };
enum class IOType { DIGITAL_INPUT, DIGITAL_OUTPUT, ANALOG_INPUT, ANALOG_OUTPUT };

class GPIO
{
public:
    GPIO(IOType ioType, std::vector<IOTypes> availableIOTypes);
    void changeIOType(IOType ioType);
    int pinNumber();
    std::vector<IOType> availableIOTypes;
    virtual IOType currentIOType();
    virtual LogicState digitalRead();
    virtual double analogRead();
    virtual WriteStatus digitalWrite(LogicState logicState);
    virtual WriteStatus analogWrite(double value);

private:
    int _pinNumber;
    std::vector<IOType> _availableIOTypes;
};

#endif // GPIO_H
