#ifndef ARDUINOPC_SMARTIOREPORTFUTURE_H
#define ARDUINOPC_SMARTIOREPORTFUTURE_H

#include <QMainWindow>
#include <future>
#include <memory>
#include <functional>
#include <arduino.h>

#include "manualscreen/manualscreen.h"
#include "arduinopcstrings.h"

class ManualScreen;

class SmartIOReportFuture
{
public:
    SmartIOReportFuture(ManualScreen *manualScreen,
                        const std::function<IOReport(ManualScreen *)> &function);

    void setFunction(const std::function<IOReport(ManualScreen*)> &func);
    std::function<IOReport (ManualScreen *)> function() const;
    bool isCompleted();
    bool isStarted() const;

    void stop();
    void start();
    IOReport result();

private:
    ManualScreen *m_manualScreenArg;
    std::function<IOReport(ManualScreen*)> m_function;
    std::future<IOReport> m_future;
    bool m_isStarted;
    bool m_isCompleted;
};

#endif //ARDUINOPC_SMARTIOREPORTFUTURE_H
