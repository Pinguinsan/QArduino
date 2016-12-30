
#ifndef ARDUINOPC_CANTERMINAL_H
#define ARDUINOPC_CANTERMINAL_H

#include <QDesktopWidget>
#include <QMainWindow>
#include <QShowEvent>
#include <QHideEvent>
#include <QApplication>
#include <QSize>
#include <QRect>
#include <QWidget>
#include <QTimer>
#include <QColor>
#include <memory>
#include <arduino.h>

#include "arduinopcstrings.h"
#include "arduinopcicons.h"
#include "globallogger.h"

#include "canterminal/canterminallineedit.h"
#include "canterminal/smartcanreportfuture.h"
#include "canterminal/cancommandhistoryaction.h"
#include "canterminal/canscriptexecutor.h"
#include "canterminal/canscriptreader.h"

namespace Ui
{
    class CanTerminal;
}

class CanTerminalScheduler;
class SmartCanReportFuture;
class GlobalLogger;

class CanTerminal : public QMainWindow
{
    Q_OBJECT
public:
    explicit CanTerminal(int screenIndex,
                         std::shared_ptr<ArduinoPCIcons> apciPtr,
                         std::shared_ptr<QDesktopWidget> qDesktopWidget,
                         std::shared_ptr<GlobalLogger> globalLogger,
                         std::shared_ptr<Arduino> arduino,
                         QWidget *parent = 0);

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }
    QString toQString(const QString &qstr) { return qstr; }

    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    void keyPressEvent(QKeyEvent *qke);
    void closeEvent(QCloseEvent *ce);

    int screenIndex() const;
    void setScreenIndex(int screenIndex);
    std::shared_ptr<Ui::CanTerminal> uiPtr() const;
    void bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget);
    void bindArduinoPCIcons(std::shared_ptr<ArduinoPCIcons> apciPtr);
    void bindArduino(std::shared_ptr<Arduino> arduino);
    void bindGlobalLogger(std::shared_ptr<GlobalLogger> globalLogger);
    void zeroOutCanBytes();
    std::shared_ptr<Arduino> arduino() const;

    void pauseUpdates();
    void resumeUpdates();
    QApplication *application();

    void appendString(const std::string &str);
    void appendCanReport(const CanReport &canReport);
    static CanReport canReport(CanTerminal *canTerminal);

    void centerAndFitWindow();
    int xPlacement() const;
    int yPlacement() const;
    bool cancelScript() const;

    void appendCanMessage(const CanMessage &message);
signals:
    void aboutToClose(int);


private slots:
    void updateCanTerminal();
    void onActionLoadScriptTriggered(bool checked);
    void onCommandHistoryContextMenuRequested(const QPoint &point);
    void onCommandHistoryContextMenuActionTriggered(CanCommandHistoryAction *action, bool checked);
    void onClearButtonClicked();
    void onSendButtonClicked();
    void onCanTerminalLineEditEditingFinished(CanTerminalLineEdit *cle, unsigned int byteOrderIndex, int screenIndex);
    void onReturnKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onUpArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onDownArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onLeftArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onRightArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onEscapeKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onAltKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onCtrlAPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onCtrlEPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onCtrlUPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onCtrlGPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskReturnKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskUpArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskDownArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskLeftArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskRightArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskEscapeKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskAltKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskCtrlAPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskCtrlEPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskCtrlUPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onPositiveCanMaskCtrlGPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);

    void onNegativeCanMaskReturnKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onNegativeCanMaskUpArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onNegativeCanMaskDownArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onNegativeCanMaskLeftArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onNegativeCanMaskRightArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onNegativeCanMaskEscapeKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onNegativeCanMaskAltKeyPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onNegativeCanMaskCtrlAPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onNegativeCanMaskCtrlEPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onNegativeCanMaskCtrlUPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onNegativeCanMaskCtrlGPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void onReturnKeyPressed();
    void onUpArrowPressed();
    void onDownArrowPressed();
    void onEscapeKeyPressed();
    void onAltKeyPressed();
    void onCtrlAPressed();
    void onCtrlEPressed();
    void onCtrlUPressed();
    void onCtrlGPressed();
    void onPositiveCanMaskSendButtonClicked(bool checked);
    void onPositiveCanMaskRemoveButtonClicked(bool checked);
    void onPositiveCanMaskRemoveAllButtonClicked(bool checked);
    void onNegativeCanMaskSendButtonClicked(bool checked);
    void onNegativeCanMaskRemoveButtonClicked(bool checked);
    void onNegativeCanMaskRemoveAllButtonClicked(bool checked);
    void onPositiveCanMaskCommandHistoryContextMenuRequested(const QPoint &point);
    void onNegativeCanMaskCommandHistoryContextMenuRequested(const QPoint &point);
    void onPositiveCanMaskCommandHistoryContextMenuActionTriggered(CanCommandHistoryAction *action, bool checked);
    void onNegativeCanMaskCommandHistoryContextMenuActionTriggered(CanCommandHistoryAction *action, bool checked);
