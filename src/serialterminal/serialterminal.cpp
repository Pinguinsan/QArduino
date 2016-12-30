#include "serialterminal/serialterminal.h"
#include "ui_serialterminal.h"

const int SerialTerminal::s_SERIAL_TIMEOUT{400};
const int SerialTerminal::s_SERIAL_SCRIPT_INDENT{0};
const int SerialTerminal::s_SERIAL_TERMINAL_UPDATE_TIME{100};
const int SerialTerminal::s_SERIAL_STATUS_BAR_TIMEOUT{4000};

SerialTerminal::SerialTerminal(int screenIndex,
                               std::shared_ptr<ArduinoPCIcons> apciPtr,
                               std::shared_ptr<QDesktopWidget> qDesktopWidget,
                               std::shared_ptr<GlobalLogger> globalLogger,
                               std::shared_ptr<Arduino> arduino,
                               QWidget *parent) :
    QMainWindow{parent},
    m_uiPtr{std::make_shared<Ui::SerialTerminal>()},
    m_apciPtr{apciPtr},
    m_qDesktopWidget{qDesktopWidget},
    m_globalLogger{globalLogger},
    m_arduino{arduino},
    m_packagedSerialReportTask{SerialTerminal::serialReport},
    m_serialReportFuture{std::make_unique<SmartSerialReportFuture>(this, this->m_packagedSerialReportTask)},
    m_serialUpdateTimer{std::make_unique<QTimer>()},
    m_packagedRxResultTask{SerialTerminal::staticPrintRxResult},
    m_packagedTxResultTask{SerialTerminal::staticPrintTxResult},
    m_packagedDelayResultTask{SerialTerminal::staticPrintDelayResult},
    m_packagedFlushResultTask{SerialTerminal::staticPrintFlushResult},
    m_packagedLoopResultTask{SerialTerminal::staticPrintLoopResult},
    m_currentLinePushedIntoCommandHistory{false},
    m_currentHistoryIndex{0},
    m_cancelScript{false},
    m_screenIndex{screenIndex},
    m_xPlacement{0},
    m_yPlacement{0}
{
    using namespace ArduinoPCStrings;
    this->m_uiPtr->setupUi(this);
    this->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);

    connect(this->m_uiPtr->actionClose, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(this->m_uiPtr->sendBox, SIGNAL(commandHistoryContextMenuRequested(const QPoint &)), this, SLOT(onCommandHistoryContextMenuRequested(const QPoint &)));
    connect(this->m_uiPtr->actionLoadScript, SIGNAL(triggered(bool)), this, SLOT(onActionLoadScriptTriggered(bool)));

    connect(this->m_uiPtr->sendButton, SIGNAL(clicked(bool)), this, SLOT(onSendButtonClicked()));
    connect(this->m_uiPtr->sendBox, SIGNAL(returnPressed(SerialTerminalLineEdit*)), this, SLOT(onReturnKeyPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(upArrowPressed(SerialTerminalLineEdit*)), this, SLOT(onUpArrowPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(downArrowPressed(SerialTerminalLineEdit*)), this, SLOT(onDownArrowPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(escapePressed(SerialTerminalLineEdit*)), this, SLOT(onEscapeKeyPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(altPressed(SerialTerminalLineEdit*)), this, SLOT(onAltKeyPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(ctrlAPressed(SerialTerminalLineEdit*)), this, SLOT(onCtrlAPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(ctrlEPressed(SerialTerminalLineEdit*)), this, SLOT(onCtrlEPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(ctrlUPressed(SerialTerminalLineEdit*)), this, SLOT(onCtrlUPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(ctrlGPressed(SerialTerminalLineEdit*)), this, SLOT(onCtrlGPressed(SerialTerminalLineEdit*)));

    this->m_serialUpdateTimer->setInterval(SerialTerminal::s_SERIAL_TERMINAL_UPDATE_TIME);
}

void SerialTerminal::setScreenIndex(int screenIndex)
{
    this->m_screenIndex = screenIndex;
}

int SerialTerminal::screenIndex() const
{
    return this->m_screenIndex;
}

void SerialTerminal::hideEvent(QHideEvent *event)
{
    if (event->type() == QHideEvent::Hide) {
        pauseUpdates();
    } else {
        resumeUpdates();
    }
    return QMainWindow::hideEvent(event);
}

void SerialTerminal::showEvent(QShowEvent *event)
{
    if (event->type() == QShowEvent::Show) {
        resumeUpdates();
    } else {
        pauseUpdates();
    }
    return QMainWindow::showEvent(event);
}

void SerialTerminal::pauseUpdates()
{
    disconnect(this->m_serialUpdateTimer.get(), SIGNAL(timeout()), this, SLOT(updateSerialTerminal()));
    this->m_serialUpdateTimer->stop();
}

void SerialTerminal::resumeUpdates()
{
    connect(this->m_serialUpdateTimer.get(), SIGNAL(timeout()), this, SLOT(updateSerialTerminal()));
    this->m_serialUpdateTimer->start();
    try {
        this->m_serialReportFuture->start();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    this->m_cancelScript = false;
}

std::shared_ptr<Arduino> SerialTerminal::arduino()
{
    return this->m_arduino;
}

SerialReport SerialTerminal::serialReport(SerialTerminal *serialTerminal)
{
    using namespace ArduinoPCStrings;
    try {
        return serialTerminal->arduino()->serialReportRequest(serialTerminal->screenIndex());
    } catch (std::exception &e) {
        std::cout << STD_EXCEPTION_IN_SERIAL_REPORT_STRING << e.what() << std::endl;
        return SerialReport();
    }
}

void SerialTerminal::appendSerialReport(const SerialReport &report)
{
    for (auto &it : report.serialResults()) {
        printRxResult(it);
    }
}

void SerialTerminal::appendReceivedString(const std::string &str)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    if ((str != "") && (!isWhitespace(str)) && startsWith(str, '{')) {
        printRxResult(str);
    }
}

void SerialTerminal::appendTransmittedString(const QString &str)
{
    using namespace ArduinoPCStrings;
    this->m_commandHistory.insert(this->m_commandHistory.begin(), this->m_uiPtr->sendBox->text());
    resetCommandHistory();
    this->m_arduino->writeRawString(str.toStdString());
    printTxResult(str.toStdString());
    this->m_uiPtr->sendBox->clear();
}

void SerialTerminal::bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget)
{
    this->m_qDesktopWidget.reset();
    this->m_qDesktopWidget = qDesktopWidget;
}

void SerialTerminal::bindArduinoPCIcons(std::shared_ptr<ArduinoPCIcons> apciPtr)
{
    this->m_apciPtr.reset();
    this->m_apciPtr = apciPtr;
}

void SerialTerminal::bindGlobalLogger(std::shared_ptr<GlobalLogger> globalLogger)
{
    this->m_globalLogger.reset();
    this->m_globalLogger = globalLogger;
}

void SerialTerminal::centerAndFitWindow()
{
    calculateXYPlacement();
    this->move(this->m_xPlacement, this->m_yPlacement);
}

int SerialTerminal::xPlacement() const
{
    return this->m_xPlacement;
}

int SerialTerminal::yPlacement() const
{
    return this->m_yPlacement;
}

void SerialTerminal::calculateXYPlacement()
{
    std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qDesktopWidget->availableGeometry())};
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
    this->m_yPlacement = (avail->height()/2)-(this->height()/2) - MainWindow::TASKBAR_HEIGHT();
}

void SerialTerminal::resetCommandHistory()
{
    this->m_currentHistoryIndex = 0;
    this->m_currentLinePushedIntoCommandHistory = false;
    clearEmptyStringsFromCommandHistory();
}

void SerialTerminal::updateSerialTerminal()
{
    if (this->m_serialReportFuture->isCompleted()) {
        pauseUpdates();
        appendSerialReport(this->m_serialReportFuture->result());
        resumeUpdates();
    }
}

void SerialTerminal::clearEmptyStringsFromCommandHistory()
{
    using namespace GeneralUtilities;
    while (true) {
        int i{0};
        for (auto &it : this->m_commandHistory) {
            if ((isWhitespace(it.toStdString())) || (it.toStdString() == "")) {
                this->m_commandHistory.erase(this->m_commandHistory.begin() + i);
                break;
            }
            i++;
        }
        break;
    }
}

void SerialTerminal::onSendButtonClicked()
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    if (this->m_uiPtr->sendButton->text() == toQString(SEND_STRING)) {
        if ((this->m_uiPtr->sendBox->text().toStdString() != "") && (!isWhitespace(this->m_uiPtr->sendBox->text().toStdString()))) {
            appendTransmittedString(toQString(stripLineEndings(this->m_uiPtr->sendBox->text().toStdString())));
        }
        this->m_uiPtr->sendBox->setFocus();
    } else {
        this->m_cancelScript = true;
    }
}

void SerialTerminal::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    this->m_cancelScript = true;
    pauseUpdates();
    this->m_serialReportFuture->stop();
    this->m_uiPtr->terminal->clear();
    this->m_uiPtr->sendBox->clear();
    emit(aboutToClose(this->m_screenIndex));
    event->accept();
    /*
    using namespace ArduinoPCStrings;
    QMessageBox::StandardButton userReply;
    userReply = QMessageBox::question(this, QUIT_PROMPT_WINDOW_TITLE_STRING, QUIT_PROMPT_STRING, QMessageBox::Yes|QMessageBox::No);
    if (userReply == QMessageBox::Yes) {
        event->accept();
        QSerialTerminal::closeEvent(event);
    } else {
        event->ignore();
    }
    */
}

void SerialTerminal::onCommandHistoryContextMenuRequested(const QPoint &point)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    QMenu commandHistoryContextMenu(COMMAND_HISTORY_CONTEXT_MENU_STRING, this);
    int i{0};
    for (auto &it : this->m_commandHistory) {
        if ((isWhitespace(it.toStdString())) || (it.toStdString() == "")) {
            i++;
            continue;
        }
        SerialCommandHistoryAction *tempAction{new SerialCommandHistoryAction{it, i++, this}};
        commandHistoryContextMenu.addAction(tempAction);
        connect(tempAction, SIGNAL(triggered(SerialCommandHistoryAction *,bool)), this, SLOT(onCommandHistoryContextMenuActionTriggered(SerialCommandHistoryAction *, bool)));
    }
    commandHistoryContextMenu.exec(mapToGlobal(point));
}

void SerialTerminal::onCommandHistoryContextMenuActionTriggered(SerialCommandHistoryAction *action, bool checked)
{
    (void)checked;
    if (action) {
        this->m_currentHistoryIndex = action->index();
        this->m_uiPtr->sendBox->setText(action->text());
    }
}

void SerialTerminal::printRxResult(const std::string &str)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    if ((str != "") && (!isWhitespace(str)) && startsWith(str, '{')) {
        this->m_uiPtr->terminal->setTextColor(QColor(RED_COLOR_STRING));
        std::string returnString{stripLineEndings(stripNonAsciiCharacters(str))};
        std::vector<std::string> stringsToPrint;
        if (returnString.find("}{") != std::string::npos) {
            while (returnString.find("}{") != std::string::npos) {
                stringsToPrint.emplace_back(returnString.substr(0, returnString.find("}{")+1));
                returnString = returnString.substr(returnString.find("}{") + static_cast<std::string>("}{").length()-1);
            }
        } else {
            stringsToPrint.emplace_back(returnString);
        }
        for (auto &it : stringsToPrint) {
            this->m_uiPtr->terminal->append(toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + toQString(TERMINAL_RECEIVE_BASE_STRING) + toQString(it));
        }
    }
}

void SerialTerminal::printTxResult(const std::string &str)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    this->m_uiPtr->terminal->setTextColor(QColor(BLUE_COLOR_STRING));
    this->m_uiPtr->terminal->append(toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + toQString(TERMINAL_TRANSMIT_BASE_STRING) + toQString(str));
}

