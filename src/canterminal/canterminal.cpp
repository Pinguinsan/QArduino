#include "canterminal/canterminal.h"
#include "ui_canterminal.h"

const int CanTerminal::s_CAN_TERMINAL_UPDATE_TIME{10};
const int CanTerminal::s_CAN_TIMEOUT{400};
const int CanTerminal::s_CAN_SCRIPT_INDENT{0};
const int CanTerminal::s_CAN_STATUS_BAR_TIMEOUT{4000};

CanTerminal::CanTerminal(int screenIndex,
                         std::shared_ptr<ArduinoPCIcons> apciPtr,
                         std::shared_ptr<QDesktopWidget> qDesktopWidget,
                         std::shared_ptr<GlobalLogger> globalLogger,
                         std::shared_ptr<Arduino> arduino,
                         QWidget *parent) :
    QMainWindow{parent},
    m_screenIndex{screenIndex},
    m_uiPtr{std::make_shared<Ui::CanTerminal>()},
    m_apciPtr{apciPtr},
    m_qDesktopWidget{qDesktopWidget},
    m_globalLogger{globalLogger},
    m_arduino{arduino},
    m_packagedCanReportTask{CanTerminal::canReport},
    m_canReportFuture{std::make_unique<SmartCanReportFuture>(this, this->m_packagedCanReportTask)},
    m_canUpdateTimer{std::make_unique<QTimer>()},
    m_packagedRxResultTask{CanTerminal::staticPrintRxResult},
    m_packagedTxResultTask{CanTerminal::staticPrintTxResult},
    m_packagedDelayResultTask{CanTerminal::staticPrintDelayResult},
    m_packagedFlushResultTask{CanTerminal::staticPrintFlushResult},
    m_packagedLoopResultTask{CanTerminal::staticPrintLoopResult},
    m_currentLinePushedIntoCommandHistory{false},
    m_currentHistoryIndex{0},
    m_currentLinePushedIntoPositiveCanMaskCommandHistory{false},
    m_currentPositiveLineEditHistoryIndex{0},
    m_currentLinePushedIntoNegativeCanMaskCommandHistory{false},
    m_currentNegativeLineEditHistoryIndex{0},
    m_cancelScript{false},
    m_xPlacement{0},
    m_yPlacement{0}
{
    using namespace ArduinoPCStrings;
    this->m_uiPtr->setupUi(this);
    this->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);

    connect(this->m_uiPtr->actionClose, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(this->m_uiPtr->actionLoadScript, SIGNAL(triggered(bool)), this, SLOT(onActionLoadScriptTriggered(bool)));

    this->m_canTerminalLineEdits.push_back(this->m_uiPtr->canMessageIDLineEdit);
    this->m_canTerminalLineEdits.push_back(this->m_uiPtr->canByte0LineEdit);
    this->m_canTerminalLineEdits.push_back(this->m_uiPtr->canByte1LineEdit);
    this->m_canTerminalLineEdits.push_back(this->m_uiPtr->canByte2LineEdit);
    this->m_canTerminalLineEdits.push_back(this->m_uiPtr->canByte3LineEdit);
    this->m_canTerminalLineEdits.push_back(this->m_uiPtr->canByte4LineEdit);
    this->m_canTerminalLineEdits.push_back(this->m_uiPtr->canByte5LineEdit);
    this->m_canTerminalLineEdits.push_back(this->m_uiPtr->canByte6LineEdit);
    this->m_canTerminalLineEdits.push_back(this->m_uiPtr->canByte7LineEdit);

    this->m_uiPtr->canMessageIDLineEdit->setTabOrder(this->m_uiPtr->canByte7LineEdit, this->m_uiPtr->canMessageIDLineEdit);
    this->m_uiPtr->canByte0LineEdit->setTabOrder(this->m_uiPtr->canMessageIDLineEdit, this->m_uiPtr->canByte0LineEdit);
    this->m_uiPtr->canByte1LineEdit->setTabOrder(this->m_uiPtr->canByte0LineEdit, this->m_uiPtr->canByte1LineEdit);
    this->m_uiPtr->canByte2LineEdit->setTabOrder(this->m_uiPtr->canByte1LineEdit, this->m_uiPtr->canByte2LineEdit);
    this->m_uiPtr->canByte3LineEdit->setTabOrder(this->m_uiPtr->canByte2LineEdit, this->m_uiPtr->canByte3LineEdit);
    this->m_uiPtr->canByte4LineEdit->setTabOrder(this->m_uiPtr->canByte3LineEdit, this->m_uiPtr->canByte4LineEdit);
    this->m_uiPtr->canByte5LineEdit->setTabOrder(this->m_uiPtr->canByte4LineEdit, this->m_uiPtr->canByte5LineEdit);
    this->m_uiPtr->canByte6LineEdit->setTabOrder(this->m_uiPtr->canByte5LineEdit, this->m_uiPtr->canByte6LineEdit);
    this->m_uiPtr->canByte7LineEdit->setTabOrder(this->m_uiPtr->canByte6LineEdit, this->m_uiPtr->canByte7LineEdit);

    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(returnPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskReturnKeyPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(upArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskUpArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(downArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskDownArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(leftArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskLeftArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(rightArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskRightArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(escapePressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskEscapeKeyPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(altPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskAltKeyPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(ctrlAPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskCtrlAPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(ctrlEPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskCtrlEPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(ctrlUPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskCtrlUPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->positiveCanMaskLineEdit, SIGNAL(ctrlGPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onPositiveCanMaskCtrlGPressed(CanTerminalLineEdit*,unsigned int,int)));

    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(returnPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskReturnKeyPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(upArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskUpArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(downArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskDownArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(leftArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskLeftArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(rightArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskRightArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(escapePressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskEscapeKeyPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(altPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskAltKeyPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(ctrlAPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskCtrlAPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(ctrlEPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskCtrlEPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(ctrlUPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskCtrlUPressed(CanTerminalLineEdit*,unsigned int,int)));
    connect(this->m_uiPtr->negativeCanMaskLineEdit, SIGNAL(ctrlGPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onNegativeCanMaskCtrlGPressed(CanTerminalLineEdit*,unsigned int,int)));


    connect(this->m_uiPtr->positiveCanMaskSetButton, SIGNAL(clicked(bool)), this, SLOT(onPositiveCanMaskSendButtonClicked(bool)));
    connect(this->m_uiPtr->positiveCanMaskRemoveButton, SIGNAL(clicked(bool)), this, SLOT(onPositiveCanMaskRemoveButtonClicked(bool)));
    connect(this->m_uiPtr->positiveCanMaskRemoveAllButton, SIGNAL(clicked(bool)), this, SLOT(onPositiveCanMaskRemoveAllButtonClicked(bool)));
    connect(this->m_uiPtr->negativeCanMaskSetButton, SIGNAL(clicked(bool)), this, SLOT(onNegativeCanMaskSendButtonClicked(bool)));
    connect(this->m_uiPtr->negativeCanMaskRemoveButton, SIGNAL(clicked(bool)), this, SLOT(onNegativeCanMaskRemoveButtonClicked(bool)));
    connect(this->m_uiPtr->negativeCanMaskRemoveAllButton, SIGNAL(clicked(bool)), this, SLOT(onNegativeCanMaskRemoveAllButtonClicked(bool)));
    this->m_uiPtr->positiveCanMaskLineEdit->setText(CAN_ID_LINE_EDIT_ZERO_STRING);
    this->m_uiPtr->negativeCanMaskLineEdit->setText(CAN_ID_LINE_EDIT_ZERO_STRING);

    unsigned int byteOrderIndex{0};
    for (auto &it : this->m_canTerminalLineEdits) {
        it->setupParameters(++byteOrderIndex, screenIndex);
        connect(it, SIGNAL(editingFinished(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onCanTerminalLineEditEditingFinished(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(returnPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onReturnKeyPressed(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(upArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onUpArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(downArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onDownArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(leftArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onLeftArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(rightArrowPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onRightArrowPressed(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(escapePressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onEscapeKeyPressed(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(altPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onAltKeyPressed(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(ctrlAPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onCtrlAPressed(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(ctrlEPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onCtrlEPressed(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(ctrlUPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onCtrlUPressed(CanTerminalLineEdit*,unsigned int,int)));
        connect(it, SIGNAL(ctrlGPressed(CanTerminalLineEdit*,unsigned int,int)), this, SLOT(onCtrlGPressed(CanTerminalLineEdit*,unsigned int,int)));
    }
    zeroOutCanBytes();
    connect(this->m_uiPtr->sendButton, SIGNAL(clicked(bool)), this, SLOT(onSendButtonClicked()));
    connect(this->m_uiPtr->clearButton, SIGNAL(clicked(bool)), this, SLOT(onClearButtonClicked()));
    this->m_canUpdateTimer->setInterval(CanTerminal::s_CAN_TERMINAL_UPDATE_TIME);

}

void CanTerminal::hideEvent(QHideEvent *event)
{
    (void)event;
    if (this->isHidden()) {
        pauseUpdates();
    } else {
        resumeUpdates();
    }
    return QMainWindow::hideEvent(event);
}

void CanTerminal::showEvent(QShowEvent *event)
{
    if (!this->isHidden()) {
        resumeUpdates();
    } else {
        pauseUpdates();
    }
    (void)event;
    return QMainWindow::showEvent(event);
}


void CanTerminal::pauseUpdates()
{
    disconnect(this->m_canUpdateTimer.get(), SIGNAL(timeout()), this, SLOT(updateCanTerminal()));
    this->m_canUpdateTimer->start();
}

void CanTerminal::resumeUpdates()
{
    connect(this->m_canUpdateTimer.get(), SIGNAL(timeout()), this, SLOT(updateCanTerminal()));
    this->m_canReportFuture->start();
    try {
        this->m_canUpdateTimer->start();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

}

std::shared_ptr<Arduino> CanTerminal::arduino() const
{
    return this->m_arduino;
}

CanReport CanTerminal::canReport(CanTerminal *canTerminal)
{
    using namespace ArduinoPCStrings;
    try {
        return canTerminal->arduino()->canReportRequest(canTerminal->screenIndex());
    } catch (std::exception &e) {
        std::cout << STD_EXCEPTION_IN_CAN_REPORT_STRING << e.what() << std::endl;
        return CanReport();
    }
}

void CanTerminal::onCommandHistoryContextMenuRequested(const QPoint &point)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    QMenu commandHistoryContextMenu(COMMAND_HISTORY_CONTEXT_MENU_STRING, this);
    CanMessage canMessage{};
    canMessage.setID(std::stoi(this->m_uiPtr->canMessageIDLineEdit->text().toStdString()));
    canMessage.setLength(Arduino::CAN_FRAME);
    canMessage.setLength(Arduino::CAN_MESSAGE_LENGTH);
    int byteOrderIndex{1};
    for (auto &it : this->m_canTerminalLineEdits) {
        canMessage.setDataPacketNthByte(std::stoi(it->text().toStdString()), byteOrderIndex++);
    }
    int i{0};
    for (auto &it : this->m_commandHistory) {
        QString stringToAppend{""};
        for (auto &innerIt : it) {
            if ((isWhitespace(innerIt.toStdString())) || (innerIt.toStdString() == "")) {
                continue;
            } else {
                stringToAppend.append(":" + innerIt);
            }
            if (stringToAppend.length() == 0) {
                i++;
                continue;
            }
        }
        CanCommandHistoryAction *tempAction{new CanCommandHistoryAction{stringToAppend, i++, canMessage, this}};
        commandHistoryContextMenu.addAction(tempAction);
        connect(tempAction, SIGNAL(triggered(CanCommandHistoryAction *,bool)), this, SLOT(onCommandHistoryContextMenuActionTriggered(CanCommandHistoryAction *, bool)));
    }
    commandHistoryContextMenu.exec(mapToGlobal(point));
}

void CanTerminal::onCommandHistoryContextMenuActionTriggered(CanCommandHistoryAction *action, bool checked)
{
    (void)checked;
    if (action) {
        this->m_currentHistoryIndex = action->index();
        CanMessage canMessage{action->canMessage()};
        int byteOrderIndex{0};
        int iteration{0};
        for (auto &it : this->m_canTerminalLineEdits) {
            if (iteration++ == 0) {
                it->setText(toQString(canMessage.id()));
            } else {
                it->setText(toQString(canMessage.nthDataPacketByte(byteOrderIndex++)));
            }
        }
    }
}

void CanTerminal::appendString(const std::string &str)
{
    using namespace ArduinoPCStrings;
    std::unique_lock<std::mutex>{*CanTerminalScheduler::canTerminalMutexByScreenIndex(this->m_screenIndex)};
    this->m_uiPtr->terminal->setTextColor(QColor(RED_COLOR_STRING));
    this->m_uiPtr->terminal->append(QString::fromStdString(TERMINAL_RECEIVE_BASE_STRING) + QString::fromStdString(str));
}

void CanTerminal::updateCanTerminal()
{
    using namespace GeneralUtilities;
    if (this->m_canReportFuture->isCompleted()) {
        pauseUpdates();
        appendCanReport(this->m_canReportFuture->result());
        resumeUpdates();
    }
}

void CanTerminal::onClearButtonClicked()
{
    zeroOutCanBytes();
}

void CanTerminal::zeroOutCanBytes()
{
    using namespace ArduinoPCStrings;
    for (auto &it : this->m_canTerminalLineEdits) {
        it->setText(CAN_BYTES_LINE_EDIT_ZERO_STRING);
    }
    this->m_uiPtr->canMessageIDLineEdit->setText(CAN_ID_LINE_EDIT_ZERO_STRING);
}

void CanTerminal::onSendButtonClicked()
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    std::unique_lock<std::mutex>{*CanTerminalScheduler::canTerminalMutexByScreenIndex(this->m_screenIndex)};
    std::string message{HEXADECIMAL_BASE_STRING + toFixedWidth(this->m_uiPtr->canMessageIDLineEdit->text().toStdString(), Arduino::CAN_ID_WIDTH)};
    message += toString(":") + HEXADECIMAL_BASE_STRING + toFixedWidth(validateCanByte(this->m_uiPtr->canByte0LineEdit->text().toStdString()), Arduino::CAN_BYTE_WIDTH);
    message += toString(":") + HEXADECIMAL_BASE_STRING + toFixedWidth(validateCanByte(this->m_uiPtr->canByte1LineEdit->text().toStdString()), Arduino::CAN_BYTE_WIDTH);
    message += toString(":") + HEXADECIMAL_BASE_STRING + toFixedWidth(validateCanByte(this->m_uiPtr->canByte2LineEdit->text().toStdString()), Arduino::CAN_BYTE_WIDTH);
    message += toString(":") + HEXADECIMAL_BASE_STRING + toFixedWidth(validateCanByte(this->m_uiPtr->canByte3LineEdit->text().toStdString()), Arduino::CAN_BYTE_WIDTH);
    message += toString(":") + HEXADECIMAL_BASE_STRING + toFixedWidth(validateCanByte(this->m_uiPtr->canByte4LineEdit->text().toStdString()), Arduino::CAN_BYTE_WIDTH);
    message += toString(":") + HEXADECIMAL_BASE_STRING + toFixedWidth(validateCanByte(this->m_uiPtr->canByte5LineEdit->text().toStdString()), Arduino::CAN_BYTE_WIDTH);
    message += toString(":") + HEXADECIMAL_BASE_STRING + toFixedWidth(validateCanByte(this->m_uiPtr->canByte6LineEdit->text().toStdString()), Arduino::CAN_BYTE_WIDTH);
    message += toString(":") + HEXADECIMAL_BASE_STRING + toFixedWidth(validateCanByte(this->m_uiPtr->canByte7LineEdit->text().toStdString()), Arduino::CAN_BYTE_WIDTH);
    this->m_uiPtr->terminal->setTextColor(QColor(BLUE_COLOR_STRING));
    CanMessage msg{CanMessage::parseCanMessage(message)};
    this->m_uiPtr->terminal->append(TERMINAL_TRANSMIT_BASE_STRING + toQString(msg.toPrettyString()));
    this->m_arduino->canWrite(msg, this->m_screenIndex);

}

void CanTerminal::appendCanReport(const CanReport &report)
{
    for (auto &it : report.canMessageResults()) {
        printRxResult(it.toPrettyString());
    }
}

void CanTerminal::appendCanMessage(const CanMessage &message)
{
    printRxResult(message.toPrettyString());
}

void CanTerminal::onCanTerminalLineEditEditingFinished(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)byteOrderIndex;
    if (!ctle) {
        return;
    }
    ctle->setText(QString::fromStdString(validateCanByte(ctle->text().toStdString())));
}

void CanTerminal::clearEmptyStringsFromPositiveCanMaskCommandHistory()
{
    using namespace GeneralUtilities;
    while (true) {
        int i{0};
        for (auto &it : this->m_positiveCanMaskCommandHistory) {
            if ((isWhitespace(it.toStdString())) || (it.toStdString() == "")) {
                this->m_positiveCanMaskCommandHistory.erase(this->m_positiveCanMaskCommandHistory.begin() + i);
                break;
            }
            i++;
        }
        break;
    }
}

void CanTerminal::onPositiveCanMaskReturnKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    this->m_uiPtr->positiveCanMaskLineEdit->setFocus();
    this->m_uiPtr->positiveCanMaskSetButton->click();
}

void CanTerminal::onPositiveCanMaskUpArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    if (this->m_currentPositiveLineEditHistoryIndex >= this->m_positiveCanMaskCommandHistory.size()-1) {
        return;
    }
    if (this->m_currentHistoryIndex == 0) {
        if (!this->m_currentLinePushedIntoPositiveCanMaskCommandHistory) {
            this->m_positiveCanMaskCommandHistory.insert(this->m_positiveCanMaskCommandHistory.begin(), this->m_uiPtr->positiveCanMaskLineEdit->text());
            this->m_currentLinePushedIntoPositiveCanMaskCommandHistory = true;
        }
    }
    this->m_currentPositiveLineEditHistoryIndex++;
    this->m_uiPtr->positiveCanMaskLineEdit->setText(this->m_positiveCanMaskCommandHistory.at(this->m_currentPositiveLineEditHistoryIndex-1));
}

void CanTerminal::onPositiveCanMaskDownArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    if (this->m_currentPositiveLineEditHistoryIndex == 0) {
        return;
    }
    this->m_uiPtr->positiveCanMaskLineEdit->setText(this->m_positiveCanMaskCommandHistory.at(--this->m_currentPositiveLineEditHistoryIndex));
}

void CanTerminal::onPositiveCanMaskLeftArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)byteOrderIndex;
    if (ctle->cursorPosition() != 0) {
        ctle->setCursorPosition(ctle->cursorPosition()-1);
    }
}

void CanTerminal::onPositiveCanMaskRightArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)byteOrderIndex;
    if (ctle->cursorPosition() != ctle->text().length()) {
        ctle->setCursorPosition(ctle->cursorPosition()+1);
    }
}

void CanTerminal::onPositiveCanMaskEscapeKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)byteOrderIndex;
    if (this->m_currentPositiveLineEditHistoryIndex != 0) {
        ctle->setText(this->m_positiveCanMaskCommandHistory.at(--this->m_currentPositiveLineEditHistoryIndex));
    }
    ctle->clearFocus();
}

void CanTerminal::onPositiveCanMaskAltKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
}

void CanTerminal::onPositiveCanMaskCtrlAPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    ctle->setCursorPosition(0);
}

void CanTerminal::onPositiveCanMaskCtrlEPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    ctle->setCursorPosition(ctle->text().size());
}

void CanTerminal::onPositiveCanMaskCtrlUPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    ctle->clear();
}

void CanTerminal::onPositiveCanMaskCtrlGPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
}

void CanTerminal::clearEmptyStringsFromNegativeCanMaskCommandHistory()
{
    using namespace GeneralUtilities;
    while (true) {
        int i{0};
        for (auto &it : this->m_negativeCanMaskCommandHistory) {
            if ((isWhitespace(it.toStdString())) || (it.toStdString() == "")) {
                this->m_negativeCanMaskCommandHistory.erase(this->m_negativeCanMaskCommandHistory.begin() + i);
                break;
            }
            i++;
        }
        break;
    }
}

void CanTerminal::onPositiveCanMaskCommandHistoryContextMenuRequested(const QPoint &point)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    QMenu commandHistoryContextMenu(COMMAND_HISTORY_CONTEXT_MENU_STRING, this);
    CanMessage canMessage;
    canMessage.setID(std::stoi(this->m_uiPtr->positiveCanMaskLineEdit->text().toStdString()));
    int i{0};
    for (auto &it : this->m_positiveCanMaskCommandHistory) {
        if ((isWhitespace(it.toStdString())) || (it.toStdString() == "")) {
            i++;
            continue;
        }
        CanCommandHistoryAction *tempAction{new CanCommandHistoryAction{it, i++, canMessage, this}};
        commandHistoryContextMenu.addAction(tempAction);
        connect(tempAction, SIGNAL(triggered(CanCommandHistoryAction *,bool)), this, SLOT(onPositiveCanMaskCommandHistoryContextMenuActionTriggered(SerialCommandHistoryAction *, bool)));
    }
    commandHistoryContextMenu.exec(mapToGlobal(point));
}

void CanTerminal::onNegativeCanMaskCommandHistoryContextMenuRequested(const QPoint &point)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    QMenu commandHistoryContextMenu(COMMAND_HISTORY_CONTEXT_MENU_STRING, this);
    CanMessage canMessage;
    canMessage.setID(std::stoi(this->m_uiPtr->negativeCanMaskLineEdit->text().toStdString()));
    int i{0};
    for (auto &it : this->m_negativeCanMaskCommandHistory) {
        if ((isWhitespace(it.toStdString())) || (it.toStdString() == "")) {
            i++;
            continue;
        }
        CanCommandHistoryAction *tempAction{new CanCommandHistoryAction{it, i++, canMessage, this}};
        commandHistoryContextMenu.addAction(tempAction);
        connect(tempAction, SIGNAL(triggered(CanCommandHistoryAction *,bool)), this, SLOT(onNegativeCanMaskCommandHistoryContextMenuActionTriggered(SerialCommandHistoryAction *, bool)));
    }
    commandHistoryContextMenu.exec(mapToGlobal(point));
}

void CanTerminal::onPositiveCanMaskCommandHistoryContextMenuActionTriggered(CanCommandHistoryAction *action, bool checked)
{
    (void)checked;
    if (action) {
        this->m_currentHistoryIndex = action->index();
        this->m_uiPtr->positiveCanMaskLineEdit->setText(toQString(action->canMessage().id()));
    }
}


void CanTerminal::onNegativeCanMaskCommandHistoryContextMenuActionTriggered(CanCommandHistoryAction *action, bool checked)
{
    (void)checked;
    if (action) {
        this->m_currentHistoryIndex = action->index();
        this->m_uiPtr->positiveCanMaskLineEdit->setText(toQString(action->canMessage().id()));
    }
}


void CanTerminal::resetNegativeCanMaskCommandHistory()
{
    this->m_currentHistoryIndex = 0;
    this->m_currentLinePushedIntoCommandHistory = false;
    clearEmptyStringsFromNegativeCanMaskCommandHistory();
}

void CanTerminal::resetPositiveCanMaskCommandHistory()
{
    this->m_currentPositiveLineEditHistoryIndex = 0;
    this->m_currentLinePushedIntoPositiveCanMaskCommandHistory = false;
    clearEmptyStringsFromPositiveCanMaskCommandHistory();
}

void CanTerminal::onPositiveCanMaskSendButtonClicked(bool checked)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    (void)checked;
    if ((this->m_uiPtr->positiveCanMaskLineEdit->text().toStdString() != "") && (!isWhitespace(this->m_uiPtr->positiveCanMaskLineEdit->text().toStdString()))) {
        std::unique_lock<std::mutex>{*CanTerminalScheduler::canTerminalMutexByScreenIndex(this->m_screenIndex)};
        std::string message{HEXADECIMAL_BASE_STRING + toFixedWidth(this->m_uiPtr->positiveCanMaskLineEdit->text().toStdString(), Arduino::CAN_ID_WIDTH)};
        this->m_positiveCanMaskCommandHistory.insert(this->m_positiveCanMaskCommandHistory.begin(), toQString(message));
        resetPositiveCanMaskCommandHistory();
        std::pair<IOStatus, uint32_t> result{this->m_arduino->addCanMask(CanMaskType::POSITIVE, message, this->m_screenIndex)};
        if (result.first == IOStatus::OPERATION_FAILURE) {
            std::cout << FAILED_TO_ADD_POSITIVE_CAN_MASK_STRING << message << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
            this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_ADD_POSITIVE_CAN_MASK_STRING) + toQString(message) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
        } else {
            std::cout << SUCCESSFULLY_ADDED_POSITIVE_CAN_MASK_STRING << message << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
            this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_ADDED_POSITIVE_CAN_MASK_STRING) + toQString(message) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
        }
        //this->m_uiPtr->positiveCanMaskLineEdit->setText(ArduinoPCStrings::CAN_ID_LINE_EDIT_ZERO_STRING);
    }
}

void CanTerminal::onPositiveCanMaskRemoveButtonClicked(bool checked)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    (void)checked;
    if ((this->m_uiPtr->positiveCanMaskLineEdit->text().toStdString() != "") && (!isWhitespace(this->m_uiPtr->positiveCanMaskLineEdit->text().toStdString()))) {
        std::unique_lock<std::mutex>{*CanTerminalScheduler::canTerminalMutexByScreenIndex(this->m_screenIndex)};
        std::string message{HEXADECIMAL_BASE_STRING + toFixedWidth(this->m_uiPtr->positiveCanMaskLineEdit->text().toStdString(), Arduino::CAN_ID_WIDTH)};
        resetPositiveCanMaskCommandHistory();
        this->m_positiveCanMaskCommandHistory.insert(this->m_positiveCanMaskCommandHistory.begin(), toQString(message));
        std::pair<IOStatus, uint32_t> result{this->m_arduino->removeCanMask(CanMaskType::POSITIVE, message, this->m_screenIndex)};
        if (result.first == IOStatus::OPERATION_FAILURE) {
            std::cout << FAILED_TO_REMOVE_POSITIVE_CAN_MASK_STRING << message << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
            this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_REMOVE_POSITIVE_CAN_MASK_STRING) + toQString(message) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
        } else {
            std::cout << SUCCESSFULLY_REMOVED_POSITIVE_CAN_MASK_STRING << message << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
            this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_REMOVED_POSITIVE_CAN_MASK_STRING) + toQString(message) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
        }
        this->m_uiPtr->positiveCanMaskLineEdit->setText(ArduinoPCStrings::CAN_ID_LINE_EDIT_ZERO_STRING);
    }
}
void CanTerminal::onPositiveCanMaskRemoveAllButtonClicked(bool checked)
{
    using namespace ArduinoPCStrings;
    (void)checked;
    std::pair<IOStatus, bool> result{this->m_arduino->removeAllCanMasks(CanMaskType::POSITIVE, this->m_screenIndex)};
    if (result.first == IOStatus::OPERATION_FAILURE) {
        std::cout << FAILED_TO_REMOVE_ALL_POSITIVE_CAN_MASKS_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
        this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_REMOVE_ALL_POSITIVE_CAN_MASKS_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
    } else {
        std::cout << SUCCESSFULLY_REMOVED_ALL_POSITIVE_CAN_MASKS_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
        this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_REMOVED_ALL_POSITIVE_CAN_MASKS_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
    }
}
/*
std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_SOFT_DIGITAL_READ_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
this->m_uiPtr->statusBar->showMessage(toQString(FAILED_SOFT_DIGITAL_READ_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
*/

void CanTerminal::onNegativeCanMaskSendButtonClicked(bool checked)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    (void)checked;
    if ((this->m_uiPtr->negativeCanMaskLineEdit->text().toStdString() != "") && (!isWhitespace(this->m_uiPtr->negativeCanMaskLineEdit->text().toStdString()))) {
        std::unique_lock<std::mutex>{*CanTerminalScheduler::canTerminalMutexByScreenIndex(this->m_screenIndex)};
        std::string message{HEXADECIMAL_BASE_STRING + toFixedWidth(this->m_uiPtr->negativeCanMaskLineEdit->text().toStdString(), Arduino::CAN_ID_WIDTH)};
        this->m_negativeCanMaskCommandHistory.insert(this->m_negativeCanMaskCommandHistory.begin(), toQString(message));
        resetNegativeCanMaskCommandHistory();
        std::pair<IOStatus, uint32_t> result{this->m_arduino->addCanMask(CanMaskType::NEGATIVE, message, this->m_screenIndex)};
        if (result.first == IOStatus::OPERATION_FAILURE) {
            std::cout << FAILED_TO_ADD_NEGATIVE_CAN_MASK_STRING << message << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
            this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_ADD_NEGATIVE_CAN_MASK_STRING) + toQString(message) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
        } else {
            std::cout << SUCCESSFULLY_ADDED_NEGATIVE_CAN_MASK_STRING << message << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
            this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_ADDED_NEGATIVE_CAN_MASK_STRING) + toQString(message) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
        }
        //this->m_uiPtr->negativeCanMaskLineEdit->setText(ArduinoPCStrings::CAN_ID_LINE_EDIT_ZERO_STRING);
    }
}

void CanTerminal::onNegativeCanMaskRemoveButtonClicked(bool checked)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    (void)checked;
    if ((this->m_uiPtr->negativeCanMaskLineEdit->text().toStdString() != "") && (!isWhitespace(this->m_uiPtr->negativeCanMaskLineEdit->text().toStdString()))) {
        std::unique_lock<std::mutex>{*CanTerminalScheduler::canTerminalMutexByScreenIndex(this->m_screenIndex)};
        std::string message{HEXADECIMAL_BASE_STRING + toFixedWidth(message, Arduino::CAN_ID_WIDTH)};
        resetNegativeCanMaskCommandHistory();
        this->m_negativeCanMaskCommandHistory.insert(this->m_negativeCanMaskCommandHistory.begin(), toQString(message));
        std::pair<IOStatus, uint32_t> result{this->m_arduino->removeCanMask(CanMaskType::NEGATIVE, this->m_uiPtr->negativeCanMaskLineEdit->text().toStdString(), this->m_screenIndex)};
        if (result.first == IOStatus::OPERATION_FAILURE) {
            std::cout << FAILED_TO_REMOVE_NEGATIVE_CAN_MASK_STRING << message << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
            this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_REMOVE_NEGATIVE_CAN_MASK_STRING) + toQString(message) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
        } else {
            std::cout << SUCCESSFULLY_REMOVED_NEGATIVE_CAN_MASK_STRING << message << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
            this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_REMOVED_NEGATIVE_CAN_MASK_STRING) + toQString(message) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
        }
        //this->m_uiPtr->negativeCanMaskLineEdit->setText(ArduinoPCStrings::CAN_ID_LINE_EDIT_ZERO_STRING);
    }
}

void CanTerminal::onNegativeCanMaskRemoveAllButtonClicked(bool checked)
{
    using namespace ArduinoPCStrings;
    (void)checked;
    std::pair<IOStatus, bool> result{this->m_arduino->removeAllCanMasks(CanMaskType::POSITIVE, this->m_screenIndex)};
    if (result.first == IOStatus::OPERATION_FAILURE) {
        std::cout << FAILED_TO_REMOVE_ALL_NEGATIVE_CAN_MASKS_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
        this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_REMOVE_ALL_NEGATIVE_CAN_MASKS_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
    } else {
        std::cout << SUCCESSFULLY_REMOVED_ALL_NEGATIVE_CAN_MASKS_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
        this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_REMOVED_ALL_NEGATIVE_CAN_MASKS_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", CanTerminal::s_CAN_STATUS_BAR_TIMEOUT);
    }
}

void CanTerminal::onNegativeCanMaskReturnKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    this->m_uiPtr->negativeCanMaskLineEdit->setFocus();
    this->m_uiPtr->negativeCanMaskSetButton->click();
}

void CanTerminal::onNegativeCanMaskUpArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    if (this->m_currentNegativeLineEditHistoryIndex >= this->m_negativeCanMaskCommandHistory.size()-1) {
        return;
    }
    if (this->m_currentHistoryIndex == 0) {
        if (!this->m_currentLinePushedIntoNegativeCanMaskCommandHistory) {
            this->m_negativeCanMaskCommandHistory.insert(this->m_negativeCanMaskCommandHistory.begin(), this->m_uiPtr->negativeCanMaskLineEdit->text());
            this->m_currentLinePushedIntoNegativeCanMaskCommandHistory = true;
        }
    }
    this->m_currentNegativeLineEditHistoryIndex++;
    this->m_uiPtr->negativeCanMaskLineEdit->setText(this->m_negativeCanMaskCommandHistory.at(this->m_currentNegativeLineEditHistoryIndex-1));
}

void CanTerminal::onNegativeCanMaskDownArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    if (this->m_currentNegativeLineEditHistoryIndex == 0) {
        return;
    }
    this->m_uiPtr->negativeCanMaskLineEdit->setText(this->m_negativeCanMaskCommandHistory.at(--this->m_currentNegativeLineEditHistoryIndex));
}

void CanTerminal::onNegativeCanMaskLeftArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)byteOrderIndex;
    if (ctle->cursorPosition() != 0) {
        ctle->setCursorPosition(ctle->cursorPosition()-1);
    }
}

void CanTerminal::onNegativeCanMaskRightArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)byteOrderIndex;
    if (ctle->cursorPosition() != ctle->text().length()) {
        ctle->setCursorPosition(ctle->cursorPosition()+1);
    }
}

void CanTerminal::onNegativeCanMaskEscapeKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)byteOrderIndex;
    if (this->m_currentNegativeLineEditHistoryIndex != 0) {
        ctle->setText(this->m_negativeCanMaskCommandHistory.at(--this->m_currentNegativeLineEditHistoryIndex));
    }
    ctle->clearFocus();
}

void CanTerminal::onNegativeCanMaskAltKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
}

void CanTerminal::onNegativeCanMaskCtrlAPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    ctle->setCursorPosition(0);
}

void CanTerminal::onNegativeCanMaskCtrlEPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    ctle->setCursorPosition(ctle->text().size());
}

void CanTerminal::onNegativeCanMaskCtrlUPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
    ctle->clear();
}

void CanTerminal::onNegativeCanMaskCtrlGPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)byteOrderIndex;
    (void)screenIndex;
}

void CanTerminal::printRxResult(const std::string &str)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    if ((str != "") && (!isWhitespace(str))) {
        this->m_uiPtr->terminal->setTextColor(QColor(RED_COLOR_STRING));
        this->m_uiPtr->terminal->append(toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + toQString(TERMINAL_RECEIVE_BASE_STRING) + toQString(stripLineEndings(stripNonAsciiCharacters(str))));
    }
}

