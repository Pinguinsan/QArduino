#ifndef ARDUINOPC_GLOBALLOGGER_H
#define ARDUINOPC_GLOBALLOGGER_H

#include <memory>
#include <deque>
#include <string>
#include <mutex>
#include <generalutilities.h>


class GlobalLogger
{
public:

    GlobalLogger();
    void logString(const std::string &str);
    bool logMessageAvailable() const;
    std::string nextMessage();
    
    template<typename T>                                             
    GlobalLogger &operator<<(const T& rhs)
    {
        using namespace GeneralUtilities;
        std::string toLog{toString(rhs)};
        if ((toLog == tEndl() || (toLog == "\n")) || (toLog == "\r\n")) {
            logString(this->m_temporaryBuffer);
            this->m_temporaryBuffer = "";
        } else {
            this->m_temporaryBuffer.append(toLog);
        }
        return *this;
    }

private:
    std::unique_ptr<std::deque<std::string>> m_log;
    std::string m_temporaryBuffer;
    static const size_t LOG_BUFFER_MAX_SIZE;
    static std::unique_ptr<std::mutex> ioMutex;
};

#endif //ARDUINOPC_GLOBALLOGGER_H