void SerialTerminal::printDelayResult(DelayType delayType, int howLong)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    this->m_uiPtr->terminal->setTextColor(QColor(GREEN_COLOR_STRING));
    QString stringToAppend{toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + toQString(TERMINAL_DELAY_BASE_STRING) + toQString(howLong)};
    if (delayType == DelayType::SECONDS) {
        stringToAppend.append(SECONDS_SUFFIX_STRING);
    } else if (delayType == DelayType::MILLISECONDS) {
        stringToAppend.append(MILLISECONDS_SUFFIX_STRING);
    } else if (delayType == DelayType::MICROSECONDS) {
        stringToAppend.append(MICROSECONDS_SUFFIX_STRING);
    }
    this->m_uiPtr->terminal->append(stringToAppend);
}

void SerialTerminal::printFlushResult(FlushType flushType)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    QString stringToAppend{""};

    if (flushType == FlushType::RX) {
        stringToAppend.append(TERMINAL_FLUSH_RX_BASE_STRING);
    } else if (flushType == FlushType::TX) {
        stringToAppend.append(TERMINAL_FLUSH_TX_BASE_STRING);
    } else if (flushType == FlushType::RX_TX) {
        stringToAppend.append(TERMINAL_FLUSH_RX_TX_BASE_STRING);
    }
    this->m_uiPtr->terminal->setTextColor(QColor(GRAY_COLOR_STRING));
    this->m_uiPtr->terminal->append(toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + stringToAppend);
    this->m_uiPtr->terminal->setTextColor(QColor());
}