void CanTerminal::printTxResult(const std::string &str)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    this->m_uiPtr->terminal->setTextColor(QColor(BLUE_COLOR_STRING));
    this->m_uiPtr->terminal->append(toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + toQString(TERMINAL_TRANSMIT_BASE_STRING) + toQString(str));
}

void CanTerminal::printDelayResult(DelayType delayType, int howLong)
{
    using namespace ArduinoPCStrings;
    using namespace GeneralUtilities;
    this->m_uiPtr->terminal->setTextColor(QColor(GREEN_COLOR_STRING));
    QString stringToAppend{toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + toQString(TERMINAL_DELAY_BASE_STRING) + toQString(howLong)};
    if (delayType == DelayType::SECONDS) {
        stringToAppend.append(SECONDS_SUFFIX_STRING);
    } else if (delayType == DelayType::MILLISECONDS) {
        stringToAppend.append(MILLISECONDS_SUFFIX_STRING);
    } else if (delayType == DelayType::MICROSECONDS) {
        stringToAppend.append(MICROSECONDS_SUFFIX_STRING);
    }
    this->m_uiPtr->terminal->append(stringToAppend);
}

void CanTerminal::printFlushResult(FlushType flushType)
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
    this->m_uiPtr->terminal->append(toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + stringToAppend);
    this->m_uiPtr->terminal->setTextColor(QColor());
}



