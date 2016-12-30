#include "manualscreen/smartioreportfuture.h"

SmartIOReportFuture::SmartIOReportFuture(ManualScreen *manualScreen,
                                         const std::function<IOReport(ManualScreen *)> &function) :
    m_manualScreenArg{manualScreen},
    m_function{function},
    m_isStarted{false},
    m_isCompleted{false}
{

}

bool SmartIOReportFuture::isStarted() const
{
    return this->m_isStarted;
}

bool SmartIOReportFuture::isCompleted()
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
        //throw std::runtime_error(IO_REPORT_FUTURE_NOT_STARTED_ERROR_STRING);
    }
}

std::function<IOReport(ManualScreen*)> SmartIOReportFuture::function() const
{
    return this->m_function;
}

void SmartIOReportFuture::setFunction(const std::function<IOReport(ManualScreen *)> &function)
{
    this->m_function = function;
}

void SmartIOReportFuture::start()
{
    using namespace ArduinoPCStrings;
    if (this->m_isStarted) {
        this->m_future = std::async(std::launch::async, this->m_function, this->m_manualScreenArg);
        this->m_isStarted = true;
        this->m_isCompleted = false;
        //throw std::runtime_error(IO_REPORT_FUTURE_ALREADY_STARTED_ERROR_STRING);
    } else {
        this->m_future = std::async(std::launch::async, this->m_function, this->m_manualScreenArg);
        this->m_isStarted = true;
        this->m_isCompleted = false;
    }
}

IOReport SmartIOReportFuture::result()
{
    using namespace ArduinoPCStrings;
    if (this->m_isCompleted) {
        try {
            return this->m_future.get();
        } catch (std::exception &e) {
            throw e;
        }
    } else {
        throw std::runtime_error(IO_REPORT_FUTURE_NOT_COMPLETED_ERROR_STRING);
    }
}

void SmartIOReportFuture::stop()
{
    this->m_isStarted = false;
    this->m_isCompleted = false;
}