QApplication *SerialTerminal::application()
{
    return qApp;
}


void SerialTerminal::printLoopResult(LoopType loopType, int currentLoop, int loopCount)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    QString stringToAppend{""};
    this->m_uiPtr->terminal->setTextColor(QColor(ORANGE_COLOR_STRING));
    if (loopCount == -1) {
        if (loopType == LoopType::START) {
            if (currentLoop == 0) {
                this->m_uiPtr->terminal->append(toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + BEGINNING_INFINITE_LOOP_STRING);
            }
            this->m_uiPtr->terminal->append(toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + toQString(BEGIN_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString(INFINITE_LOOP_COUNT_TAIL_STRING));
        } else if (loopType == LoopType::END) {
            this->m_uiPtr->terminal->append(toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + toQString(END_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString(INFINITE_LOOP_COUNT_TAIL_STRING));
        }
    } else {
        if (loopType == LoopType::START) {
            if (currentLoop == 0) {
                this->m_uiPtr->terminal->append(toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + toQString(BEGINNING_LOOPS_BASE_STRING) + toQString(loopCount) + toQString(LOOPS_TAIL_STRING));
            }
            this->m_uiPtr->terminal->append(toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + toQString(BEGIN_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString("/") + toQString(loopCount) + toQString(")"));
        } else if (loopType == LoopType::END) {
            this->m_uiPtr->terminal->append(toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + toQString(END_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString("/") + toQString(loopCount) + toQString(")"));
            if ((currentLoop+1) == loopCount) {
                this->m_uiPtr->terminal->append(toQString(tWhitespace(SerialTerminal::s_SERIAL_SCRIPT_INDENT)) + toQString(ENDING_LOOPS_BASE_STRING) + toQString(loopCount) + toQString(LOOPS_TAIL_STRING));
            }
        }
        this->m_uiPtr->terminal->setTextColor(QColor());
    }
}

void SerialTerminal::staticPrintRxResult(SerialTerminal *SerialTerminal, const std::string &str)
{
    if (SerialTerminal) {
        SerialTerminal->printRxResult(str);
    }
}

void SerialTerminal::staticPrintTxResult(SerialTerminal *SerialTerminal, const std::string &str)
{
    if (SerialTerminal) {
        SerialTerminal->printTxResult(str);
    }
}

void SerialTerminal::staticPrintDelayResult(SerialTerminal *SerialTerminal,  DelayType delayType, int howLong)
{
    if (SerialTerminal) {
        SerialTerminal->printDelayResult(delayType, howLong);
    }
}

void SerialTerminal::staticPrintFlushResult(SerialTerminal *SerialTerminal, FlushType flushType)
{
    if (SerialTerminal) {
        SerialTerminal->printFlushResult(flushType);
    }
}

void SerialTerminal::staticPrintLoopResult(SerialTerminal *SerialTerminal, LoopType loopType, int currentLoop, int loopCount)
{
    if (SerialTerminal) {
        SerialTerminal->printLoopResult(loopType, currentLoop, loopCount);
    }
}


void SerialTerminal::onActionLoadScriptTriggered(bool checked)
{
    (void)checked;
    pauseUpdates();
    this->m_serialReportFuture->stop();
    using namespace ArduinoPCStrings;
    QString defaultScriptFileDirectory{""};
    QDir homeCheck{OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY};
    if (homeCheck.exists()) {
        defaultScriptFileDirectory = homeCheck.absolutePath();
    } else {
        homeCheck = BACKUP_OPEN_SCRIPT_FILE_DIRECTORY;
        if (homeCheck.exists()) {
            defaultScriptFileDirectory = homeCheck.absolutePath();
        } else {
            homeCheck = LAST_CHANCE_OPEN_SCRIPT_FILE_DIRECTORY;
            if (homeCheck.exists()) {
                defaultScriptFileDirectory = homeCheck.absolutePath();
            }
        }
    }

    QFile file{QFileDialog::getOpenFileName(this, OPEN_SCRIPT_FILE_CAPTION, defaultScriptFileDirectory)};
    if (file.fileName() == "") {
        return;
    }
    if (file.exists()) {
        std::unique_ptr<SerialScriptExecutor> scriptExecutor{std::make_unique<SerialScriptExecutor>(file.fileName().toStdString())};
        if (scriptExecutor->scriptReader()->commands()->empty()) {
            std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
            warningBox->setText(EMPTY_SCRIPT_STRING + file.fileName());
            warningBox->setWindowTitle(EMPTY_SCRIPT_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
            warningBox->exec();
        } else {
            this->m_uiPtr->terminal->append("");
            this->m_uiPtr->terminal->append(EXECUTING_SCRIPT_STRING + file.fileName());
            this->m_uiPtr->sendBox->setEnabled(false);
            this->m_uiPtr->sendButton->setText(CANCEL_SCRIPT_STRING);
            scriptExecutor->execute(this,
                                    this->m_arduino,
                                    this->m_packagedRxResultTask,
                                    this->m_packagedTxResultTask,
                                    this->m_packagedDelayResultTask,
                                    this->m_packagedFlushResultTask,
                                    this->m_packagedLoopResultTask);
           this->m_uiPtr->terminal->setTextColor(QColor());
            if (this->m_cancelScript) {
                this->m_uiPtr->terminal->append(CANCELED_EXECUTING_SCRIPT_STRING + file.fileName());
            } else {
                this->m_uiPtr->terminal->append(FINISHED_EXECUTING_SCRIPT_STRING + file.fileName());
            }
            this->m_cancelScript = false;
            this->m_uiPtr->sendBox->setEnabled(false);
            this->m_uiPtr->sendBox->setFocus();
            this->m_uiPtr->sendButton->setText(SEND_STRING);
        }
    } else {
        std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
        warningBox->setText(FILE_DOES_NOT_EXIST_STRING + file.fileName());
        warningBox->setWindowTitle(FILE_DOES_NOT_EXIST_WINDOW_TITLE_STRING);
        warningBox->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
        warningBox->exec();
    }
    this->m_uiPtr->sendBox->setEnabled(true);
    resumeUpdates();
}

bool SerialTerminal::cancelScript() const
{
    return this->m_cancelScript;
}

void SerialTerminal::keyPressEvent(QKeyEvent *qke)
{
    if (qke) {
        if ((qke->key() == Qt::Key_Enter) || (qke->key() == Qt::Key_Return)) {
            this->parentWidget()->setFocus();
            onReturnKeyPressed();
        } else if (qke->key() == Qt::Key_Up) {
            onUpArrowPressed();
        } else if (qke->key() == Qt::Key_Down) {
            onDownArrowPressed();
        } else if (qke->key() == Qt::Key_Escape) {
            onEscapeKeyPressed();
        } else if (qke->key() == Qt::Key_Alt) {
            onAltKeyPressed();
        } else if ((qke->key() == Qt::Key_A) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            onCtrlAPressed();
        } else if ((qke->key() == Qt::Key_E) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            onCtrlEPressed();
        } else if ((qke->key() == Qt::Key_U) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            onCtrlUPressed();
        } else if ((qke->key() == Qt::Key_G) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            onCtrlGPressed();
        } else if ((qke->key() == Qt::Key_C) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            onCtrlCPressed();
        }
        else {
            return QWidget::keyPressEvent(qke);
        }
    }
}


void SerialTerminal::onReturnKeyPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    onReturnKeyPressed();
}

void SerialTerminal::onUpArrowPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    onUpArrowPressed();
}

void SerialTerminal::onDownArrowPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    onDownArrowPressed();
}

void SerialTerminal::onEscapeKeyPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    onEscapeKeyPressed();
}