void CanTerminal::printLoopResult(LoopType loopType, int currentLoop, int loopCount)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    this->m_uiPtr->terminal->setTextColor(QColor(ORANGE_COLOR_STRING));
    if (loopCount == -1) {
        if (loopType == LoopType::START) {
            if (currentLoop == 0) {
                this->m_uiPtr->terminal->append(toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + BEGINNING_INFINITE_LOOP_STRING);
            }
            this->m_uiPtr->terminal->append(toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + toQString(BEGIN_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString(INFINITE_LOOP_COUNT_TAIL_STRING));
        } else if (loopType == LoopType::END) {
            this->m_uiPtr->terminal->append(toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + toQString(END_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString(INFINITE_LOOP_COUNT_TAIL_STRING));
        }
    } else {
        if (loopType == LoopType::START) {
            if (currentLoop == 0) {
                this->m_uiPtr->terminal->append(toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + toQString(BEGINNING_LOOPS_BASE_STRING) + toQString(loopCount) + toQString(LOOPS_TAIL_STRING));
            }
            this->m_uiPtr->terminal->append(toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + toQString(BEGIN_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString("/") + toQString(loopCount) + toQString(")"));
        } else if (loopType == LoopType::END) {
            this->m_uiPtr->terminal->append(toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + toQString(END_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString("/") + toQString(loopCount) + toQString(")"));
            if ((currentLoop+1) == loopCount) {
                this->m_uiPtr->terminal->append(toQString(tWhitespace(CanTerminal::s_CAN_SCRIPT_INDENT)) + toQString(ENDING_LOOPS_BASE_STRING) + toQString(loopCount) + toQString(LOOPS_TAIL_STRING));
            }
        }
        this->m_uiPtr->terminal->setTextColor(QColor());
    }
}


void CanTerminal::staticPrintRxResult(CanTerminal *canTerminal, const std::string &str)
{
    if (canTerminal) {
        canTerminal->printRxResult(str);
    }
}

void CanTerminal::staticPrintTxResult(CanTerminal *canTerminal, const std::string &str)
{
    if (canTerminal) {
        canTerminal->printTxResult(str);
    }
}

void CanTerminal::staticPrintDelayResult(CanTerminal *canTerminal,  DelayType delayType, int howLong)
{
    if (canTerminal) {
        canTerminal->printDelayResult(delayType, howLong);
    }
}

void CanTerminal::staticPrintFlushResult(CanTerminal *canTerminal, FlushType flushType)
{
    if (canTerminal) {
        canTerminal->printFlushResult(flushType);
    }
}

void CanTerminal::staticPrintLoopResult(CanTerminal *canTerminal, LoopType loopType, int currentLoop, int loopCount)
{
    if (canTerminal) {
        canTerminal->printLoopResult(loopType, currentLoop, loopCount);
    }
}


void CanTerminal::onActionLoadScriptTriggered(bool checked)
{
    (void)checked;
    pauseUpdates();
    using namespace ArduinoPCStrings;
    QFile homeCheck{OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY};
    QString defaultScriptFileDirectory{""};
    if (homeCheck.exists()) {
        defaultScriptFileDirectory = OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY;
    }
    QFile file{QFileDialog::getOpenFileName(this, OPEN_SCRIPT_FILE_CAPTION, defaultScriptFileDirectory)};
    if (file.fileName() == "") {
        return;
    }
    if (file.exists()) {
        std::unique_ptr<CanScriptExecutor> scriptExecutor{std::make_unique<CanScriptExecutor>(file.fileName().toStdString())};
        if (scriptExecutor->scriptReader()->commands()->empty()) {
            std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
            warningBox->setText(EMPTY_SCRIPT_STRING + file.fileName());
            warningBox->setWindowTitle(EMPTY_SCRIPT_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
            warningBox->exec();
        } else {
            this->m_uiPtr->terminal->append("");
            this->m_uiPtr->terminal->append(EXECUTING_SCRIPT_STRING + file.fileName());
            this->m_uiPtr->sendButton->setText(CANCEL_SCRIPT_STRING);
            this->m_uiPtr->statusBar->showMessage(EXECUTING_SCRIPT_STRING + file.fileName());
            disableCanLineEdits();
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
                this->m_uiPtr->statusBar->showMessage(CANCELED_EXECUTING_SCRIPT_STRING + file.fileName());
                this->m_cancelScript = false;
            } else {
                this->m_uiPtr->terminal->append(FINISHED_EXECUTING_SCRIPT_STRING + file.fileName());
                this->m_uiPtr->statusBar->showMessage(FINISHED_EXECUTING_SCRIPT_STRING + file.fileName());
            }
            enableCanLineEdits();
            this->m_uiPtr->canMessageIDLineEdit->setFocus();
            this->m_uiPtr->sendButton->setText(SEND_STRING);
        }
    } else {
        std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
        warningBox->setText(FILE_DOES_NOT_EXIST_STRING + file.fileName());
        warningBox->setWindowTitle(FILE_DOES_NOT_EXIST_WINDOW_TITLE_STRING);
        warningBox->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
        warningBox->exec();
    }
    resumeUpdates();
}

void CanTerminal::disableCanLineEdits()
{
    for (auto &it : this->m_canTerminalLineEdits) {
        it->setEnabled(false);
    }
}

void CanTerminal::enableCanLineEdits()
{
    for (auto &it : this->m_canTerminalLineEdits) {
        it->setEnabled(true);
    }
}


bool CanTerminal::cancelScript() const
{
    return this->m_cancelScript;
}

std::string CanTerminal::validateCanByte(const std::string &byte)
{
    //TODO:
    if (byte == "") {
        return "0";
    }
    return byte;
}

void CanTerminal::bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget)
{
    this->m_qDesktopWidget.reset();
    this->m_qDesktopWidget = qDesktopWidget;
}

void CanTerminal::bindArduino(std::shared_ptr<Arduino> arduino)
{
    this->m_arduino.reset();
    this->m_arduino = arduino;
}

void CanTerminal::bindGlobalLogger(std::shared_ptr<GlobalLogger> globalLogger)
{
    this->m_globalLogger.reset();
    this->m_globalLogger = globalLogger;
}

int CanTerminal::screenIndex() const
{
    return this->m_screenIndex;
}

void CanTerminal::setScreenIndex(int screenIndex)
{
    this->m_screenIndex = screenIndex;
}

void CanTerminal::closeEvent(QCloseEvent *ce)
{
    this->m_uiPtr->terminal->clear();
    zeroOutCanBytes();
    emit (aboutToClose(this->m_screenIndex));
    QWidget::closeEvent(ce);
}

std::shared_ptr<Ui::CanTerminal> CanTerminal::uiPtr() const
{
    return this->m_uiPtr;
}

void CanTerminal::centerAndFitWindow()
{
    calculateXYPlacement();
    this->move(this->m_xPlacement, this->m_yPlacement);
}

int CanTerminal::xPlacement() const
{
    return this->m_xPlacement;
}

int CanTerminal::yPlacement() const
{
    return this->m_yPlacement;
}

void CanTerminal::calculateXYPlacement()
{
    std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qDesktopWidget->availableGeometry())};
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
    this->m_yPlacement = (avail->height()/2)-(this->height()/2) - MainWindow::TASKBAR_HEIGHT();
}

