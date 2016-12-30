#ifndef ARDUINOPC_MANUALSCREEN_H
#define ARDUINOPC_MANUALSCREEN_H

#include <QIntValidator>
#include <QDoubleValidator>
#include <QDesktopWidget>
#include <QShowEvent>
#include <QHideEvent>
#include <QMenu>
#include <QFrame>
#include <QSize>
#include <QRect>
#include <QWidget>
#include <QTimer>
#include <memory>
#include <functional>

#include <arduino.h>
#include <eventtimer.h>

#include "manualscreen/aliaschangedialog.h"
#include "manualscreen/analogiorawstatepushbutton.h"
#include "manualscreen/analogoutputentrybox.h"
#include "manualscreen/diostatepushbutton.h"
#include "manualscreen/iostatusview.h"
#include "manualscreen/pullupstatepushbutton.h"
#include "manualscreen/smartioreportfuture.h"
#include "manualscreen/diostatelabel.h"

#include "mainwindow.h"
#include "customcombobox.h"
#include "arduinopcstrings.h"
#include "arduinopcicons.h"
#include "globallogger.h"

namespace Ui
{
    class ManualScreen;
}

class SmartIOReportFuture;
class GlobalLogger;
class AliasChangeDialog;

class ManualScreen : public QMainWindow
{
    Q_OBJECT
public:
    ManualScreen(int screenIndex,
                 std::shared_ptr<ArduinoPCIcons> apciPtr,
                 std::shared_ptr<QDesktopWidget> qDesktopWidget,
                 std::shared_ptr<GlobalLogger> globalLogger,
                 std::shared_ptr<Arduino> arduino,
                 QWidget *parent = 0);

    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *hideEvent);
    void showEvent(QShowEvent *showEvent);

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(GeneralUtilities::toString(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }
    QString toQString(const QString &qstr) { return qstr; }

    int screenIndex() const;
    void setScreenIndex(int screenIndex);
    std::shared_ptr<Ui::ManualScreen> msuiPtr() const;
    void bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget);
    void bindArduinoPCIcons(std::shared_ptr<ArduinoPCIcons> apciPtr);
    void bindGlobalLogger(std::shared_ptr<GlobalLogger> globalLogger);
    std::shared_ptr<Arduino> arduino();

    void centerAndFitWindow();
    int xPlacement() const;
    int yPlacement() const;

    void resumeUpdates();
    void pauseUpdates();

    static IOReport ioReport(ManualScreen *manualScreen);

    static const int s_MANUAL_SCREEN_UPDATE_TIME;
    static const int s_DIGITAL_IO_LABEL_MAXIMUM_WIDTH;
    static const int s_ANALOG_IO_LABEL_MINIMUM_WIDTH;
    static const int s_ANALOG_IO_LABEL_MAXIMUM_WIDTH;
    static const int s_SYNCHRONIZE_GPIO_TIMEOUT;

    std::shared_ptr<Ui::ManualScreen> uiPtr() const;

public slots:
    void onChangeIOTypeActionTriggered(IOStatusView *iosv, IOType newIOType);
    void onChangeAliasActionTriggered(IOStatusView *iosv);
    void onChangeAliasDialogClosed(int pinNumber, bool change, const QString &newAlias);
private slots:
    void onCloseActionTriggered(bool);
    void updateManualScreen();
    void onAnalogToDigitalBoxEditingFinished();
    void onAnalogOutputLineEditLostFocus(int);
    void onAnalogOutputLineEditReturnPressed(int pinNumber, AnalogOutputEntryBox *aoeb, int screenIndex);
    void onAnalogRawStatePushButtonClicked(int, AnalogIORawStatePushButton*, int, bool);
    void onDIOStatePushButtonClicked(int pinNumber, DIOStatePushButton *qpb, int screenIndex, bool checked);
    void onPullupStatePushButtonClicked(int pinNumber, PullupStatePushButton *pspb, int screenIndex, bool clicked);
    void onAnalogToDigitalSetButtonClicked(bool);

    void onAnalogToDigitalLabelClicked(bool checked);
    void onAnalogToDigitalLineEditReturnPressed(int pinNumber, AnalogOutputEntryBox *aoeb, int screenIndex);
    void onDIOStateLabelClicked(int pinNumber, bool checked);
    void onIOStatusViewContextMenuRequested(IOStatusView *iosv, const QPoint &pos);
signals:
    void aboutToClose(int);

private:
    int m_screenIndex;
    std::shared_ptr<Ui::ManualScreen> m_uiPtr;
    std::shared_ptr<ArduinoPCIcons> m_apciPtr;
    std::shared_ptr<QDesktopWidget> m_qDesktopWidget;
    std::shared_ptr<GlobalLogger> m_globalLogger;
    std::shared_ptr<Arduino> m_arduino;
    std::function<IOReport(ManualScreen *manualScreen)> m_packagedIOReportTask;
    std::unique_ptr<SmartIOReportFuture> m_ioReportFuture;
    std::unique_ptr<QTimer> m_manualUpdateTimer;
    std::map<int, IOStatusView*> m_ioStatusViews;
    std::shared_ptr<QDoubleValidator> m_inputValidator;
    std::shared_ptr<QIntValidator> m_rawInputValidator;
    std::unique_ptr<EventTimer> m_synchronizeGpioTimer;
    bool m_analogToDigitalRawState;
    AnalogOutputEntryBox *m_analogToDigitalThresholdLineEdit;
    bool m_aliasesSet;
    int m_xPlacement;
    int m_yPlacement;

    void setAliases();
    void calculateXYPlacement();
    int findCorrectInsertionLocation(int pinNumber, IOType ioType);
    void addSingleIOStatusView(int pinNumber, IOType newIOType, int screenIndex);

    static const int s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT;
    void reassignIOType(int pinNumber, IOType oldIOType, IOType newIOType, int screenIndex);
    void synchronizeGPIOTypes(IOReport ioReport);
};

#endif //ARDUINOPC_MANUALSCREEN_H
