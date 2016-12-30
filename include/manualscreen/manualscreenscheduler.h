#ifndef ARDUINOPC_MANUALSCREENSCHEDULER_H
#define ARDUINOPC_MANUALSCREENSCHEDULER_H

#include <QDesktopWidget>
#include <memory>
#include <map>

#include "arduinopcstrings.h"
#include "arduinopcicons.h"
#include "globallogger.h"

#include <arduino.h>
#include "manualscreen/manualscreen.h"

class ManualScreen;

class ManualScreenScheduler
{
public:
    std::shared_ptr<ManualScreen> manualScreenByScreenIndex(int screenIndex) const;
    void removeManualScreen(int screenIndex);
    int numberOfScreens() const;
    void addManualScreen(int screenIndex,
                         std::shared_ptr<ArduinoPCIcons> apciPtr,
                         std::shared_ptr<QDesktopWidget> qDesktopWidget,
                         std::shared_ptr<GlobalLogger> globalLogger,
                         std::shared_ptr<Arduino> arduino,
                         QWidget *parent = 0);
    std::vector<std::shared_ptr<ManualScreen>> manualScreens() const;

private:
    std::map<int, std::shared_ptr<ManualScreen>> m_manualScreens;
};

#endif //ARDUINOPC_MANUALSCREENSCHEDULER_H