void CanTerminal::onReturnKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)screenIndex;
    (void)byteOrderIndex;
    onReturnKeyPressed();
}

void CanTerminal::onUpArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)ctle;
    (void)byteOrderIndex;
    onUpArrowPressed();
}

void CanTerminal::onDownArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)ctle;
    (void)byteOrderIndex;
    onDownArrowPressed();
}

void CanTerminal::onLeftArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)ctle;
    (void)byteOrderIndex;
    if (ctle) {
        if (ctle->cursorPosition() == 0) {
            unsigned int nextIndex{0};
            if (byteOrderIndex == 0) {
                nextIndex = this->m_canTerminalLineEdits.size()-1;
            } else {
                nextIndex = byteOrderIndex - 1;
            }
            this->m_canTerminalLineEdits.at(nextIndex)->setFocus();
            this->m_canTerminalLineEdits.at(nextIndex)->setCursorPosition(this->m_canTerminalLineEdits.at(nextIndex)->text().length());
        }
    }
}

void CanTerminal::onRightArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)ctle;
    (void)byteOrderIndex;
    if (ctle) {
        if (ctle->cursorPosition() == ctle->text().size()) {
            unsigned int nextIndex{0};
            if (byteOrderIndex == this->m_canTerminalLineEdits.size()-1) {
                nextIndex = 0;
            } else {
                nextIndex = byteOrderIndex + 1;
            }
            this->m_canTerminalLineEdits.at(nextIndex)->setFocus();
            this->m_canTerminalLineEdits.at(nextIndex)->setCursorPosition(0);
        }
    }
}

