#include "programloadingwidget.h"
#include "ui_programloadingwidget.h"

ProgramLoadingWidget::ProgramLoadingWidget(std::shared_ptr<ArduinoPCIcons> apciPtr,
                                           std::shared_ptr<QDesktopWidget> qDesktopWidget,
                                           double step,
                                           std::shared_ptr<GlobalLogger> globalLogger,
                                           QWidget *parent) :
    QWidget{parent},
    m_uiPtr{std::make_shared<Ui::ProgramLoadingWidget>()},
    m_apciPtr{apciPtr},
    m_qDesktopWidget{qDesktopWidget},
    m_globalLogger{globalLogger},
    m_step{step},
    m_quitAsyncUpdates{false},
    m_xPlacement{0},
    m_yPlacement{0}
{
    using namespace ArduinoPCStrings;
    this->m_uiPtr->setupUi(this);
    this->m_uiPtr->progressBar->setValue(0);
    this->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
    this->setWindowTitle(LOADING_PROGRAM_WIDGET_WINDOW_TITLE);
}

void ProgramLoadingWidget::startAsyncUpdates()
{
    std::async(std::launch::async, &ProgramLoadingWidget::updateGlobalMessages, this);
}

void ProgramLoadingWidget::updateGlobalMessages(ProgramLoadingWidget *plw)
{
    do {
        plw->updateMessages();
    } while (!plw->quitAsyncUpdates());
}

void ProgramLoadingWidget::showMessage(const QString &str)
{
    this->m_uiPtr->mainMessageLabel->setText(str);
}

void ProgramLoadingWidget::updateMessages()
{
    if (this->m_globalLogger) {
        if (this->m_globalLogger->logMessageAvailable()) {
            this->m_uiPtr->mainMessageLabel->setText(toQString(this->m_globalLogger->nextMessage()));
        }
    }
}

int ProgramLoadingWidget::progressBarValue() const
{
    return this->m_uiPtr->progressBar->value();
}

void ProgramLoadingWidget::setProgressBarMaximumValue(int maximum)
{
    this->m_uiPtr->progressBar->setMaximum(maximum);
}

void ProgramLoadingWidget::setProgressBarMinimumValue(int minimum)
{
    this->m_uiPtr->progressBar->setMinimum(minimum);
}

int ProgramLoadingWidget::progressBarMinimumValue() const
{
    return this->m_uiPtr->progressBar->minimum();
}

int ProgramLoadingWidget::progressBarMaximumValue() const
{
    return this->m_uiPtr->progressBar->maximum();
}

void ProgramLoadingWidget::bindArduinoPCIcons(std::shared_ptr<ArduinoPCIcons> apciPtr)
{
    this->m_apciPtr.reset();
    this->m_apciPtr = apciPtr;
}

void ProgramLoadingWidget::bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget)
{
    this->m_qDesktopWidget.reset();
    this->m_qDesktopWidget = qDesktopWidget;
}

void ProgramLoadingWidget::bindGlobalLogger(std::shared_ptr<GlobalLogger> globalLogger)
{
    this->m_globalLogger.reset();
    this->m_globalLogger = globalLogger;
}

bool ProgramLoadingWidget::quitAsyncUpdates() const
{
    return this->m_quitAsyncUpdates;
}

void ProgramLoadingWidget::setStep(double step)
{
    this->m_step = step;
}

double ProgramLoadingWidget::step() const
{
    return this->m_step;
}

void ProgramLoadingWidget::updateProgressBar()
{
    this->m_uiPtr->progressBar->setValue(this->m_uiPtr->progressBar->value() + this->m_step);
}

void ProgramLoadingWidget::closeEvent(QCloseEvent *ce)
{
    emit (aboutToClose());
    this->m_quitAsyncUpdates = true;
    QWidget::closeEvent(ce);
}

void ProgramLoadingWidget::centerAndFitWindow()
{
    calculateXYPlacement();
    this->move(this->m_xPlacement, this->m_yPlacement);
}

int ProgramLoadingWidget::xPlacement() const
{
    return this->m_xPlacement;
}

int ProgramLoadingWidget::yPlacement() const
{
    return this->m_yPlacement;
}

void ProgramLoadingWidget::calculateXYPlacement()
{
    std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qDesktopWidget->availableGeometry())};
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
    this->m_yPlacement = (avail->height()/2)-(this->height()/2) - MainWindow::TASKBAR_HEIGHT();
}
