#ifndef ARDUINOPC_MAINWINDOW_H
#define ARDUINOPC_MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QComboBox>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <functional>
#include <string>
#include <set>
#include <memory>
#include <future>
#include <serialport.h>
#include <mathutilities.h>
#include <generalutilities.h>

#include "programloadingwidget.h"
#include <arduino.h>
#include "arduinoscheduler.h"

#include "manualscreen/manualscreen.h"
#include "manualscreen/manualscreenscheduler.h"

#include "serialterminal/serialterminal.h"
#include "serialterminal/serialterminalscheduler.h"

#include "canterminal/canterminal.h"
#include "canterminal/canterminalscheduler.h"

#include "customaction.h"
#include "arduinopcstrings.h"
#include "arduinopcicons.h"
#include "globallogger.h"

namespace Ui
{
    class MainWindow;
    class ManualScreen;
    class SerialTerminal;
    class CanTerminal;
    class ProgramLoadingWidget;
}

class Arduino;
class ArduinoScheduler;

class ManualScreen;
class ManualScreenScheduler;

class SerialTerminal;
class SerialTerminalScheduler;

class CanTerminal;
class CanTerminalScheduler;

class ProgramLoadingWidget;
class GlobalLogger;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::shared_ptr<ArduinoPCIcons> apciPtr,
                        std::shared_ptr<QDesktopWidget> qDesktopWidget,
                        std::shared_ptr<GlobalLogger> globalLogger = nullptr,
                        QWidget *parent = 0);

    ~MainWindow();
    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }
    QString toQString(const QString &qstr) { return qstr; }

    void bindQDesktopWidget(const std::shared_ptr<QDesktopWidget> &qDesktopWidget);
    void bindArduinoPCIcons(const std::shared_ptr<ArduinoPCIcons> &apciPtr);

    QApplication *application();
    std::shared_ptr<Arduino> arduinoPtrAtIndex(int index) const;
    void closeEvent(QCloseEvent *event);
    void initializeIO(std::vector<std::shared_ptr<SerialPort> > serialPort, std::shared_ptr<ProgramLoadingWidget> plw = nullptr);
    void centerAndFitWindow();
    int xPlacement() const;
    int yPlacement() const;
    static int TASKBAR_HEIGHT();
    static int nextAvailableArduinoSlot();
    static void forceUiRefresh();
    void bindGlobalLogger(std::shared_ptr<GlobalLogger> globalLogger);
    static int numberOfAttachedArduinos();
signals:
    void okayToExecuteSerialTerminalScript(MainWindow *mainWindow, std::shared_ptr<SerialPort> serialPort);

private slots:
    void onCustomActionTriggered(CustomActionType customActionType, int screenIndex, bool checked);
    void onManualScreenClosed(int screenIndex);
    void onSerialTerminalClosed(int screenIndex);
    void onCanTerminalClosed(int screenIndex);
    void cleanupSerialPorts();
    void checkForDisconnectedSerialPorts();
private:
    std::unique_ptr<Ui::MainWindow> m_uiPtr;
    std::shared_ptr<ArduinoPCIcons> m_apciPtr;
    std::shared_ptr<QDesktopWidget> m_qDesktopWidget;
    std::shared_ptr<GlobalLogger> m_globalLogger;
    std::unique_ptr<ArduinoScheduler> m_arduinoScheduler;
    std::unique_ptr<ManualScreenScheduler> m_manualScreenScheduler;
    std::unique_ptr<CanTerminalScheduler> m_canTerminalScheduler;
    std::unique_ptr<SerialTerminalScheduler> m_serialTerminalScheduler;
    std::unique_ptr<QTimer> m_disconnectedSerialPortTimer;
    std::unique_ptr<ConfigurationFileReader> m_configurationFileReader;
    std::set<int> m_lemonSerialPorts;
    int m_xPlacement;
    int m_yPlacement;

    std::multimap<int, CustomAction*> m_customActions;
    static std::set<int> s_attachedArduinoIndexes;

    bool isLemonSerialPort(int serialPortIndex);
    void calculateXYPlacement();
    void startManualScreenAutoUpdate(int screenIndex);
    void startSerialTerminalAutoUpdate(int screenIndex);
    void startCanTerminalAutoUpdate(int screenIndex);

    void assignIOTypesFromMap(std::shared_ptr<Arduino> arduino, const std::map<int, std::string> &ioTypesMap);
    void assignInitialStatesFromMap(std::shared_ptr<Arduino> arduino, const std::map<int, std::string> &initialStatesMap);

    static int s_numberOfAttachedArduinos;
    static const int s_TASKBAR_HEIGHT;
    static const int s_CHECK_DISCONNECTED_SERIAL_PORTS_INTERVAL;
    void initializeSingleIO(int serialPortIndex, std::shared_ptr<SerialPort> serialPort, std::shared_ptr<GlobalLogger> globalLogger = nullptr);
    void uninitializeSingleIO(int serialPortIndex, std::shared_ptr<GlobalLogger> globalLogger = nullptr);
    void applyConfigurationFile(std::shared_ptr<Arduino> arduino);
};

#endif //ARDUINOPC_MAINWINDOW_H