void SerialTerminal::onAltKeyPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    onAltKeyPressed();
}

void SerialTerminal::onCtrlAPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    onCtrlAPressed();
}

void SerialTerminal::onCtrlEPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    onCtrlEPressed();
}

void SerialTerminal::onCtrlUPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    onCtrlUPressed();
}

void SerialTerminal::onCtrlGPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    onCtrlGPressed();
}

void SerialTerminal::onCtrlCPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    onCtrlCPressed();
}

void SerialTerminal::onReturnKeyPressed()
{
    using namespace GeneralUtilities;
    this->m_uiPtr->sendButton->click();
}

void SerialTerminal::onUpArrowPressed()
{
    if (this->m_currentHistoryIndex >= this->m_commandHistory.size()-1) {
        return;
    }
    if (this->m_currentHistoryIndex == 0) {
        if (!this->m_currentLinePushedIntoCommandHistory) {
            this->m_commandHistory.insert(this->m_commandHistory.begin(), this->m_uiPtr->sendBox->text());
            this->m_currentLinePushedIntoCommandHistory = true;
        }
    }
    this->m_currentHistoryIndex++;
    this->m_uiPtr->sendBox->setText(this->m_commandHistory.at(this->m_currentHistoryIndex));
}

void SerialTerminal::onDownArrowPressed()
{
    if (this->m_currentHistoryIndex == 0) {
        return;
    }
    this->m_uiPtr->sendBox->setText(this->m_commandHistory.at(--this->m_currentHistoryIndex));
}

void SerialTerminal::onEscapeKeyPressed()
{

}

void SerialTerminal::onAltKeyPressed()
{

}

void SerialTerminal::onCtrlAPressed()
{
     this->m_uiPtr->sendBox->setCursorPosition(0);
}

void SerialTerminal::onCtrlEPressed()
{
    this->m_uiPtr->sendBox->setCursorPosition(this->m_uiPtr->sendBox->text().size());
}

void SerialTerminal::onCtrlUPressed()
{
    this->m_uiPtr->sendBox->clear();
}

void SerialTerminal::onCtrlGPressed()
{
    this->m_uiPtr->terminal->clear();
}

void SerialTerminal::onCtrlCPressed()
{
    using namespace ArduinoPCStrings;
    if (this->m_uiPtr->sendButton->text() == toQString(CANCEL_SCRIPT_STRING)) {
        this->m_cancelScript = true;
    }
}
