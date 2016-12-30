#include "mainwindow.h"
#include "ui_mainwindow.h"

const int MainWindow::s_TASKBAR_HEIGHT{15};
const int MainWindow::s_CHECK_DISCONNECTED_SERIAL_PORTS_INTERVAL{150};
std::set<int> MainWindow::s_attachedArduinoIndexes{};

MainWindow::MainWindow(std::shared_ptr<ArduinoPCIcons> apciPtr,
                       std::shared_ptr<QDesktopWidget> qDesktopWidget,
                       std::shared_ptr<GlobalLogger> globalLogger,
                       QWidget *parent) :
    QMainWindow{parent},
    m_uiPtr{std::make_unique<Ui::MainWindow>()},
    m_apciPtr{apciPtr},
    m_qDesktopWidget{qDesktopWidget},
    m_globalLogger{globalLogger},
    m_arduinoScheduler{std::make_unique<ArduinoScheduler>()},
    m_manualScreenScheduler{std::make_unique<ManualScreenScheduler>()},
    m_canTerminalScheduler{std::make_unique<CanTerminalScheduler>()},
    m_serialTerminalScheduler{std::make_unique<SerialTerminalScheduler>()},
    m_disconnectedSerialPortTimer{std::make_unique<QTimer>()},
    m_configurationFileReader{std::make_unique<ConfigurationFileReader>()},
    m_xPlacement{0},
    m_yPlacement{0}
{
    using namespace ArduinoPCStrings;
    this->m_uiPtr->setupUi(this);

    this->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
    this->setStyleSheet(toQString(MAIN_WINDOW_STYLESHEET));
    this->setWindowTitle(toQString(MAIN_WINDOW_TITLE));

    connect(this->m_uiPtr->actionAboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    connect(this->m_uiPtr->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(cleanupSerialPorts()));

    this->m_disconnectedSerialPortTimer->setInterval(MainWindow::s_CHECK_DISCONNECTED_SERIAL_PORTS_INTERVAL);
    connect(this->m_disconnectedSerialPortTimer.get(), SIGNAL(timeout()), this, SLOT(checkForDisconnectedSerialPorts()));
    //this->m_disconnectedSerialPortTimer->start();
}

void MainWindow::checkForDisconnectedSerialPorts()
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::vector<std::string> currentSerialPorts{SerialPort::availableSerialPorts()};
    currentSerialPorts = GeneralUtilities::safeErasePartialMatch(currentSerialPorts, static_cast<std::string>(KNOWN_BAD_SERIAL_PORT_NAME));
    for (auto &it : MainWindow::s_attachedArduinoIndexes) {
        bool deletionNeeded{true};
        for (auto &innerIt : currentSerialPorts) {
            if (this->m_arduinoScheduler->arduinoByScreenIndex(it)->serialPort()->portName() == innerIt) {
                deletionNeeded = false;
            }
        }
        if (deletionNeeded) {
           uninitializeSingleIO(it);
        }
    }

    std::vector<std::string> serialPorts;
    std::set<std::string> serialPortsToAdd;
    for (auto &it : MainWindow::s_attachedArduinoIndexes) {
        serialPorts.emplace_back(this->m_arduinoScheduler->arduinoByScreenIndex(it)->serialPort()->portName());
    }
    for (auto &it : currentSerialPorts) {
        if (std::find(serialPorts.begin(), serialPorts.end(), it) != serialPorts.end()) {
            serialPortsToAdd.emplace(it);
        }
    }
    for (auto &it : serialPortsToAdd) {
        std::shared_ptr<SerialPort> serialPort{std::make_shared<SerialPort>(it,
                                                                            Arduino::FIRMWARE_BAUD,
                                                                            Arduino::FIRMWARE_DATA_BITS,
                                                                            Arduino::FIRMWARE_STOP_BITS,
                                                                            Arduino::FIRMWARE_PARITY)};

        for (unsigned int tryCount = 0; tryCount < Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT(); tryCount++) {
            try {
                serialPort->openPort();
                delayMillisecondsWithCallback(Arduino::BOOTLOADER_BOOT_TIME, MainWindow::forceUiRefresh);
                std::cout << SUCCESSFULLY_OPENED_SERIAL_PORT_STRING << serialPort->portName() << std::endl;
                tryCount = Arduino::SERIAL_PORT_TRY_COUNT_HIGH_LIMIT();
                initializeSingleIO(MainWindow::nextAvailableArduinoSlot(), serialPort);
            } catch (std::exception &e) {
                (void)e;
                tryCount++;
            }
        }
    }
}