void CanTerminal::onEscapeKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)screenIndex;
    (void)byteOrderIndex;
    ctle->clearFocus();
    onEscapeKeyPressed();
}

void CanTerminal::onAltKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)ctle;
    (void)screenIndex;
    (void)byteOrderIndex;
    onAltKeyPressed();
}

void CanTerminal::onCtrlAPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)ctle;
    (void)byteOrderIndex;
    onCtrlAPressed();
}

void CanTerminal::onCtrlEPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)ctle;
    (void)byteOrderIndex;
    onCtrlEPressed();
}

void CanTerminal::onCtrlUPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)ctle;
    (void)byteOrderIndex;
    onCtrlUPressed();
}

void CanTerminal::onCtrlGPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex)
{
    (void)screenIndex;
    (void)ctle;
    (void)byteOrderIndex;
    onCtrlGPressed();
}

void CanTerminal::onReturnKeyPressed()
{
    this->m_uiPtr->sendButton->click();
    this->m_uiPtr->canMessageIDLineEdit->setFocus();
}

void CanTerminal::addToCommandHistory()
{
    std::vector<QString> temp;
    for (auto &it : this->m_canTerminalLineEdits) {
        temp.emplace_back(it->text());
    }
    this->m_commandHistory.emplace_back(temp);
}

