#include "globallogger.h"

const size_t GlobalLogger::LOG_BUFFER_MAX_SIZE{100};
std::unique_ptr<std::mutex> GlobalLogger::ioMutex{std::make_unique<std::mutex>()};

GlobalLogger::GlobalLogger() :
    m_log{std::make_unique<std::deque<std::string>>()},
    m_temporaryBuffer{""}
{

}

void GlobalLogger::logString(const std::string &str)
{
    std::unique_lock<std::mutex> ioLock{*GlobalLogger::ioMutex};
    this->m_log->emplace_back(str);
    if (this->m_log->size() >= GlobalLogger::LOG_BUFFER_MAX_SIZE) {
        this->m_log->pop_front();
    }
}

bool GlobalLogger::logMessageAvailable() const
{
    return (this->m_log->size() != 0);
}

std::string GlobalLogger::nextMessage()
{
    std::unique_lock<std::mutex> ioLock{*GlobalLogger::ioMutex};
    std::string returnString{this->m_log->front()};
    this->m_log->pop_front();
    return returnString;
}
                                                                                                     