int MainWindow::numberOfAttachedArduinos()
{
    return MainWindow::s_attachedArduinoIndexes.size();
}

void MainWindow::forceUiRefresh()
{
    qApp->processEvents();
}

void MainWindow::initializeSingleIO(int screenIndex, std::shared_ptr<SerialPort> serialPort, std::shared_ptr<GlobalLogger> globalLogger)
{
    using namespace ArduinoPCStrings;
    (void)globalLogger;
    std::shared_ptr<Arduino> arduinoTemp{Arduino::makeArduino(serialPort)};
    if (arduinoTemp == nullptr) {
        std::unique_ptr<QMessageBox> errorBox{std::make_unique<QMessageBox>()};
        errorBox->setWindowTitle(COULD_NOT_FETCH_ARDUINO_STRING);
        errorBox->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
        errorBox->setText(toQString(static_cast<std::string>(UNKNOWN_ARDUINO_TYPE_RECEIVED_STRING) + serialPort->portName()) + ")");
        errorBox->exec();
        return;
    } else {
        this->m_arduinoScheduler->addArduino(screenIndex, arduinoTemp);
    }
    this->applyConfigurationFile(arduinoTemp);

    this->m_manualScreenScheduler->addManualScreen(screenIndex, this->m_apciPtr, this->m_qDesktopWidget, this->m_globalLogger, arduinoTemp);
    this->m_serialTerminalScheduler->addSerialTerminal(screenIndex, this->m_apciPtr, this->m_qDesktopWidget, this->m_globalLogger, arduinoTemp);

    CustomAction *serialTerminalAction{new CustomAction(CustomActionType::SERIAL_TERMINAL, screenIndex)};
    connect(serialTerminalAction, SIGNAL(triggered(CustomActionType,int,bool)), this, SLOT(onCustomActionTriggered(CustomActionType,int,bool)));
    connect(this->m_serialTerminalScheduler->serialTerminalByScreenIndex(screenIndex).get(), SIGNAL(aboutToClose(int)), this, SLOT(onSerialTerminalClosed(int)));

    serialTerminalAction->setText(toQString(arduinoTemp->longName()) + " (" + arduinoTemp->serialPortName().c_str() + ")");
    this->m_serialTerminalScheduler->serialTerminalByScreenIndex(screenIndex)->setWindowTitle(toQString(SERIAL_TERMINAL_WINDOW_TITLE_BASE) + toQString(arduinoTemp->longName() + static_cast<std::string>(" (") + arduinoTemp->serialPortName() + ")"));
    this->m_uiPtr->menuSerialTerminal->addAction(serialTerminalAction);

    CustomAction *manualScreenAction{new CustomAction(CustomActionType::MANUAL_SCREEN, screenIndex)};
    connect(manualScreenAction, SIGNAL(triggered(CustomActionType,int,bool)), this, SLOT(onCustomActionTriggered(CustomActionType,int,bool)));
    connect(this->m_manualScreenScheduler->manualScreenByScreenIndex(screenIndex).get(), SIGNAL(aboutToClose(int)), this, SLOT(onManualScreenClosed(int)));

    manualScreenAction->setText(toQString(arduinoTemp->longName()) + " (" + arduinoTemp->serialPortName().c_str() + ")");
    this->m_manualScreenScheduler->manualScreenByScreenIndex(screenIndex)->setWindowTitle(toQString(MANUAL_SCREEN_WINDOW_TITLE_BASE) + toQString(arduinoTemp->longName() +  static_cast<std::string>(" (") + arduinoTemp->serialPortName() + ")"));
    manualScreenAction->setData(screenIndex);
    this->m_uiPtr->menuManual->addAction(manualScreenAction);

    if (this->m_arduinoScheduler->arduinoByScreenIndex(screenIndex)->canCapability().second) {
        this->m_canTerminalScheduler->addCanTerminal(screenIndex, this->m_apciPtr, this->m_qDesktopWidget, this->m_globalLogger, arduinoTemp);

        CustomAction *canTerminalAction{new CustomAction(CustomActionType::CAN_TERMINAL, screenIndex)};
        connect(canTerminalAction, SIGNAL(triggered(CustomActionType,int,bool)), this, SLOT(onCustomActionTriggered(CustomActionType,int,bool)));
        connect(this->m_canTerminalScheduler->canTerminalByScreenIndex(screenIndex).get(), SIGNAL(aboutToClose(int)), this, SLOT(onCanTerminalClosed(int)));

        canTerminalAction->setText(toQString(arduinoTemp->longName()) + " (" + arduinoTemp->serialPortName().c_str() + ")");
        this->m_canTerminalScheduler->canTerminalByScreenIndex(screenIndex)->setWindowTitle(toQString(CAN_TERMINAL_WINDOW_TITLE_BASE) + toQString(arduinoTemp->longName() + static_cast<std::string>(" (") + arduinoTemp->serialPortName() + ")"));

        this->m_uiPtr->menuCANTerminal->addAction(canTerminalAction);
        this->m_customActions.emplace(screenIndex, canTerminalAction);
    }
    this->m_customActions.emplace(screenIndex, serialTerminalAction);
    this->m_customActions.emplace(screenIndex, manualScreenAction);
    MainWindow::s_attachedArduinoIndexes.emplace(screenIndex);
}

