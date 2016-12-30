#include "serialterminal/smartserialreportfuture.h"

SmartSerialReportFuture::SmartSerialReportFuture(SerialTerminal *serialTeminal,
                                                 const std::function<SerialReport(SerialTerminal *)> &function) :
    m_serialTerminalArg{serialTeminal},
    m_function{function},
    m_isStarted{false},
    m_isCompleted{false}
{

}

bool SmartSerialReportFuture::isStarted() const
{
    return this->m_isStarted;
}

bool SmartSerialReportFuture::isCompleted()
{
    using namespace ArduinoPCStrings;
    if (this->m_isStarted) {
        this->m_isCompleted = (this->m_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready);
        if (this->m_isCompleted) {
            this->m_isStarted = false;
        }
        return this->m_isCompleted;
    } else {
        return false;
        //throw std::runtime_error(SERIAL_TERMINAL_FUTURE_NOT_STARTED_ERROR_STRING);
    }
}

std::function<SerialReport(SerialTerminal *)> SmartSerialReportFuture::function() const
{
    return this->m_function;
}

void SmartSerialReportFuture::setFunction(const std::function<SerialReport(SerialTerminal *)> &function)
{
    this->m_function = function;
}

void SmartSerialReportFuture::start()
{
    using namespace ArduinoPCStrings;
    if (this->m_isStarted) {
        this->m_future = std::async(std::launch::async, this->m_function, this->m_serialTerminalArg);
        this->m_isStarted = true;
        this->m_isCompleted = false;
        //throw std::runtime_error(SERIAL_TERMINAL_FUTURE_ALREADY_STARTED_ERROR_STRING);
    } else {
        this->m_future = std::async(std::launch::async, this->m_function, this->m_serialTerminalArg);
        this->m_isStarted = true;
        this->m_isCompleted = false;
    }
}

SerialReport SmartSerialReportFuture::result()
{
    using namespace ArduinoPCStrings;
    if (this->m_isCompleted) {
        try {
            return this->m_future.get();
        } catch (std::exception &e) {
            throw e;
        }
    } else {
        throw std::runtime_error(SERIAL_TERMINAL_FUTURE_NOT_COMPLETED_ERROR_STRING);
    }
}

void SmartSerialReportFuture::stop()
{
    this->m_isStarted = false;
    this->m_isCompleted = false;
}

