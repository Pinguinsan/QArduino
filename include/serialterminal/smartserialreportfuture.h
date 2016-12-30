#ifndef ARDUINOPC_SMARTSERIALREPORTFUTURE_H
#define ARDUINOPC_SMARTSERIALREPORTFUTURE_H

#include <QMainWindow>
#include <future>
#include <string>
#include <memory>
#include <functional>

#include "arduinopcstrings.h"
#include "serialterminal/serialterminal.h"
#include "serialterminal/smartserialreportfuture.h"

class SerialTerminal;

class SmartSerialReportFuture
{
public:
    SmartSerialReportFuture(SerialTerminal *serialTerminal,
                            const std::function<SerialReport(SerialTerminal*)> &function);

    void setFunction(const std::function<SerialReport(SerialTerminal *)> &func);
    std::function<SerialReport(SerialTerminal*)> function() const;
    bool isCompleted();
    bool isStarted() const;

    void stop();
    void start();
    SerialReport result();

private:
    SerialTerminal *m_serialTerminalArg;
    std::function<SerialReport(SerialTerminal*)> m_function;
    std::future<SerialReport> m_future;
    bool m_isStarted;
    bool m_isCompleted;
};

#endif //ARDUINOPC_SMARTSERIALREPORTFUTURE_H