void CanTerminal::onUpArrowPressed()
{
    if (this->m_currentHistoryIndex >= this->m_commandHistory.size()-1) {
        return;
    }
    if (this->m_currentHistoryIndex == 0) {
        if (!this->m_currentLinePushedIntoCommandHistory) {
            std::vector<QString> stringToAdd{};
            for (auto &it : this->m_canTerminalLineEdits) {
                stringToAdd.push_back(it->text());
            }
            this->m_commandHistory.insert(this->m_commandHistory.begin(), stringToAdd);
            this->m_currentLinePushedIntoCommandHistory = true;
        }
    }
    this->m_currentHistoryIndex++;
    int byteOrderIndex{0};
    for (auto &it : this->m_canTerminalLineEdits) {
        it->setText(this->m_commandHistory.at(this->m_currentHistoryIndex).at(byteOrderIndex++));
    }
}

void CanTerminal::onDownArrowPressed()
{
    if (this->m_currentHistoryIndex == 0) {
        return;
    }
    this->m_currentHistoryIndex--;
    int byteOrderIndex{0};
    for (auto &it : this->m_canTerminalLineEdits) {
        it->setText(this->m_commandHistory.at(this->m_currentHistoryIndex).at(byteOrderIndex++));
    }
}

void CanTerminal::onEscapeKeyPressed()
{

}

void CanTerminal::onAltKeyPressed()
{

}

void CanTerminal::onCtrlAPressed()
{
    this->m_uiPtr->canMessageIDLineEdit->setFocus();
    this->m_uiPtr->canMessageIDLineEdit->setCursorPosition(0);
}

void CanTerminal::onCtrlEPressed()
{
    this->m_uiPtr->canByte7LineEdit->setFocus();
    this->m_uiPtr->canByte7LineEdit->setCursorPosition(this->m_uiPtr->canByte7LineEdit->text().size());
}

void CanTerminal::onCtrlUPressed()
{
    onClearButtonClicked();
}

void CanTerminal::onCtrlGPressed()
{
    this->m_uiPtr->terminal->clear();
}

void CanTerminal::keyPressEvent(QKeyEvent *qke)
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
        } else {
            return QWidget::keyPressEvent(qke);
        }
    }
}

QApplication *CanTerminal::application()
{
    return qApp;
}
