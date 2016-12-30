#ifndef ARDUINOPC_PROGRAMLOADINGWIDGET_H
#define ARDUINOPC_PROGRAMLOADINGWIDGET_H

#include <QWidget>
#include <QDesktopWidget>
#include <QCloseEvent>
#include <QRect>
#include <QSize>
#include <QTimer>
#include <memory>
#include <future>

#include "mainwindow.h"
#include "globallogger.h"
#include "arduinopcicons.h"
#include "arduinopcstrings.h"

namespace Ui
{
    class ProgramLoadingWidget;
}

class ProgramLoadingWidget : public QWidget
{
    Q_OBJECT
public:

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }
    QString toQString(const QString &qstr) { return qstr; }

    explicit ProgramLoadingWidget(std::shared_ptr<ArduinoPCIcons> apciPtr,
                                  std::shared_ptr<QDesktopWidget> qDesktopWidget,
                                  double step,
                                  std::shared_ptr<GlobalLogger> globalLogger = nullptr,
                                  QWidget *parent = 0);

    void closeEvent(QCloseEvent *event);
    int xPlacement() const;
    int yPlacement() const;
    int progressBarValue() const;
    void centerAndFitWindow();
    void setStep(double step);
    double step() const;
    void showMessage(const QString &str);

    int progressBarMaximumValue() const;
    int progressBarMinimumValue() const;
    void setProgressBarMinimumValue(int minimum);
    void setProgressBarMaximumValue(int maximum);
    void updateProgressBar();
    void updateMessages();

    static void updateGlobalMessages(ProgramLoadingWidget *plw);
    void startAsyncUpdates();
    void bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget);
    void bindArduinoPCIcons(std::shared_ptr<ArduinoPCIcons> apciPtr);
    void bindGlobalLogger(std::shared_ptr<GlobalLogger> globalLogger);
    bool quitAsyncUpdates() const;
signals:
    void aboutToClose();

private:
    std::shared_ptr<Ui::ProgramLoadingWidget> m_uiPtr;
    std::shared_ptr<ArduinoPCIcons> m_apciPtr;
    std::shared_ptr<QDesktopWidget> m_qDesktopWidget;
    std::shared_ptr<GlobalLogger> m_globalLogger;
    double m_step;
    bool m_quitAsyncUpdates;
    int m_xPlacement;
    int m_yPlacement;

    void calculateXYPlacement();
};

#endif //ARDUINOPC_PROGRAMLOADINGWIDGET_H