void MainWindow::uninitializeSingleIO(int serialPortIndex, std::shared_ptr<GlobalLogger> globalLogger)
{
    (void)globalLogger;
    if (this->m_arduinoScheduler->arduinoByScreenIndex(serialPortIndex)->canCapability().second) {
        this->m_canTerminalScheduler->removeCanTerminal(serialPortIndex);
    }
    this->m_serialTerminalScheduler->removeSerialTerminal(serialPortIndex);
    this->m_manualScreenScheduler->removeManualScreen(serialPortIndex);
    this->m_arduinoScheduler->removeArduino(serialPortIndex);

    auto range = this->m_customActions.equal_range(serialPortIndex);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second->customActionType() == CustomActionType::MANUAL_SCREEN) {
            this->m_uiPtr->menuManual->removeAction(it->second);
        } else if (it->second->customActionType() == CustomActionType::SERIAL_TERMINAL) {
            this->m_uiPtr->menuSerialTerminal->removeAction(it->second);
        } else if (it->second->customActionType() == CustomActionType::CAN_TERMINAL) {
            this->m_uiPtr->menuCANTerminal->removeAction(it->second);
        }
    }
    this->m_customActions.erase(serialPortIndex);
    MainWindow::s_attachedArduinoIndexes.erase(serialPortIndex);
}

void MainWindow::applyConfigurationFile(std::shared_ptr<Arduino> arduino)
{
    std::map<int, std::string> aliasesMap;
    std::map<int, std::string> ioTypesMap;
    std::map<int, std::string> initialStatesMap;
    if (arduino->arduinoType() == ArduinoType::UNO) {
        aliasesMap = this->m_configurationFileReader->unoAliasesMap();
        ioTypesMap = this->m_configurationFileReader->unoIOTypesMap();
        initialStatesMap = this->m_configurationFileReader->unoInitialStatesMap();
    } else if (arduino->arduinoType() == ArduinoType::NANO) {
        aliasesMap = this->m_configurationFileReader->nanoAliasesMap();
        ioTypesMap = this->m_configurationFileReader->nanoIOTypesMap();
        initialStatesMap = this->m_configurationFileReader->nanoInitialStatesMap();
    } else if (arduino->arduinoType() == ArduinoType::MEGA) {
        aliasesMap = this->m_configurationFileReader->megaAliasesMap();
        ioTypesMap = this->m_configurationFileReader->megaIOTypesMap();
        initialStatesMap = this->m_configurationFileReader->megaInitialStatesMap();
    }

    arduino->assignAliasesFromMap(aliasesMap);
    arduino->assignIOTypesFromMap(ioTypesMap);
    arduino->assignInitialStatesFromMap(initialStatesMap);
    arduino->assignIOTypes();

   if (this->m_configurationFileReader->bluetoothMultiplier() != -1) {
       Arduino::bluetoothSendDelayMultiplier = this->m_configurationFileReader->bluetoothMultiplier();
   }
}

