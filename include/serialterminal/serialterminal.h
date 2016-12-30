#ifndef ARDUINOPC_SERIALTERMINAL_H
#define ARDUINOPC_SERIALTERMINAL_H

#include <QMainWindow>
#include <QString>
#include <QColor>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QPoint>
#include <QFileDialog>
#include <QHideEvent>
#include <QShowEvent>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <vector>
#include <list>
#include <string>
#include <chrono>
#include <functional>
#include <memory>
#include <serialport.h>
#include <systemcommand.h>
#include <generalutilities.h>
#include <arduino.h>

#include "serialterminal/serialcommandhistoryaction.h"
#include "serialterminal/serialcommand.h"
#include "serialterminal/serialscriptexecutor.h"
#include "serialterminal/serialscriptreader.h"
#include "serialterminal/serialterminallineedit.h"
#include "serialterminal/smartserialreportfuture.h"

#include "arduinopcstrings.h"
#include "arduinopcicons.h"
#include "mainwindow.h"
#include "globallogger.h"

namespace Ui
{
    class SerialTerminal;
}

class SmartSerialReportFuture;
class GlobalLogger;

class SerialTerminal : public QMainWindow
{
    Q_OBJECT

public:
    explicit SerialTerminal(int screenIndex,
                            std::shared_ptr<ArduinoPCIcons> apciPtr,
                            std::shared_ptr<QDesktopWidget> qDesktopWidget,
                            std::shared_ptr<GlobalLogger> globalLogger,
                            std::shared_ptr<Arduino> arduino,
                            QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *qke);
    void hideEvent(QHideEvent *hideEvent);
    void showEvent(QShowEvent *showEvent);

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }
    QString toQString(const QString &qstr) { return qstr; }

    std::shared_ptr<ArduinoPCIcons> strPtr() const;
    void bindArduinoPCIcons(std::shared_ptr<ArduinoPCIcons> apciPtr);
    void bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget);
    void bindGlobalLogger(std::shared_ptr<GlobalLogger> globalLogger);
    void centerAndFitWindow();
    int xPlacement() const;
    int yPlacement() const;
    std::shared_ptr<Arduino> arduino();
    void pauseUpdates();
    void resumeUpdates();
    QApplication *application();
    bool cancelScript() const;

    void setScreenIndex(int screenIndex);
    int screenIndex() const;
    void appendSerialReport(const SerialReport &report);
    static SerialReport serialReport(SerialTerminal *serialTerminal);

signals:
    void aboutToClose(int screenIndex);

private slots:
    void updateSerialTerminal();
    void onActionLoadScriptTriggered(bool checked);
    void onCommandHistoryContextMenuRequested(const QPoint &point);
    void onCommandHistoryContextMenuActionTriggered(SerialCommandHistoryAction *action, bool checked);

    void onSendButtonClicked();
    void onReturnKeyPressed(SerialTerminalLineEdit *stle);
    void onUpArrowPressed(SerialTerminalLineEdit *stle);
    void onDownArrowPressed(SerialTerminalLineEdit *stle);
    void onEscapeKeyPressed(SerialTerminalLineEdit *stle);
    void onAltKeyPressed(SerialTerminalLineEdit *stle);
    void onCtrlAPressed(SerialTerminalLineEdit *stle);
    void onCtrlEPressed(SerialTerminalLineEdit *stle);
    void onCtrlUPressed(SerialTerminalLineEdit *stle);
    void onCtrlGPressed(SerialTerminalLineEdit *stle);
    void onCtrlCPressed(SerialTerminalLineEdit *stle);
    void onReturnKeyPressed();
    void onUpArrowPressed();
    void onDownArrowPressed();
    void onEscapeKeyPressed();
    void onAltKeyPressed();
    void onCtrlAPressed();
    void onCtrlEPressed();
    void onCtrlUPressed();
    void onCtrlGPressed();
    void onCtrlCPressed();
private:
    std::shared_ptr<Ui::SerialTerminal> m_uiPtr;
    std::shared_ptr<ArduinoPCIcons> m_apciPtr;
    std::shared_ptr<QDesktopWidget> m_qDesktopWidget;
    std::shared_ptr<GlobalLogger> m_globalLogger;
    std::shared_ptr<Arduino> m_arduino;
    std::function<SerialReport(SerialTerminal *serialTerminal)> m_packagedSerialReportTask;
    std::unique_ptr<SmartSerialReportFuture> m_serialReportFuture;
    std::unique_ptr<QTimer> m_serialUpdateTimer;
    std::function<void(SerialTerminal*, const std::string &)> m_packagedRxResultTask;
    std::function<void(SerialTerminal*, const std::string &)> m_packagedTxResultTask;
    std::function<void(SerialTerminal*, DelayType, int)> m_packagedDelayResultTask;
    std::function<void(SerialTerminal*, FlushType)> m_packagedFlushResultTask;
    std::function<void(SerialTerminal*, LoopType, int, int)> m_packagedLoopResultTask;
    bool m_currentLinePushedIntoCommandHistory;
    std::vector<QString> m_commandHistory;
    unsigned int m_currentHistoryIndex;
    bool m_cancelScript;
    int m_screenIndex;
    int m_xPlacement;
    int m_yPlacement;

    void resetCommandHistory();
    void clearEmptyStringsFromCommandHistory();
    void calculateXYPlacement();
    void appendReceivedString(const std::string &str);
    void appendTransmittedString(const QString &str);

    static void staticPrintRxResult(SerialTerminal *SerialTerminal, const std::string &str);
    static void staticPrintTxResult(SerialTerminal *SerialTerminal, const std::string &str);
    static void staticPrintDelayResult(SerialTerminal *SerialTerminal, DelayType delayType, int howLong);
    static void staticPrintFlushResult(SerialTerminal *SerialTerminal, FlushType flushType);
    static void staticPrintLoopResult(SerialTerminal *SerialTerminal, LoopType loopType, int currentLoop, int loopCount);

    void printRxResult(const std::string &str);
    void printTxResult(const std::string &str);
    void printDelayResult(DelayType delayType, int howLong);
    void printFlushResult(FlushType flushType);
    void printLoopResult(LoopType loopType, int currentLoop, int loopCount);

    static const int s_SERIAL_TIMEOUT;
    static const int s_SERIAL_SCRIPT_INDENT;
    static const int s_SERIAL_TERMINAL_UPDATE_TIME;
    static const int s_SERIAL_STATUS_BAR_TIMEOUT;

};

#endif //ARDUINOPC_SERIALTERMINAL_H
