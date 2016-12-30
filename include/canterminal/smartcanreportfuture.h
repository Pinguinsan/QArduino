#ifndef ARDUINOPC_SMARTCANREPORTFUTURE_H
#define ARDUINOPC_SMARTCANREPORTFUTURE_H

#include <QMainWindow>
#include <future>
#include <string>
#include <memory>
#include <functional>
#include <arduino.h>

#include "arduinopcstrings.h"
#include "canterminal/canterminal.h"

class CanReport;
class CanTerminal;

class SmartCanReportFuture
{
public:
    SmartCanReportFuture(CanTerminal *canReport,
                         const std::function<CanReport(CanTerminal *)> &function);

    void setFunction(const std::function<CanReport(CanTerminal *)> &func);
    std::function<CanReport(CanTerminal *)> function() const;
    bool isCompleted();
    bool isStarted() const;

    void stop();
    void start();
    CanReport result();

private:
    CanTerminal *m_canReportArg;
    std::shared_ptr<Arduino> m_arduino;
    std::function<CanReport(CanTerminal *)> m_function;
    std::future<CanReport> m_future;
    bool m_isStarted;
    bool m_isCompleted;
};

#endif //ARDUINOPC_SMARTCANREPORTFUTURE_H
