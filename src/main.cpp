#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QString>
#include <chrono>
#include <memory>
#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <generalutilities.h>
#include <systemcommand.h>
#include <serialport.h>
#include <mathutilities.h>
#include <arduino.h>

#include "arduinopcstrings.h"
#include "mainwindow.h"
#include "programloadingwidget.h"

template <typename T>
QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
QString toQString(const std::string &str) { return QString::fromStdString(str); }
QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }
QString toQString(const QString &qstr) { return qstr; }
void setProgressBar(QApplication *update, std::shared_ptr<ProgramLoadingWidget> plw);
bool isCommandLineIgnored(const std::string &portName, const std::vector<std::__cxx11::string> &ignoredSerialPorts);
bool isSwitch(const std::string &toCheck, const std::vector<const char *> &switches);
bool isSwitch(const std::string &toCheck, const std::vector<const char *> &switches);
bool isSwitch(const char *toCheck, const std::vector<std::string> &switches);
bool isSwitch(const char *toCheck, const std::vector<const char *> &switches);

static const std::vector<const char *> IGNORE_PORT_SWITCHES{"-i", "--i", "-ignore", "--ignore", "-ignore-port", "--ignore-port"};

static const int TRY_COUNT_HIGH_LIMIT{3};

int main(int argc, char *argv[])
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    using namespace MathUtilities;

    std::vector<std::string> ignoredSerialPorts;
    for (int i = 0; i < argc; i++) {
        if (isSwitch(static_cast<std::string>(argv[i]), IGNORE_PORT_SWITCHES)) {
            if ((i + 1) == argc) {
                std::cout << NO_SERIAL_PORT_NAME_SPECIFIED_STRING << std::endl;
                continue;
            } else {
                if (SerialPort::isValidSerialPortName(static_cast<std::string>(argv[i+1]))) {
                    ignoredSerialPorts.emplace_back(argv[i+1]);
                } else {
                    std::cout << INVALID_SERIAL_PORT_NAME_STRING << std::endl;
                    continue;
                }
            }
        }
    }

    std::unique_ptr<QApplication> qApplication{std::make_unique<QApplication>(argc, argv)};
    std::vector<std::string> serialPortNames{SerialPort::availableSerialPorts()};
    std::vector<std::shared_ptr<SerialPort>> serialPorts;
    std::shared_ptr<ArduinoPCIcons> apci{std::make_shared<ArduinoPCIcons>()};
    std::shared_ptr<QDesktopWidget> qDesktopWidget{std::make_shared<QDesktopWidget>()};
    serialPortNames.erase(std::remove_if(serialPortNames.begin(), serialPortNames.end(), [](const std::string &str) -> bool { return startsWith(str, "/dev/ttyS"); }), serialPortNames.end());
    if (serialPortNames.empty()) {
        std::unique_ptr<QMessageBox> errorBox{std::make_unique<QMessageBox>()};
        errorBox->setWindowTitle(toQString(static_cast<std::string>(NO_SERIAL_PORTS_FOUND_WINDOW_TITLE)));
        errorBox->setWindowIcon(apci->MAIN_WINDOW_ICON);
        errorBox->setText(toQString(static_cast<std::string>(NO_SERIAL_PORTS_FOUND_STRING)));
        errorBox->exec();
        exit(EXIT_FAILURE);
    }
    double step{static_cast<double>(Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT())/static_cast<double>(intExp(serialPortNames.size(), 2))};
    std::shared_ptr<GlobalLogger> globalLogger{std::make_shared<GlobalLogger>()};
    std::shared_ptr<ProgramLoadingWidget> plwuiPtr{std::make_shared<ProgramLoadingWidget>(apci, qDesktopWidget, step, globalLogger)};
    plwuiPtr->centerAndFitWindow();
    plwuiPtr->show();
    plwuiPtr->setProgressBarMinimumValue(0);
    plwuiPtr->setProgressBarMaximumValue(static_cast<double>(Arduino::BOOTLOADER_BOOT_TIME)*serialPortNames.size());
    //plwuiPtr->startAsyncUpdates();
    for (auto &it : serialPortNames) {
        if (startsWith(it, NATIVE_LINUX_SERIAL_PORT_NAME_BASE_STRING)) {
            continue;
        } else if (isCommandLineIgnored(it, ignoredSerialPorts)) {
            continue;
        }
        std::cout << DETECTED_SERIAL_PORT_STRING << std::quoted(it) << std::endl;
        if (globalLogger) {
            *globalLogger << DETECTED_SERIAL_PORT_STRING << std::quoted(it) << tEndl();
        }
        std::shared_ptr<SerialPort> serialPort{std::make_shared<SerialPort>(it,
                                                                            Arduino::FIRMWARE_BAUD,
                                                                            Arduino::FIRMWARE_DATA_BITS,
                                                                            Arduino::FIRMWARE_STOP_BITS,
                                                                            Arduino::FIRMWARE_PARITY)};

        for (unsigned int tryCount = 0; tryCount < Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT(); tryCount++) {
            try {
                serialPort->openPort();
                delayMillisecondsWithCallback(Arduino::BOOTLOADER_BOOT_TIME, setProgressBar, qApplication.get(), plwuiPtr);

                std::cout << SUCCESSFULLY_OPENED_SERIAL_PORT_STRING << serialPort->portName() << std::endl;
                if (globalLogger) {
                    *globalLogger << SUCCESSFULLY_OPENED_SERIAL_PORT_STRING << serialPort->portName() << tEndl();
                }
                serialPorts.emplace_back(serialPort);
                tryCount = Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT();
            } catch (std::exception &e) {
                serialPort->closePort();
                if (tryCount == Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT()-1) {
                    std::unique_ptr<QMessageBox> errorBox{std::make_unique<QMessageBox>()};
                    errorBox->setWindowTitle(toQString(ArduinoPCStrings::ERROR_OPENING_SERIAL_PORT_WINDOW_TITLE));
                    errorBox->setWindowIcon(apci->MAIN_WINDOW_ICON);
                    errorBox->setText(toQString(static_cast<std::string>(ERROR_OPENING_SERIAL_PORT_STRING) + it + "): " + static_cast<std::string>(e.what())));
                    errorBox->exec();
                }
                std::cout << "Attempt " << tryCount+1 << "/" << TRY_COUNT_HIGH_LIMIT  << ": " << ERROR_OPENING_SERIAL_PORT_STRING << it << "): " << e.what() << std::endl;
                tryCount++;
            }
        }
    }
    std::shared_ptr<MainWindow> mainWindow{std::make_shared<MainWindow>(apci, qDesktopWidget, globalLogger)};
    mainWindow->initializeIO(serialPorts, plwuiPtr);
    plwuiPtr->close();
    mainWindow->showMaximized();

    return qApplication->exec();
}

void setProgressBar(QApplication *update, std::shared_ptr<ProgramLoadingWidget> plw)
{
    plw->updateProgressBar();
    plw->updateMessages();
    update->processEvents();
}

bool isSwitch(const std::string &toCheck, const std::vector<std::string> &switches)
{
    std::string copyString{toCheck};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    for (auto &it : switches) {
        if (it == copyString) {
            return true;
        }
    }
    return false;
}

bool isSwitch(const std::string &toCheck, const std::vector<const char *> &switches)
{
    std::string copyString{toCheck};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    for (auto &it : switches) {
        if (static_cast<std::string>(it) == copyString) {
            return true;
        }
    }
    return false;
}

bool isSwitch(const char *toCheck, const std::vector<std::string> &switches)
{
    return isSwitch(static_cast<std::string>(toCheck), switches);
}

bool isSwitch(const char *toCheck, const std::vector<const char *> &switches)
{
    return isSwitch(static_cast<std::string>(toCheck), switches);
}

bool isCommandLineIgnored(const std::string &portName, const std::vector<std::string> &ignoredSerialPorts)
{
    for (auto &it : ignoredSerialPorts) {
        if (portName == it) {
            return true;
        }
    }
    return false;
}