private:
    int m_screenIndex;
    std::shared_ptr<Ui::CanTerminal> m_uiPtr;
    std::shared_ptr<ArduinoPCIcons> m_apciPtr;
    std::shared_ptr<QDesktopWidget> m_qDesktopWidget;
    std::shared_ptr<GlobalLogger> m_globalLogger;
    std::shared_ptr<Arduino> m_arduino;
    std::function<CanReport(CanTerminal *canTerminal)> m_packagedCanReportTask;
    std::unique_ptr<SmartCanReportFuture> m_canReportFuture;
    std::unique_ptr<QTimer> m_canUpdateTimer;
    std::function<void(CanTerminal*, const std::string &)> m_packagedRxResultTask;
    std::function<void(CanTerminal*, const std::string &)> m_packagedTxResultTask;
    std::function<void(CanTerminal*, DelayType, int)> m_packagedDelayResultTask;
    std::function<void(CanTerminal*, FlushType)> m_packagedFlushResultTask;
    std::function<void(CanTerminal*, LoopType, int, int)> m_packagedLoopResultTask;
    bool m_currentLinePushedIntoCommandHistory;
    unsigned int m_currentHistoryIndex;
    bool m_currentLinePushedIntoPositiveCanMaskCommandHistory;
    unsigned int m_currentPositiveLineEditHistoryIndex;
    bool m_currentLinePushedIntoNegativeCanMaskCommandHistory;
    unsigned int m_currentNegativeLineEditHistoryIndex;
    std::vector<std::vector<QString>> m_commandHistory;
    std::vector<QString> m_positiveCanMaskCommandHistory;
    std::vector<QString> m_negativeCanMaskCommandHistory;
    std::vector<CanTerminalLineEdit*> m_canTerminalLineEdits;
    bool m_cancelScript;
    int m_xPlacement;
    int m_yPlacement;

    void clearEmptyStringsFromPositiveCanMaskCommandHistory();
    void clearEmptyStringsFromNegativeCanMaskCommandHistory();
    void resetPositiveCanMaskCommandHistory();
    void resetNegativeCanMaskCommandHistory();
    void disableCanLineEdits();
    void enableCanLineEdits();
    void calculateXYPlacement();
    std::string validateCanByte(const std::string &byte);
    void addToCommandHistory();
    void addToPositiveCanMaskCommandHistory();
    void addToNegativeCanMaskCommandHistory();

    static void staticPrintRxResult(CanTerminal *CanTerminal, const std::string &str);
    static void staticPrintTxResult(CanTerminal *CanTerminal, const std::string &str);
    static void staticPrintDelayResult(CanTerminal *CanTerminal, DelayType delayType, int howLong);
    static void staticPrintFlushResult(CanTerminal *CanTerminal, FlushType flushType);
    static void staticPrintLoopResult(CanTerminal *CanTerminal, LoopType loopType, int currentLoop, int loopCount);

    void printRxResult(const std::string &str);
    void printTxResult(const std::string &str);
    void printDelayResult(DelayType delayType, int howLong);
    void printFlushResult(FlushType flushType);
    void printLoopResult(LoopType loopType, int currentLoop, int loopCount);

    static const int s_CAN_TIMEOUT;
    static const int s_CAN_SCRIPT_INDENT;
    static const int s_CAN_TERMINAL_UPDATE_TIME;
    static const int s_CAN_STATUS_BAR_TIMEOUT;

};

#endif //ARDUINOPC_CANTERMINAL_H