void MainWindow::assignIOTypesFromMap(std::shared_ptr<Arduino> arduino, const std::map<int, std::string> &ioTypesMap)
{
    using namespace GeneralUtilities;
    for (auto &it : ioTypesMap) {
        std::string stringToLog{toString(Arduino::SETTING_IO_TYPE_STRING) + tQuoted(it.second) + toString(Arduino::FOR_PIN_NUMBER_STRING) + toString(it.first) + toString(Arduino::ELIPSES_STRING)};
        try {
            std::shared_ptr<GPIO> gpioTemp{arduino->gpioPinByPinNumber(it.first)};
            gpioTemp->setIOType(arduino->parseIOTypeFromString(it.second));
            std::pair<IOStatus, IOType> result{arduino->pinMode(gpioTemp, gpioTemp->ioType())};
            if (result.first == IOStatus::OPERATION_FAILURE) {
                stringToLog.append(Arduino::GENERIC_FAILED_STRING);
            } else {
                stringToLog.append(Arduino::GENERIC_SUCCESS_STRING);
            }
        } catch (std::exception &e) {
            (void)e;
            stringToLog.append(Arduino::GENERIC_FAILED_STRING);
        }
        std::cout << stringToLog;
        std::cout << " (" << arduino->identifier() << ", " << arduino->serialPortName() << ")" << std::endl;
    }
}

void MainWindow::assignInitialStatesFromMap(std::shared_ptr<Arduino> arduino, const std::map<int, std::string> &initialStatesMap)
{
    using namespace GeneralUtilities;
    for (auto &it : initialStatesMap) {
        std::string logString{toString(Arduino::USING_INITIAL_STATE_STRING) + tQuoted(it.second) + toString(Arduino::FOR_PIN_NUMBER_STRING) + toString(it.first) + toString(Arduino::ELIPSES_STRING)};
        try {
            std::shared_ptr<GPIO> gpioTemp{arduino->gpioPinByPinNumber(it.first)};
            if (gpioTemp->ioType() == IOType::DIGITAL_OUTPUT) {
                std::pair<IOStatus, bool> result{arduino->digitalWrite(gpioTemp, arduino->parseToDigitalState(it.second))};
                if (result.first == IOStatus::OPERATION_FAILURE) {
                    logString.append(Arduino::GENERIC_FAILED_STRING);
                } else {
                    logString.append(Arduino::GENERIC_SUCCESS_STRING);
                }
            } else if (gpioTemp->ioType() == IOType::ANALOG_OUTPUT) {
                std::pair<IOStatus, int> result{arduino->analogWriteRaw(gpioTemp, arduino->parseToAnalogStateRaw(it.second))};
                if (result.first == IOStatus::OPERATION_FAILURE) {
                    logString.append(Arduino::GENERIC_FAILED_STRING);
                } else {
                    logString.append(Arduino::GENERIC_SUCCESS_STRING);
                }
            }
        } catch (std::exception &e) {
            logString.append(Arduino::GENERIC_FAILED_STRING);
            (void)e;
        }
        std::cout << logString;
        std::cout << " (" << arduino->identifier() << ", " << arduino->serialPortName() << ")" << std::endl;
    }
}


QApplication *MainWindow::application()
{
    return qApp;
}

void MainWindow::cleanupSerialPorts()
{
    for (auto &it : MainWindow::s_attachedArduinoIndexes) {
        this->m_arduinoScheduler->arduinoByScreenIndex(it)->serialPortAtIndex(it)->closePort();
    }
}

