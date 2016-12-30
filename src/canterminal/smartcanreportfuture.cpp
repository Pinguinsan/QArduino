#include "canterminal/smartcanreportfuture.h"

SmartCanReportFuture::SmartCanReportFuture(CanTerminal *canReport,
                                           const std::function<CanReport(CanTerminal *)> &function) :
    m_canReportArg{canReport},
    m_function{function},
    m_isStarted{false},
    m_isCompleted{false}
{

}

bool SmartCanReportFuture::isStarted() const
{
    return this->m_isStarted;
}

bool SmartCanReportFuture::isCompleted()
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
        //throw std::runtime_error(CAN_TERMINAL_FUTURE_NOT_STARTED_ERROR_STRING);
    }
}

std::function<CanReport(CanTerminal*)> SmartCanReportFuture::function() const
{
    return this->m_function;
}

void SmartCanReportFuture::setFunction(const std::function<CanReport(CanTerminal *)> &function)
{
    this->m_function = function;
}

void SmartCanReportFuture::start()
{
    using namespace ArduinoPCStrings;
    if (this->m_isStarted) {
        this->m_future = std::async(std::launch::async, this->m_function, this->m_canReportArg);
        this->m_isStarted = true;
        this->m_isCompleted = false;
        //throw std::runtime_error(CAN_TERMINAL_FUTURE_ALREADY_STARTED_ERROR_STRING);
    } else {
        this->m_future = std::async(std::launch::async, this->m_function, this->m_canReportArg);
        this->m_isStarted = true;
        this->m_isCompleted = false;
    }
}

CanReport SmartCanReportFuture::result()
{
    using namespace ArduinoPCStrings;
    if (this->m_isCompleted) {
        try {
            return this->m_future.get();
        } catch (std::exception &e) {
            throw e;
        }
    } else {
        throw std::runtime_error(CAN_TERMINAL_FUTURE_NOT_COMPLETED_ERROR_STRING);
    }
}

void SmartCanReportFuture::stop()
{
    this->m_isStarted = false;
    this->m_isCompleted = false;
}