void MainWindow::initializeIO(std::vector<std::shared_ptr<SerialPort>> serialPorts, std::shared_ptr<ProgramLoadingWidget> plw)
{

    using namespace ArduinoPCStrings;
    for (auto &it : serialPorts) {
        if (plw) {
            plw->updateProgressBar();
            plw->updateMessages();
        }
        std::shared_ptr<Arduino> arduinoTemp{Arduino::makeArduino(it)};
        if (arduinoTemp == nullptr) {
            std::unique_ptr<QMessageBox> errorBox{std::make_unique<QMessageBox>()};
            errorBox->setWindowTitle(COULD_NOT_FETCH_ARDUINO_STRING);
            errorBox->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
            errorBox->setText(toQString(static_cast<std::string>(UNKNOWN_ARDUINO_TYPE_RECEIVED_STRING) + it->portName()) + ")");
            errorBox->exec();
        } else {
            int arduinoIndex{MainWindow::nextAvailableArduinoSlot()};
            this->m_arduinoScheduler->addArduino(arduinoIndex, arduinoTemp);
            MainWindow::s_attachedArduinoIndexes.emplace(arduinoIndex);
        }
    }

    using namespace ArduinoPCStrings;
    for (auto &screenIndex : MainWindow::s_attachedArduinoIndexes) {
        if (isLemonSerialPort(screenIndex)) {
            continue;
        }
        std::shared_ptr<Arduino> arduinoTemp{this->m_arduinoScheduler->arduinoByScreenIndex(screenIndex)};
        this->applyConfigurationFile(arduinoTemp);

        this->m_manualScreenScheduler->addManualScreen(screenIndex, this->m_apciPtr, this->m_qDesktopWidget, this->m_globalLogger, arduinoTemp);
        this->m_serialTerminalScheduler->addSerialTerminal(screenIndex, this->m_apciPtr, this->m_qDesktopWidget, this->m_globalLogger, arduinoTemp);

        CustomAction *serialTerminalAction{new CustomAction(CustomActionType::SERIAL_TERMINAL, screenIndex)};
        connect(serialTerminalAction, SIGNAL(triggered(CustomActionType,int,bool)), this, SLOT(onCustomActionTriggered(CustomActionType,int,bool)));
        connect(this->m_serialTerminalScheduler->serialTerminalByScreenIndex(screenIndex).get(), SIGNAL(aboutToClose(int)), this, SLOT(onSerialTerminalClosed(int)));

        serialTerminalAction->setText(toQString(arduinoTemp->longName()) + " (" + arduinoTemp->serialPortName().c_str() + ")");
        this->m_serialTerminalScheduler->serialTerminalByScreenIndex(screenIndex)->setWindowTitle(toQString(SERIAL_TERMINAL_WINDOW_TITLE_BASE) + toQString(arduinoTemp->longName() + static_cast<std::string>(" (") + arduinoTemp->serialPortName() + ")"));
        this->m_uiPtr->menuSerialTerminal->addAction(serialTerminalAction);

        CustomAction *manualScreenAction{new CustomAction(CustomActionType::MANUAL_SCREEN, screenIndex)};
        connect(manualScreenAction, SIGNAL(triggered(CustomActionType,int,bool)), this, SLOT(onCustomActionTriggered(CustomActionType,int,bool)));
        connect(this->m_manualScreenScheduler->manualScreenByScreenIndex(screenIndex).get(), SIGNAL(aboutToClose(int)), this, SLOT(onManualScreenClosed(int)));

        manualScreenAction->setText(toQString(arduinoTemp->longName()) + " (" + arduinoTemp->serialPortName().c_str() + ")");
        this->m_manualScreenScheduler->manualScreenByScreenIndex(screenIndex)->setWindowTitle(toQString(MANUAL_SCREEN_WINDOW_TITLE_BASE) + toQString(arduinoTemp->longName() +  static_cast<std::string>(" (") + arduinoTemp->serialPortName() + ")"));

        manualScreenAction->setData(screenIndex);
        this->m_uiPtr->menuManual->addAction(manualScreenAction);

        if (this->m_arduinoScheduler->arduinoByScreenIndex(screenIndex)->canCapability().second) {
            this->m_canTerminalScheduler->addCanTerminal(screenIndex, this->m_apciPtr, this->m_qDesktopWidget, this->m_globalLogger, arduinoTemp);

            CustomAction *canTerminalAction{new CustomAction(CustomActionType::CAN_TERMINAL, screenIndex)};
            connect(canTerminalAction, SIGNAL(triggered(CustomActionType,int,bool)), this, SLOT(onCustomActionTriggered(CustomActionType,int,bool)));
            connect(this->m_canTerminalScheduler->canTerminalByScreenIndex(screenIndex).get(), SIGNAL(aboutToClose(int)), this, SLOT(onCanTerminalClosed(int)));

            canTerminalAction->setText(toQString(arduinoTemp->longName()) + " (" + arduinoTemp->serialPortName().c_str() + ")");
            this->m_canTerminalScheduler->canTerminalByScreenIndex(screenIndex)->setWindowTitle(toQString(CAN_TERMINAL_WINDOW_TITLE_BASE) + toQString(arduinoTemp->longName() + static_cast<std::string>(" (") + arduinoTemp->serialPortName() + ")"));

            this->m_uiPtr->menuCANTerminal->addAction(canTerminalAction);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    /*
    using namespace ArduinoPCStrings;
    QMessageBox::StandardButton userReply;
    userReply = QMessageBox::question(this,
                                      CLOSE_APPLICATION_WINDOW_TITLE,
                                      CLOSE_APPLICATION_WINDOW_PROMPT,
                                      QMessageBox::Yes|QMessageBox::No);
    if (userReply == QMessageBox::Yes) {
        for (auto &it : this->m_manualScreenScheduler->manualScreens()) {
            if (it) {
                it->close();
            }
        }
        event->accept();
        QApplication::exit(0);
        QMainWindow::closeEvent(event);
    } else {
        event->ignore();
    }
    */
    this->m_disconnectedSerialPortTimer->stop();
    for (auto &it : this->m_manualScreenScheduler->manualScreens()) {
        if (it) {
            it->close();
        }
    }
    for (auto &it : MainWindow::s_attachedArduinoIndexes) {
        this->m_arduinoScheduler->arduinoByScreenIndex(it)->serialPortAtIndex(it)->closePort();
    }
    event->accept();
    QApplication::exit(0);
    QMainWindow::closeEvent(event);
}

int MainWindow::nextAvailableArduinoSlot()
{
    for (int i = 0; i < 256; i++) {
        if (MainWindow::s_attachedArduinoIndexes.find(i) == MainWindow::s_attachedArduinoIndexes.end()) {
            return i;
        }
    }
    return 256;
}

std::shared_ptr<Arduino> MainWindow::arduinoPtrAtIndex(int index) const
{
    using namespace ArduinoPCStrings;
    if (MainWindow::s_attachedArduinoIndexes.find(index) == MainWindow::s_attachedArduinoIndexes.end()) {
        throw std::runtime_error(INVALID_ARDUINO_PTR_AT_INDEX_STRING + std::to_string(index));
    } else {
        return this->m_arduinoScheduler->arduinoByScreenIndex(index);
    }
}

bool MainWindow::isLemonSerialPort(int serialPortIndex)
{
    return (this->m_lemonSerialPorts.find(serialPortIndex) != this->m_lemonSerialPorts.end());
}

void MainWindow::onCustomActionTriggered(CustomActionType customActionType, int screenIndex, bool checked)
{
    (void)checked;
    if (customActionType == CustomActionType::MANUAL_SCREEN) {
        startManualScreenAutoUpdate(screenIndex);
    } else if (customActionType == CustomActionType::SERIAL_TERMINAL) {
        startSerialTerminalAutoUpdate(screenIndex);
    } else if (customActionType == CustomActionType::CAN_TERMINAL) {
        startCanTerminalAutoUpdate(screenIndex);
    }
}

void MainWindow::startManualScreenAutoUpdate(int screenIndex)
{
    try {
        this->m_uiPtr->menuManual->actions().at(screenIndex)->setEnabled(false);
    } catch (std::exception &e) {
        (void)e;
    }
    this->m_manualScreenScheduler->manualScreenByScreenIndex(screenIndex)->show();
    this->m_manualScreenScheduler->manualScreenByScreenIndex(screenIndex)->centerAndFitWindow();
}

void MainWindow::startSerialTerminalAutoUpdate(int screenIndex)
{
    std::shared_ptr<Arduino> arduinoTemp{this->m_arduinoScheduler->arduinoByScreenIndex(screenIndex)};
    using namespace ArduinoPCStrings;
    if (!this->m_manualScreenScheduler->manualScreenByScreenIndex(screenIndex)->isHidden()) {
        this->m_manualScreenScheduler->manualScreenByScreenIndex(screenIndex)->close();
    }
    try {
        this->m_uiPtr->menuManual->actions().at(screenIndex)->setEnabled(false);
    } catch (std::exception &e) {
        (void)e;
    }
    if ((arduinoTemp->canCapability().first) && ((arduinoTemp->canCapability().second))) {
        if (!this->m_canTerminalScheduler->canTerminalByScreenIndex(screenIndex)->isHidden()) {
            this->m_canTerminalScheduler->canTerminalByScreenIndex(screenIndex)->hide();
        }
        try {
            this->m_uiPtr->menuCANTerminal->actions().at(screenIndex)->setEnabled(false);
            this->m_uiPtr->menuSerialTerminal->actions().at(screenIndex)->setEnabled(false);
        } catch (std::exception &e) {
            (void)e;
        }
    }
    this->m_serialTerminalScheduler->serialTerminalByScreenIndex(screenIndex)->show();
    this->m_serialTerminalScheduler->serialTerminalByScreenIndex(screenIndex)->centerAndFitWindow();
}


void MainWindow::startCanTerminalAutoUpdate(int screenIndex)
{
    using namespace ArduinoPCStrings;
    std::shared_ptr<Arduino> arduinoTemp{this->m_arduinoScheduler->arduinoByScreenIndex(screenIndex)};
    std::pair<IOStatus, bool> initializeResult{arduinoTemp->initializeCanBus(screenIndex)};
    if (initializeResult.first == IOStatus::OPERATION_FAILURE) {
        std::cout << FAILED_TO_INITIALIZE_CAN_BUS_STRING << arduinoTemp->serialPortName() << " - " << arduinoTemp->identifier() << std::endl;
    } else {
        std::cout << SUCCESSFULLY_INITIALIZED_CAN_BUS_STRING << arduinoTemp->serialPortName() << " - " << arduinoTemp->identifier() << std::endl;
    }
    std::pair<IOStatus, bool> canUpdate{this->m_arduinoScheduler->arduinoByScreenIndex(screenIndex)->canAutoUpdate(true, screenIndex)};
    if (canUpdate.first == IOStatus::OPERATION_FAILURE) {
        std::cout << FAILED_TO_SET_CAN_AUTO_UPDATE_STRING << arduinoTemp->serialPortName() << " - " << arduinoTemp->identifier() << std::endl;
    } else {
        std::cout << SUCCESSFULLY_SET_CAN_AUTO_UPDATE_STRING  << arduinoTemp->serialPortName() << " - " << arduinoTemp->identifier() << std::endl;
    }
    this->m_canTerminalScheduler->canTerminalByScreenIndex(screenIndex)->show();
    this->m_canTerminalScheduler->canTerminalByScreenIndex(screenIndex)->centerAndFitWindow();

    if (!this->m_manualScreenScheduler->manualScreenByScreenIndex(screenIndex)->isHidden()) {
        this->m_manualScreenScheduler->manualScreenByScreenIndex(screenIndex)->close();
    }
    if (!this->m_serialTerminalScheduler->serialTerminalByScreenIndex(screenIndex)->isHidden()) {
        this->m_serialTerminalScheduler->serialTerminalByScreenIndex(screenIndex)->close();
    }
    try {
        this->m_uiPtr->menuManual->actions().at(screenIndex)->setEnabled(false);
        this->m_uiPtr->menuSerialTerminal->actions().at(screenIndex)->setEnabled(false);
        this->m_uiPtr->menuCANTerminal->actions().at(screenIndex)->setEnabled(false);
    } catch (std::exception &e) {
        (void)e;
    }
}

void MainWindow::onManualScreenClosed(int screenIndex)
{
    try {
        this->m_uiPtr->menuManual->actions().at(screenIndex)->setChecked(false);
        this->m_uiPtr->menuManual->actions().at(screenIndex)->setEnabled(true);
    } catch (std::exception &e) {
        (void)e;
    }
}

void MainWindow::onSerialTerminalClosed(int screenIndex)
{
    std::shared_ptr<Arduino> arduinoTemp{this->m_arduinoScheduler->arduinoByScreenIndex(screenIndex)};
    try {
        this->m_uiPtr->menuManual->actions().at(screenIndex)->setEnabled(true);
        this->m_uiPtr->menuSerialTerminal->actions().at(screenIndex)->setChecked(false);
        this->m_uiPtr->menuSerialTerminal->actions().at(screenIndex)->setEnabled(true);
    } catch (std::exception &e) {
        (void)e;
    }
    if ((arduinoTemp->canCapability().first) && ((arduinoTemp->canCapability().second))) {
        try {
            this->m_uiPtr->menuCANTerminal->actions().at(screenIndex)->setEnabled(true);
        } catch (std::exception &e) {
            (void)e;
        }
    }
}

void MainWindow::onCanTerminalClosed(int screenIndex)
{
    using namespace ArduinoPCStrings;
    std::shared_ptr<Arduino> arduinoTemp{this->m_arduinoScheduler->arduinoByScreenIndex(screenIndex)};
    std::pair<IOStatus, bool> canUpdate{this->m_arduinoScheduler->arduinoByScreenIndex(screenIndex)->canAutoUpdate(false, screenIndex)};
    if (canUpdate.first == IOStatus::OPERATION_FAILURE) {
        std::cout << FAILED_TO_CANCEL_CAN_AUTO_UPDATE_STRING << arduinoTemp->serialPortName() << " - " << arduinoTemp->identifier() << std::endl;
    } else {
        std::cout << SUCCESSFULLY_CANCELED_CAN_AUTO_UPDATE_STRING  << arduinoTemp->serialPortName() << " - " << arduinoTemp->identifier() << std::endl;
    }
    try {
        this->m_uiPtr->menuManual->actions().at(screenIndex)->setEnabled(true);
        this->m_uiPtr->menuCANTerminal->actions().at(screenIndex)->setChecked(false);
        this->m_uiPtr->menuCANTerminal->actions().at(screenIndex)->setEnabled(true);
    } catch (std::exception &e) {
        (void)e;
    }
    try {
        this->m_uiPtr->menuSerialTerminal->actions().at(screenIndex)->setEnabled(true);
    } catch (std::exception &e) {
        (void)e;
    }
}


int MainWindow::TASKBAR_HEIGHT()
{
    return MainWindow::s_TASKBAR_HEIGHT;
}

void MainWindow::centerAndFitWindow()
{
    this->setFixedSize(this->minimumSize());
    calculateXYPlacement();
    this->move(this->m_xPlacement, this->m_yPlacement);
}

int MainWindow::xPlacement() const
{
    return this->m_xPlacement;
}

int MainWindow::yPlacement() const
{
    return this->m_yPlacement;
}

void MainWindow::calculateXYPlacement()
{
    std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qDesktopWidget->availableGeometry())};
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
    this->m_yPlacement = (avail->height()/2)-(this->height()/2) - MainWindow::s_TASKBAR_HEIGHT;
}

void MainWindow::bindGlobalLogger(std::shared_ptr<GlobalLogger> globalLogger)
{
    this->m_globalLogger.reset();
    this->m_globalLogger = globalLogger;
}

void MainWindow::bindQDesktopWidget(const std::shared_ptr<QDesktopWidget> &qDesktopWidget)
{
    this->m_qDesktopWidget.reset();
    this->m_qDesktopWidget = qDesktopWidget;
}

void MainWindow::bindArduinoPCIcons(const std::shared_ptr<ArduinoPCIcons> &apciPtr)
{
    this->m_apciPtr.reset();
    this->m_apciPtr = apciPtr;
}

MainWindow::~MainWindow()
{

}
