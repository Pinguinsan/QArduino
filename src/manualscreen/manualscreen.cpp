#include "manualscreen/manualscreen.h"
#include "ui_manualscreen.h"

const int ManualScreen::s_MANUAL_SCREEN_UPDATE_TIME{100};
const int ManualScreen::s_DIGITAL_IO_LABEL_MAXIMUM_WIDTH{30};
const int ManualScreen::s_ANALOG_IO_LABEL_MINIMUM_WIDTH{40};
const int ManualScreen::s_ANALOG_IO_LABEL_MAXIMUM_WIDTH{50};
const int ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT{4000};
const int ManualScreen::s_SYNCHRONIZE_GPIO_TIMEOUT{1000};

ManualScreen::ManualScreen(int screenIndex,
                           std::shared_ptr<ArduinoPCIcons> apciPtr,
                           std::shared_ptr<QDesktopWidget> qDesktopWidget,
                           std::shared_ptr<GlobalLogger> globalLogger,
                           std::shared_ptr<Arduino> arduino,
                           QWidget *parent) :
    QMainWindow{parent},
    m_screenIndex{screenIndex},
    m_uiPtr{std::make_shared<Ui::ManualScreen>()},
    m_apciPtr{apciPtr},
    m_qDesktopWidget{qDesktopWidget},
    m_globalLogger{globalLogger},
    m_arduino{arduino},
    m_packagedIOReportTask{ManualScreen::ioReport},
    m_ioReportFuture{std::make_unique<SmartIOReportFuture>(this, this->m_packagedIOReportTask)},
    m_manualUpdateTimer{std::make_unique<QTimer>()},
    m_inputValidator{std::make_shared<QDoubleValidator>()},
    m_rawInputValidator{std::make_shared<QIntValidator>()},
    m_synchronizeGpioTimer{std::make_unique<EventTimer>()},
    m_analogToDigitalRawState{true},
    m_analogToDigitalThresholdLineEdit{new AnalogOutputEntryBox(0, screenIndex, nullptr)},
    m_aliasesSet{false},
    m_xPlacement{0},
    m_yPlacement{0}
{
    using namespace ArduinoPCStrings;

    this->m_uiPtr->setupUi(this);

    this->m_analogToDigitalThresholdLineEdit->setValidator(this->m_rawInputValidator.get());
    this->m_rawInputValidator->setBottom(0);
    this->m_inputValidator->setBottom(0.00);
    this->m_rawInputValidator->setTop(Arduino::ANALOG_MAX);
    this->m_inputValidator->setTop(Arduino::VOLTAGE_MAX);
    this->m_inputValidator->setNotation(QDoubleValidator::StandardNotation);
    this->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);

    this->m_uiPtr->analogToDigitalLabel->setToolTip(ANALOG_TO_DIGITAL_PB_RAW_TOOLTIP);
    this->m_uiPtr->analogToDigitalSetButton->setToolTip(ANALOG_TO_DIGITAL_PB_RAW_TOOLTIP);
    this->m_uiPtr->analogToDigitalLabel->setStyleSheet(ANALOG_TO_DIGITAL_PB_RAW_STYLESHEET);

    this->m_analogToDigitalThresholdLineEdit->setMinimumWidth(ManualScreen::s_ANALOG_IO_LABEL_MINIMUM_WIDTH);
    this->m_analogToDigitalThresholdLineEdit->setMaximumWidth(ManualScreen::s_ANALOG_IO_LABEL_MAXIMUM_WIDTH);
    this->m_analogToDigitalThresholdLineEdit->setText(ANALOG_IO_INITIAL_STATE_LABEL);

    this->m_uiPtr->analogToDigitalLayout->insertWidget(1, this->m_analogToDigitalThresholdLineEdit);
    connect(this->m_uiPtr->actionClose, SIGNAL(triggered(bool)), this, SLOT(onCloseActionTriggered(bool)));
    connect(this->m_uiPtr->analogToDigitalSetButton, SIGNAL(clicked(bool)), this, SLOT(onAnalogToDigitalSetButtonClicked(bool)));
    connect(this->m_uiPtr->analogToDigitalLabel, SIGNAL(clicked(bool)), this, SLOT(onAnalogToDigitalLabelClicked(bool)));
    connect(this->m_analogToDigitalThresholdLineEdit, SIGNAL(returnPressed(int , AnalogOutputEntryBox *, int)), this, SLOT(onAnalogToDigitalLineEditReturnPressed(int, AnalogOutputEntryBox *, int)));


    this->m_uiPtr->digitalInputsInnerLayout->addStretch();
    this->m_uiPtr->digitalOutputsInnerLayout->addStretch();
    this->m_uiPtr->analogInputsInnerLayout->addStretch();
    this->m_uiPtr->analogOutputsInnerLayout->addStretch();
    for (auto &it : this->m_arduino->AVAILABLE_PINS()) {
        std::pair<IOStatus, IOType> status{this->m_arduino->currentPinMode(it, screenIndex)};
        IOType ioType{status.second};
        addSingleIOStatusView(it, ioType, screenIndex);
    }

    this->m_uiPtr->firmwareLabel->setText(FIRMWARE_VERSION_BASE_STRING + toQString(this->m_arduino->firmwareVersion()));

    this->m_analogToDigitalThresholdLineEdit->setText(toQString(this->m_arduino->analogToDigitalThreshold()));
    this->m_manualUpdateTimer->setInterval(ManualScreen::s_MANUAL_SCREEN_UPDATE_TIME);
}

void ManualScreen::onChangeAliasActionTriggered(IOStatusView *iosv)
{
    std::unique_ptr<AliasChangeDialog> aliasChangeDialog{std::make_unique<AliasChangeDialog>(iosv->pinNumber(),
                                                                                             this->m_qDesktopWidget,
                                                                                             this->m_apciPtr,
                                                                                             this)};
    aliasChangeDialog->centerAndFitWindow();
    aliasChangeDialog->show();
}

void ManualScreen::onChangeIOTypeActionTriggered(IOStatusView *iosv, IOType newIOType)
{
    using namespace ArduinoPCStrings;
    if (!iosv) {
        return;
    }
    IOType oldIOType{iosv->ioType()};
    std::pair<IOStatus, IOType> result{this->m_arduino->pinMode(iosv->gpio(), newIOType)};
    if (result.first == IOStatus::OPERATION_FAILURE) {
        std::cout << FAILED_TO_CHANGE_IO_TYPE_STRING << iosv->pinNumber() << " - " << this->m_arduino->serialPort()->portName() << std::endl;
        this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_CHANGE_IO_TYPE_STRING) + toQString(iosv->pinNumber()) + " - " + toQString(this->m_arduino->serialPort()->portName()));
    } else {
        std::cout << SUCCESSFULLY_CHANGED_IO_TYPE_STRING << iosv->pinNumber() << " (" << this->m_arduino->parseIOType(oldIOType) << " -> " << this->m_arduino->parseIOType(newIOType) << ") - " << this->m_arduino->serialPort()->portName() << std::endl;
        this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_CHANGED_IO_TYPE_STRING) + " (" + toQString(this->m_arduino->parseIOType(oldIOType)) + " -> " + toQString(this->m_arduino->parseIOType(newIOType)) + ") - " + toQString(this->m_arduino->serialPort()->portName()));
        reassignIOType(iosv->pinNumber(), oldIOType, newIOType, this->m_screenIndex);
    }
}

void ManualScreen::reassignIOType(int pinNumber, IOType oldIOType, IOType newIOType, int screenIndex)
{
    using namespace ArduinoPCStrings;
    IOStatusView *temp{this->m_ioStatusViews.find(pinNumber)->second};
    this->m_ioStatusViews.erase(this->m_ioStatusViews.find(pinNumber));
    if ((oldIOType == IOType::DIGITAL_INPUT) || (oldIOType == IOType::DIGITAL_INPUT_PULLUP)) {
        this->m_uiPtr->digitalInputsInnerLayout->removeWidget(temp);
    } else if (oldIOType == IOType::DIGITAL_OUTPUT) {
        this->m_uiPtr->digitalOutputsInnerLayout->removeWidget(temp);
    } else if (oldIOType == IOType::ANALOG_INPUT) {
        this->m_uiPtr->analogInputsInnerLayout->removeWidget(temp);
    } else if (oldIOType == IOType::ANALOG_OUTPUT) {
        this->m_uiPtr->analogOutputsInnerLayout->removeWidget(temp);
    }
    if (temp) {
        temp->deleteLater();
    }
    addSingleIOStatusView(pinNumber, newIOType, screenIndex);

}

void ManualScreen::addSingleIOStatusView(int pinNumber, IOType newIOType, int screenIndex)
{
    using namespace ArduinoPCStrings;
    if ((newIOType == IOType::DIGITAL_INPUT) || (newIOType == IOType::DIGITAL_INPUT_PULLUP)){
        IOStatusView *ioStatusView{new IOStatusView{this->m_arduino->gpioPinByPinNumber(pinNumber), this->m_arduino}};
        ioStatusView->layout()->setMargin(0);
        ioStatusView->layout()->setSpacing(0);
        ioStatusView->setFrameShape(QFrame::WinPanel);
        ioStatusView->setFrameShadow(QFrame::Raised);

        PullupStatePushButton *ioLabel{new PullupStatePushButton(pinNumber, screenIndex)};
        if (this->m_arduino->isValidAnalogInputPin(pinNumber)) {
            ioLabel->setText(PIN_LABEL_BASE + toQString(this->m_arduino->analogPinFromNumber(pinNumber)));
        } else {
            ioLabel->setText(PIN_LABEL_BASE + toQString(pinNumber));
        }
        if (this->m_arduino->gpioPinByPinNumber(pinNumber)->ioType() == IOType::DIGITAL_INPUT_PULLUP) {
            ioLabel->setStyleSheet(DIGITAL_INPUT_PULLUP_UP_STYLESHEET);
        } else {
            ioLabel->setStyleSheet(DIGITAL_INPUT_PULLUP_DOWN_STYLESHEET);
        }
        DIOStatePushButton *onOrOffLabel{new DIOStatePushButton{pinNumber, screenIndex, true}};
        ioLabel->setToolTip(ioLabel->text());
        ioLabel->setFlat(true);
        ioLabel->setAutoFillBackground(true);

        onOrOffLabel->setFlat(true);
        onOrOffLabel->setAutoFillBackground(true);
        onOrOffLabel->setMaximumWidth(ManualScreen::s_DIGITAL_IO_LABEL_MAXIMUM_WIDTH);
        onOrOffLabel->setIcon(this->m_apciPtr->DIO_LOW_STATE_ICON);

        ioStatusView->addWidget(onOrOffLabel);
        ioStatusView->addWidget(ioLabel);

        this->m_uiPtr->digitalInputsInnerLayout->insertWidget(findCorrectInsertionLocation(ioStatusView->pinNumber(), IOType::DIGITAL_INPUT), ioStatusView);
        this->m_ioStatusViews.emplace(pinNumber, ioStatusView);
        connect(ioLabel, SIGNAL(clicked(int, PullupStatePushButton *, int, bool)), this, SLOT(onPullupStatePushButtonClicked(int, PullupStatePushButton *, int, bool)));
        connect(ioStatusView, SIGNAL(ioStatusViewContextMenuRequested(IOStatusView*,QPoint)), this, SLOT(onIOStatusViewContextMenuRequested(IOStatusView*,QPoint)));
        connect(ioStatusView, SIGNAL(changeAliasActionTriggered(IOStatusView*)), this, SLOT(onChangeAliasActionTriggered(IOStatusView*)));
        connect(ioStatusView, SIGNAL(changeIOTypeActionTriggered(IOStatusView *, IOType)), this, SLOT(onChangeIOTypeActionTriggered(IOStatusView *, IOType)));
    } else if (newIOType == IOType::DIGITAL_OUTPUT) {
        DIOStateLabel *ioLabel{new DIOStateLabel{pinNumber}};
        if (this->m_arduino->isValidAnalogInputPin(pinNumber)) {
            ioLabel->setText(PIN_LABEL_BASE + toQString(this->m_arduino->analogPinFromNumber(pinNumber)));
        } else {
            ioLabel->setText(PIN_LABEL_BASE + toQString(pinNumber));
        }
        ioLabel->setFlat(true);
        ioLabel->setAutoFillBackground(true);

        DIOStatePushButton *onOrOffLabel{new DIOStatePushButton{pinNumber, screenIndex}};
        if (this->m_arduino->softDigitalRead(pinNumber, screenIndex).second) {
            onOrOffLabel->setIcon(this->m_apciPtr->DIO_HIGH_STATE_ICON);
        } else {
            onOrOffLabel->setIcon(this->m_apciPtr->DIO_LOW_STATE_ICON);
        }
        ioLabel->setToolTip(ioLabel->text());
        onOrOffLabel->setFlat(true);
        onOrOffLabel->setAutoFillBackground(true);
        onOrOffLabel->setMaximumWidth(ManualScreen::s_DIGITAL_IO_LABEL_MAXIMUM_WIDTH);

        IOStatusView *ioStatusView{new IOStatusView{this->m_arduino->gpioPinByPinNumber(pinNumber), this->m_arduino}};
        ioStatusView->addWidget(onOrOffLabel);
        ioStatusView->addWidget(ioLabel);
        ioStatusView->layout()->setMargin(0);
        ioStatusView->layout()->setSpacing(0);
        ioStatusView->setFrameShape(QFrame::WinPanel);
        ioStatusView->setFrameShadow(QFrame::Raised);

        this->m_uiPtr->digitalOutputsInnerLayout->insertWidget(findCorrectInsertionLocation(ioStatusView->pinNumber(), IOType::DIGITAL_OUTPUT), ioStatusView);
        this->m_ioStatusViews.emplace(pinNumber, ioStatusView);
        connect(ioLabel, SIGNAL(clicked(int,bool)), this, SLOT(onDIOStateLabelClicked(int,bool)));
        connect(onOrOffLabel, SIGNAL(clicked(int, DIOStatePushButton *, int, bool)), this, SLOT(onDIOStatePushButtonClicked(int, DIOStatePushButton *, int, bool)));
        connect(ioStatusView, SIGNAL(ioStatusViewContextMenuRequested(IOStatusView*,QPoint)), this, SLOT(onIOStatusViewContextMenuRequested(IOStatusView*,QPoint)));
        connect(ioStatusView, SIGNAL(changeAliasActionTriggered(IOStatusView*)), this, SLOT(onChangeAliasActionTriggered(IOStatusView*)));
        connect(ioStatusView, SIGNAL(changeIOTypeActionTriggered(IOStatusView *, IOType)), this, SLOT(onChangeIOTypeActionTriggered(IOStatusView *, IOType)));
    } else if (newIOType == IOType::ANALOG_INPUT) {

        IOStatusView *ioStatusView{new IOStatusView{this->m_arduino->gpioPinByPinNumber(pinNumber), this->m_arduino}};
        ioStatusView->layout()->setMargin(0);
        ioStatusView->layout()->setSpacing(0);
        ioStatusView->setFrameShape(QFrame::WinPanel);
        ioStatusView->setFrameShadow(QFrame::Raised);

        AnalogIORawStatePushButton *ioLabel{new AnalogIORawStatePushButton{pinNumber, screenIndex, ioStatusView}};
        if (this->m_arduino->isValidAnalogInputPin(pinNumber)) {
            ioLabel->setText(PIN_LABEL_BASE + toQString(this->m_arduino->analogPinFromNumber(pinNumber)));
        } else {
            ioLabel->setText(PIN_LABEL_BASE + toQString(pinNumber));
        }
        ioLabel->setFlat(true);
        ioLabel->setAutoFillBackground(true);
        ioLabel->setToolTip(ioLabel->text());
        QLabel *onOrOffLabel{new QLabel{}};
        onOrOffLabel->setMinimumWidth(ManualScreen::s_ANALOG_IO_LABEL_MINIMUM_WIDTH);
        onOrOffLabel->setMaximumWidth(ManualScreen::s_ANALOG_IO_LABEL_MAXIMUM_WIDTH);
        onOrOffLabel->setText(ANALOG_IO_INITIAL_STATE_LABEL);
        onOrOffLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        ioStatusView->addWidget(onOrOffLabel);
        ioStatusView->addWidget(ioLabel);

        this->m_uiPtr->analogInputsInnerLayout->insertWidget(findCorrectInsertionLocation(ioStatusView->pinNumber(), IOType::ANALOG_INPUT), ioStatusView);
        this->m_ioStatusViews.emplace(pinNumber, ioStatusView);

        connect(ioStatusView, SIGNAL(ioStatusViewContextMenuRequested(IOStatusView*,QPoint)), this, SLOT(onIOStatusViewContextMenuRequested(IOStatusView*,QPoint)));
        connect(ioStatusView, SIGNAL(changeAliasActionTriggered(IOStatusView*)), this, SLOT(onChangeAliasActionTriggered(IOStatusView*)));
        connect(ioStatusView, SIGNAL(changeIOTypeActionTriggered(IOStatusView *, IOType)), this, SLOT(onChangeIOTypeActionTriggered(IOStatusView *, IOType)));
    } else if (newIOType == IOType::ANALOG_OUTPUT) {

        IOStatusView *ioStatusView{new IOStatusView{this->m_arduino->gpioPinByPinNumber(pinNumber), this->m_arduino}};
        ioStatusView->setRawState(false);
        ioStatusView->layout()->setMargin(0);
        ioStatusView->layout()->setSpacing(0);
        ioStatusView->setFrameShape(QFrame::WinPanel);
        ioStatusView->setFrameShadow(QFrame::Raised);

        AnalogIORawStatePushButton *ioLabel{new AnalogIORawStatePushButton{pinNumber, screenIndex, ioStatusView}};
        ioLabel->setText(PIN_LABEL_BASE + toQString(pinNumber));
        ioLabel->setFlat(true);
        ioLabel->setAutoFillBackground(true);
        ioLabel->setToolTip(ioLabel->text());

        AnalogOutputEntryBox *onOrOffLabel{new AnalogOutputEntryBox(pinNumber, screenIndex, ioStatusView)};
        onOrOffLabel->setGeometry(onOrOffLabel->x(), onOrOffLabel->y(), onOrOffLabel->height(), onOrOffLabel->height());
        onOrOffLabel->setMinimumWidth(ManualScreen::s_ANALOG_IO_LABEL_MINIMUM_WIDTH);
        onOrOffLabel->setMaximumWidth(ManualScreen::s_ANALOG_IO_LABEL_MAXIMUM_WIDTH);
        onOrOffLabel->setText(ANALOG_IO_INITIAL_STATE_LABEL);

        ioStatusView->addWidget(onOrOffLabel);
        ioStatusView->addWidget(ioLabel);

        this->m_uiPtr->analogOutputsInnerLayout->insertWidget(findCorrectInsertionLocation(ioStatusView->pinNumber(), IOType::ANALOG_OUTPUT), ioStatusView);
        this->m_ioStatusViews.emplace(pinNumber, ioStatusView);
        connect(onOrOffLabel, SIGNAL(editingFinished(int)), this, SLOT(onAnalogOutputLineEditLostFocus(int)));
        connect(onOrOffLabel, SIGNAL(returnPressed(int , AnalogOutputEntryBox *, int)), this, SLOT(onAnalogOutputLineEditReturnPressed(int, AnalogOutputEntryBox *, int)));
        connect(ioLabel, SIGNAL(clicked(int,AnalogIORawStatePushButton*,int,bool)), this, SLOT(onAnalogRawStatePushButtonClicked(int, AnalogIORawStatePushButton*, int, bool)));
        connect(ioStatusView, SIGNAL(ioStatusViewContextMenuRequested(IOStatusView*,QPoint)), this, SLOT(onIOStatusViewContextMenuRequested(IOStatusView*,QPoint)));
        connect(ioStatusView, SIGNAL(changeAliasActionTriggered(IOStatusView*)), this, SLOT(onChangeAliasActionTriggered(IOStatusView*)));
        connect(ioStatusView, SIGNAL(changeIOTypeActionTriggered(IOStatusView *, IOType)), this, SLOT(onChangeIOTypeActionTriggered(IOStatusView *, IOType)));
    }
}

int ManualScreen::findCorrectInsertionLocation(int pinNumber, IOType ioType)
{
    using namespace ArduinoPCStrings;
    if ((ioType == IOType::DIGITAL_INPUT) || (ioType == IOType::DIGITAL_INPUT_PULLUP)) {
        if (this->m_uiPtr->digitalInputsInnerLayout->count() == 1) {
            return 0;
        }
        for (int i = 0; i < this->m_uiPtr->digitalInputsInnerLayout->count(); i++) {
            if (i == 0) {
                QWidget *tempWidget{this->m_uiPtr->digitalInputsInnerLayout->itemAt(i)->widget()};
                if (tempWidget) {
                    IOStatusView *iosv{dynamic_cast<IOStatusView *>(tempWidget)};
                    if (iosv) {
                        if (pinNumber < iosv->pinNumber()) {
                            return 0;
                        }
                    }
                }
            } else if (i == this->m_uiPtr->digitalInputsInnerLayout->count()-1) {
                QWidget *tempWidget{this->m_uiPtr->digitalInputsInnerLayout->itemAt(i-1)->widget()};
                if (tempWidget) {
                    IOStatusView *iosv{dynamic_cast<IOStatusView *>(tempWidget)};
                    if (iosv) {
                        if (pinNumber < iosv->pinNumber()) {
                            return i-1;
                        } else {
                            return i;
                        }
                    }
                }
            } else {
                QWidget *lowerWidget{this->m_uiPtr->digitalInputsInnerLayout->itemAt(i)->widget()};
                QWidget *upperWidget{this->m_uiPtr->digitalInputsInnerLayout->itemAt(i+1)->widget()};
                if (lowerWidget && upperWidget) {
                    IOStatusView *lowerIosv{dynamic_cast<IOStatusView *>(lowerWidget)};
                    IOStatusView *upperIosv{dynamic_cast<IOStatusView *>(upperWidget)};
                    if (lowerIosv && upperIosv) {
                        if ((pinNumber < upperIosv->pinNumber()) && (pinNumber > lowerIosv->pinNumber())) {
                            return i;
                        }
                    }
                }
            }
        }
    } else if (ioType == IOType::DIGITAL_OUTPUT) {
        if (this->m_uiPtr->digitalOutputsInnerLayout->count() == 1) {
            return 0;
        }
        for (int i = 0; i < this->m_uiPtr->digitalOutputsInnerLayout->count(); i++) {
            if (i == 0) {
                QWidget *tempWidget{this->m_uiPtr->digitalOutputsInnerLayout->itemAt(i)->widget()};
                if (tempWidget) {
                    IOStatusView *iosv{dynamic_cast<IOStatusView *>(tempWidget)};
                    if (iosv) {
                        if (pinNumber < iosv->pinNumber()) {
                            return 0;
                        }
                    }
                }
            } else if (i == this->m_uiPtr->digitalOutputsInnerLayout->count()-1) {
                QWidget *tempWidget{this->m_uiPtr->digitalOutputsInnerLayout->itemAt(i-1)->widget()};
                if (tempWidget) {
                    IOStatusView *iosv{dynamic_cast<IOStatusView *>(tempWidget)};
                    if (iosv) {
                        if (pinNumber < iosv->pinNumber()) {
                            return i-1;
                        } else {
                            return i;
                        }
                    }
                }
            } else {
                QWidget *lowerWidget{this->m_uiPtr->digitalOutputsInnerLayout->itemAt(i)->widget()};
                QWidget *upperWidget{this->m_uiPtr->digitalOutputsInnerLayout->itemAt(i+1)->widget()};
                if (lowerWidget && upperWidget) {
                    IOStatusView *lowerIosv{dynamic_cast<IOStatusView *>(lowerWidget)};
                    IOStatusView *upperIosv{dynamic_cast<IOStatusView *>(upperWidget)};
                    if (lowerIosv && upperIosv) {
                        if ((pinNumber < upperIosv->pinNumber()) && (pinNumber > lowerIosv->pinNumber())) {
                            return i+1;
                        }
                    }
                }
            }
        }
    } else if (ioType == IOType::ANALOG_INPUT) {
        if (this->m_uiPtr->analogInputsInnerLayout->count() == 1) {
            return 0;
        }
        for (int i = 0; i < this->m_uiPtr->analogInputsInnerLayout->count(); i++) {
            if (i == 0) {
                QWidget *tempWidget{this->m_uiPtr->analogInputsInnerLayout->itemAt(i)->widget()};
                if (tempWidget) {
                    IOStatusView *iosv{dynamic_cast<IOStatusView *>(tempWidget)};
                    if (iosv) {
                        if (pinNumber < iosv->pinNumber()) {
                            return 0;
                        }
                    }
                }
            } else if (i == this->m_uiPtr->analogInputsInnerLayout->count()-1) {
                QWidget *tempWidget{this->m_uiPtr->analogInputsInnerLayout->itemAt(i-1)->widget()};
                if (tempWidget) {
                    IOStatusView *iosv{dynamic_cast<IOStatusView *>(tempWidget)};
                    if (iosv) {
                        if (pinNumber < iosv->pinNumber()) {
                            return i-1;
                        } else {
                            return i;
                        }
                    }
                }
            } else {
                QWidget *lowerWidget{this->m_uiPtr->analogInputsInnerLayout->itemAt(i)->widget()};
                QWidget *upperWidget{this->m_uiPtr->analogInputsInnerLayout->itemAt(i+1)->widget()};
                if (lowerWidget && upperWidget) {
                    IOStatusView *lowerIosv{dynamic_cast<IOStatusView *>(lowerWidget)};
                    IOStatusView *upperIosv{dynamic_cast<IOStatusView *>(upperWidget)};
                    if (lowerIosv && upperIosv) {
                        if ((pinNumber < upperIosv->pinNumber()) && (pinNumber > lowerIosv->pinNumber())) {
                            return i+1;
                        }
                    }
                }
            }
        }
    } else if (ioType == IOType::ANALOG_OUTPUT) {
        if (this->m_uiPtr->analogOutputsInnerLayout->count() == 1) {
            return 0;
        }
        for (int i = 0; i < this->m_uiPtr->analogOutputsInnerLayout->count(); i++) {
            if (i == 0) {
                QWidget *tempWidget{this->m_uiPtr->analogOutputsInnerLayout->itemAt(i)->widget()};
                if (tempWidget) {
                    IOStatusView *iosv{dynamic_cast<IOStatusView *>(tempWidget)};
                    if (iosv) {
                        if (pinNumber < iosv->pinNumber()) {
                            return 0;
                        }
                    }
                }
            } else if (i == this->m_uiPtr->analogOutputsInnerLayout->count()-1) {
                QWidget *tempWidget{this->m_uiPtr->analogOutputsInnerLayout->itemAt(i-1)->widget()};
                if (tempWidget) {
                    IOStatusView *iosv{dynamic_cast<IOStatusView *>(tempWidget)};
                    if (iosv) {
                        if (pinNumber < iosv->pinNumber()) {
                            return i-1;
                        } else {
                            return i;
                        }
                    }
                }
            } else {
                QWidget *lowerWidget{this->m_uiPtr->analogOutputsInnerLayout->itemAt(i)->widget()};
                QWidget *upperWidget{this->m_uiPtr->analogOutputsInnerLayout->itemAt(i+1)->widget()};
                if (lowerWidget && upperWidget) {
                    IOStatusView *lowerIosv{dynamic_cast<IOStatusView *>(lowerWidget)};
                    IOStatusView *upperIosv{dynamic_cast<IOStatusView *>(upperWidget)};
                    if (lowerIosv && upperIosv) {
                        if ((pinNumber < upperIosv->pinNumber()) && (pinNumber > lowerIosv->pinNumber())) {
                            return i+1;
                        }
                    }
                }
            }
        }
    } else {
        throw std::runtime_error(INVALID_IO_TYPE_PASSED_TO_FIND_CORRECT_STRING + std::to_string(pinNumber) + " - " + this->m_arduino->parseIOType(ioType));
    }

    throw std::runtime_error(NONEXISTANT_PIN_PASSED_TO_FIND_CORRECT_STRING + std::to_string(pinNumber) + " - " + this->m_arduino->parseIOType(ioType));

}

void ManualScreen::onIOStatusViewContextMenuRequested(IOStatusView *iosv, const QPoint &pos)
{
    if (!iosv) {
        return;
    }
    iosv->contextMenu().exec(mapToGlobal(pos));
}

void ManualScreen::onChangeAliasDialogClosed(int pinNumber, bool change, const QString &newAlias)
{
    using namespace ArduinoPCStrings;
    if (!change) {
        return;
    }
    std::string oldAlias{this->m_arduino->gpioAliasByPinNumber(pinNumber)};
    bool result{this->m_arduino->setPinAlias(pinNumber, newAlias.toStdString())};
    if (!result) {
        std::cout << FAILED_TO_CHANGE_ALIAS_STRING << pinNumber << " - " << this->m_arduino->serialPort()->portName() << std::endl;
        this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_CHANGE_ALIAS_STRING + toQString(pinNumber) + " - " + toQString(this->m_arduino->serialPort()->portName())));
    } else {
        std::cout << SUCCESSFULLY_CHANGED_ALIAS_STRING << pinNumber << " (" << oldAlias << " -> " << newAlias.toStdString() << ") - " << this->m_arduino->serialPort()->portName() << std::endl;
        this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_CHANGED_ALIAS_STRING) + toQString(pinNumber) + " (" + toQString(oldAlias) + " -> " + newAlias + ") - " + toQString(this->m_arduino->serialPort()->portName()));
    }

    if (this->m_ioStatusViews.find(pinNumber) != this->m_ioStatusViews.end()) {
        IOStatusView *temp{this->m_ioStatusViews.at(pinNumber)};
        QWidget *tempWidget{temp->pinNumberLabel()};
        if (!temp) {
            return;
        }
        if ((temp->ioType() == IOType::DIGITAL_INPUT) || (temp->ioType() == IOType::DIGITAL_INPUT_PULLUP)) {
            PullupStatePushButton *ioLabel{dynamic_cast<PullupStatePushButton *>(tempWidget)};
            if (ioLabel) {
                ioLabel->setText(newAlias);
            }
        } else if (temp->ioType() == IOType::DIGITAL_OUTPUT) {
            DIOStateLabel *ioLabel{dynamic_cast<DIOStateLabel *>(tempWidget)};
            if (ioLabel) {
                ioLabel->setText(newAlias);
            }
        } else if (temp->ioType() == IOType::ANALOG_INPUT) {
            AnalogIORawStatePushButton *ioLabel{dynamic_cast<AnalogIORawStatePushButton *>(tempWidget)};
            if (ioLabel) {
                ioLabel->setText(newAlias);
            }
        } else if (temp->ioType() == IOType::ANALOG_OUTPUT) {
            AnalogIORawStatePushButton *ioLabel{dynamic_cast<AnalogIORawStatePushButton *>(tempWidget)};
            if (ioLabel) {
                ioLabel->setText(newAlias);
            }
        }

    }
}

void ManualScreen::onAnalogToDigitalLineEditReturnPressed(int pinNumber, AnalogOutputEntryBox *aoeb, int screenIndex)
{
    (void)pinNumber;
    (void)aoeb;
    (void)screenIndex;
    this->m_uiPtr->analogToDigitalSetButton->click();
}

void ManualScreen::onAnalogToDigitalLabelClicked(bool checked)
{
    using namespace ArduinoPCStrings;
    (void)checked;
    this->m_analogToDigitalRawState = !this->m_analogToDigitalRawState;
    if (this->m_analogToDigitalRawState) {
        this->m_uiPtr->analogToDigitalLabel->setToolTip(ANALOG_TO_DIGITAL_PB_RAW_TOOLTIP);
        this->m_uiPtr->analogToDigitalSetButton->setToolTip(ANALOG_TO_DIGITAL_PB_RAW_TOOLTIP);
        this->m_uiPtr->analogToDigitalLabel->setStyleSheet(ANALOG_TO_DIGITAL_PB_RAW_STYLESHEET);
        this->m_analogToDigitalThresholdLineEdit->setValidator(this->m_rawInputValidator.get());
        this->m_analogToDigitalThresholdLineEdit->setText(toQString(Arduino::voltageToAnalog(std::stod(this->m_analogToDigitalThresholdLineEdit->text().toStdString()))));
    } else {
        this->m_uiPtr->analogToDigitalSetButton->setToolTip(ANALOG_TO_DIGITAL_PB_NORMAL_TOOLTIP);
        this->m_uiPtr->analogToDigitalLabel->setToolTip(ANALOG_TO_DIGITAL_PB_NORMAL_TOOLTIP);
        this->m_uiPtr->analogToDigitalLabel->setStyleSheet(ANALOG_TO_DIGITAL_PB_NORMAL_STYLESHEET);
        this->m_analogToDigitalThresholdLineEdit->setValidator(this->m_inputValidator.get());
        this->m_analogToDigitalThresholdLineEdit->setText(toQString(std::to_string(Arduino::analogToVoltage(std::stoi(this->m_analogToDigitalThresholdLineEdit->text().toStdString()))).substr(0,5)));
    }
}

void ManualScreen::onAnalogToDigitalSetButtonClicked(bool checked)
{
    using namespace ArduinoPCStrings;
    (void)checked;
    try{
        if (this->m_analogToDigitalRawState) {
            std::pair<IOStatus, int> result{this->m_arduino->changeAnalogToDigitalThreshold(std::stoi(this->m_analogToDigitalThresholdLineEdit->text().toStdString()), this->m_screenIndex)};
            if (result.first == IOStatus::OPERATION_FAILURE) {
                std::cout << FAILED_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            } else {
                std::cout << SUCCESSFULLY_CHANGED_ANALOG_TO_DIGITAL_THRESHOLD_STRING << result.second << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_CHANGED_ANALOG_TO_DIGITAL_THRESHOLD_STRING) + toQString(result.second) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
                this->m_analogToDigitalThresholdLineEdit->setText(toQString(result.second));
            }
        } else {
            std::pair<IOStatus, int> result{this->m_arduino->changeAnalogToDigitalThreshold(Arduino::voltageToAnalog(std::stod(this->m_analogToDigitalThresholdLineEdit->text().toStdString())), this->m_screenIndex)};
            if (result.first == IOStatus::OPERATION_FAILURE) {
                std::cout << FAILED_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING) + toQString(result.second) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            } else {
                std::cout << SUCCESSFULLY_CHANGED_ANALOG_TO_DIGITAL_THRESHOLD_STRING << result.second << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_CHANGED_ANALOG_TO_DIGITAL_THRESHOLD_STRING) + toQString(std::to_string(Arduino::analogToVoltage(result.second)).substr(0,5)) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
                this->m_analogToDigitalThresholdLineEdit->setText(toQString(std::to_string(Arduino::analogToVoltage(result.second)).substr(0,5)));
            }
        }
    } catch (std::exception &e) {
        std::cout << EXCEPTION_TRYING_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING << e.what();
    }
}

void ManualScreen::onAnalogToDigitalBoxEditingFinished()
{
    using namespace ArduinoPCStrings;
    try{
        if (this->m_analogToDigitalRawState) {
            std::pair<IOStatus, int> result{this->m_arduino->changeAnalogToDigitalThreshold(std::stoi(this->m_analogToDigitalThresholdLineEdit->text().toStdString()), this->m_screenIndex)};
            if (result.first == IOStatus::OPERATION_FAILURE) {
                std::cout << FAILED_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(FAILED_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            } else {
                this->m_analogToDigitalThresholdLineEdit->setText(toQString(result.second));
            }
        } else {
            std::pair<IOStatus, int> result{this->m_arduino->changeAnalogToDigitalThreshold(Arduino::voltageToAnalog(std::stod(this->m_analogToDigitalThresholdLineEdit->text().toStdString())), this->m_screenIndex)};
            if (result.first == IOStatus::OPERATION_FAILURE) {
                std::cout << FAILED_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_CHANGED_ANALOG_TO_DIGITAL_THRESHOLD_STRING) + toQString(result.second) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            } else {
                this->m_analogToDigitalThresholdLineEdit->setText(toQString(std::to_string(Arduino::analogToVoltage(result.second)).substr(0,5)));
            }
        }
    } catch (std::exception &e) {
        std::cout << EXCEPTION_TRYING_TO_CHANGE_ANALOG_TO_DIGITAL_THRESHOLD_STRING << e.what();
    }
}

void ManualScreen::onCloseActionTriggered(bool)
{
    this->close();
}

void ManualScreen::hideEvent(QHideEvent *event)
{
    (void)event;
    if (this->isHidden()) {
        pauseUpdates();
    } else {
        resumeUpdates();
    }
    return QMainWindow::hideEvent(event);
}

void ManualScreen::showEvent(QShowEvent *event)
{
    if (!this->isHidden()) {
        resumeUpdates();
    } else {
        pauseUpdates();
    }
    if (!this->m_aliasesSet) {
        setAliases();
    }
    return QMainWindow::showEvent(event);
}

void ManualScreen::setAliases()
{
    using namespace ArduinoPCStrings;
    std::shared_ptr<GPIO> tempGpio{nullptr};
    QWidget *widget{nullptr};
    for (auto &it : this->m_arduino->AVAILABLE_PINS()) {
        if (this->m_ioStatusViews.find(it) != this->m_ioStatusViews.end()) {
            tempGpio = this->m_ioStatusViews.find(it)->second->gpio();
            widget = this->m_ioStatusViews.find(it)->second->pinNumberLabel();
        } else {
            continue;
        }
        if (tempGpio) {
            if ((tempGpio->ioType() == IOType::DIGITAL_INPUT) || (tempGpio->ioType() == IOType::DIGITAL_INPUT_PULLUP)) {
                PullupStatePushButton *tempLabel{dynamic_cast<PullupStatePushButton *>(widget)};
                if (tempLabel) {
                    if ((this->m_arduino->gpioAliasByPinNumber(it) != std::to_string(it)) &&
                        (this->m_arduino->gpioAliasByPinNumber(it) != (PIN_LABEL_BASE + std::to_string(it)))) {
                        tempLabel->setText(toQString(this->m_arduino->gpioAliasByPinNumber(it)));
                    }
                    if (this->m_arduino->gpioPinByPinNumber(it)->ioType() == DIGITAL_INPUT_PULLUP) {
                        tempLabel->setToolTip(toQString(PIN_IS_PULLED_UP_LABEL_STRING));
                    } else {
                        tempLabel->setToolTip(toQString(PIN_IS_NOT_PULLED_UP_LABEL_STRING));
                    }
                }
            } else if (tempGpio->ioType() == IOType::DIGITAL_OUTPUT) {
                DIOStateLabel *tempLabel{dynamic_cast<DIOStateLabel *>(widget)};
                if (tempLabel) {
                    if ((this->m_arduino->gpioAliasByPinNumber(it) != std::to_string(it)) &&
                        (this->m_arduino->gpioAliasByPinNumber(it) != (PIN_LABEL_BASE + std::to_string(it)))) {
                        tempLabel->setToolTip(tempLabel->text());
                        tempLabel->setText(toQString(this->m_arduino->gpioAliasByPinNumber(it)));
                    }
                    if (this->m_arduino->softDigitalRead(tempGpio).second) {
                        tempLabel->setToolTip(toQString(PIN_IN_HIGH_STATE_STRING));
                    } else {
                        tempLabel->setToolTip(toQString(PIN_IN_LOW_STATE_STRING));
                    }
                }
            } else if (tempGpio->ioType() == IOType::ANALOG_INPUT) {
                AnalogIORawStatePushButton *tempLabel{dynamic_cast<AnalogIORawStatePushButton *>(widget)};
                if (tempLabel) {
                   if ((this->m_arduino->gpioAliasByPinNumber(it) != std::to_string(it)) &&
                        (this->m_arduino->gpioAliasByPinNumber(it) != this->m_arduino->analogPinFromNumber(it)) &&
                        (this->m_arduino->gpioAliasByPinNumber(it) != (PIN_LABEL_BASE + this->m_arduino->analogPinFromNumber(it))) &&
                        (this->m_arduino->gpioAliasByPinNumber(it)) != (PIN_LABEL_BASE + std::to_string(it))) {
                            tempLabel->setToolTip(tempLabel->text());
                            tempLabel->setText(toQString(this->m_arduino->gpioAliasByPinNumber(it)));
                    }
                }
            } else if (tempGpio->ioType() == IOType::ANALOG_OUTPUT) {
                AnalogOutputEntryBox *tempLabel{dynamic_cast<AnalogOutputEntryBox *>(widget)};
                if (tempLabel) {
                    if ((this->m_arduino->gpioAliasByPinNumber(it) != std::to_string(it)) &&
                        (this->m_arduino->gpioAliasByPinNumber(it) != PIN_LABEL_BASE + std::to_string(it))) {
                        tempLabel->setToolTip(tempLabel->text());
                        tempLabel->setText(toQString(this->m_arduino->gpioAliasByPinNumber(it)));
                    }
                }
            }
        }
    }
}


void ManualScreen::pauseUpdates()
{
    disconnect(this->m_manualUpdateTimer.get(), SIGNAL(timeout()), this, SLOT(updateManualScreen()));
    this->m_manualUpdateTimer->stop();
    this->m_synchronizeGpioTimer->stop();
}

void ManualScreen::resumeUpdates()
{
    connect(this->m_manualUpdateTimer.get(), SIGNAL(timeout()), this, SLOT(updateManualScreen()));
    this->m_ioReportFuture->start();
    this->m_synchronizeGpioTimer->start();
    try {
        this->m_manualUpdateTimer->start();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

void ManualScreen::onAnalogOutputLineEditLostFocus(int screenIndex)
{
    (void)screenIndex;
}


std::shared_ptr<Arduino> ManualScreen::arduino()
{
    return this->m_arduino;
}

IOReport ManualScreen::ioReport(ManualScreen *manualScreen)
{
    using namespace ArduinoPCStrings;
    try {
        return manualScreen->arduino()->ioReportRequest(manualScreen->screenIndex());
    } catch (std::exception &e) {
        std::cout << STD_EXCEPTION_IN_IO_REPORT_STRING << e.what() << std::endl;
        return IOReport();
    }
}

void ManualScreen::onDIOStateLabelClicked(int pinNumber, bool checked)
{
    if (this->m_ioStatusViews.find(pinNumber) == this->m_ioStatusViews.end()) {
        return;
    }
    IOStatusView *temp{this->m_ioStatusViews.at(pinNumber)};
    DIOStatePushButton *pb{dynamic_cast<DIOStatePushButton *>(temp->stateLabel())};
    if (pb) {
        return onDIOStatePushButtonClicked(pinNumber, pb, this->m_screenIndex, checked);
    }
}

void ManualScreen::onDIOStatePushButtonClicked(int pinNumber, DIOStatePushButton *qpb, int screenIndex, bool checked)
{
    using namespace ArduinoPCStrings;
    (void)checked;
    (void)screenIndex;
    std::pair<IOStatus, bool> firstResult{this->m_arduino->softDigitalRead(pinNumber, screenIndex)};
    if (firstResult.first == OPERATION_FAILURE) {
        std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_SOFT_DIGITAL_READ_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
        this->m_uiPtr->statusBar->showMessage(toQString(FAILED_SOFT_DIGITAL_READ_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
        qpb->setBlockIconChange(false);
        return;
    }
    if (firstResult.second) {
        try {
            std::pair<IOStatus, bool> result{this->m_arduino->digitalWrite(pinNumber, LOW, screenIndex)};
            if (result.first == IOStatus::OPERATION_FAILURE) {
                std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_DIGITAL_WRITE_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_DIGITAL_WRITE_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            } else {
                qpb->setBlockIconChange(true);
                if (result.second == LOW) {
                    std::cout << PIN_NUMBER_LABEL << pinNumber << SUCCESSFULLY_WRITTEN_LOW_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                    this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(SUCCESSFULLY_WRITTEN_LOW_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
                    qpb->forceSetIcon(this->m_apciPtr->DIO_HIGH_STATE_ICON);
                    QWidget *temp{this->m_ioStatusViews.find(pinNumber)->second->pinNumberLabel()};
                    if (temp) {
                        DIOStateLabel *tempLabel{dynamic_cast<DIOStateLabel*>(temp)};
                        if (tempLabel) {
                            tempLabel->setToolTip(toQString(PIN_IN_LOW_STATE_STRING));
                        }
                    }
                } else {
                    this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_DIGITAL_WRITE_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
                }
            }
        } catch (std::exception &e) {
           (void)e;
           std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_DIGITAL_WRITE_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
           this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_DIGITAL_WRITE_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
        }

    } else {
        try {
            std::pair<IOStatus, bool> result{this->m_arduino->digitalWrite(pinNumber, HIGH, screenIndex)};
            if (result.first == IOStatus::OPERATION_FAILURE) {
                std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_DIGITAL_WRITE_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_DIGITAL_WRITE_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            } else {
                qpb->setBlockIconChange(true);
                if (result.second == HIGH) {
                    std::cout << PIN_NUMBER_LABEL << pinNumber << SUCCESSFULLY_WRITTEN_HIGH_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                    this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(SUCCESSFULLY_WRITTEN_HIGH_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
                    qpb->forceSetIcon(this->m_apciPtr->DIO_HIGH_STATE_ICON);
                    QWidget *temp{this->m_ioStatusViews.find(pinNumber)->second->pinNumberLabel()};
                    if (temp) {
                        DIOStateLabel *tempLabel{dynamic_cast<DIOStateLabel*>(temp)};
                        if (tempLabel) {
                            tempLabel->setToolTip(toQString(PIN_IN_HIGH_STATE_STRING));
                        }
                    }
                } else {
                    std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_DIGITAL_WRITE_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                    this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_DIGITAL_WRITE_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
                }
            }
        } catch (std::exception &e) {
           (void)e;
           std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_DIGITAL_WRITE_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
           this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_DIGITAL_WRITE_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
        }
    }
    qpb->setBlockIconChange(false);
}

void ManualScreen::onPullupStatePushButtonClicked(int pinNumber, PullupStatePushButton *pspb, int screenIndex, bool clicked)
{
    (void)clicked;
    (void)screenIndex;
    using namespace ArduinoPCStrings;
    std::pair<IOStatus, IOType> firstResult{this->m_arduino->currentPinMode(pinNumber, screenIndex)};
    if (firstResult.first == OPERATION_FAILURE) {
        std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_CURRENT_PIN_MODE_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
        this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_CURRENT_PIN_MODE_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
        return;
    }
    if (firstResult.second == IOType::DIGITAL_INPUT_PULLUP) {
        std::pair<IOStatus, IOType> secondResult{this->m_arduino->pinMode(pinNumber, IOType::DIGITAL_INPUT, screenIndex)};
        if (secondResult.first == OPERATION_FAILURE) {
            std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_PIN_MODE_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
            this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_PIN_MODE_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
        } else {
            if (secondResult.second == IOType::DIGITAL_INPUT) {
                pspb->setStyleSheet(DIGITAL_INPUT_PULLUP_DOWN_STYLESHEET);
                std::cout << PIN_NUMBER_LABEL << pinNumber << SUCCESSFULLY_PULLED_DOWN_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(SUCCESSFULLY_PULLED_DOWN_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
                pspb->setToolTip(toQString(PIN_IS_NOT_PULLED_UP_LABEL_STRING));
            } else {
                std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_PULL_DOWN_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_PULL_DOWN_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            }
        }
    } else {
        std::pair<IOStatus, IOType> secondResult{this->m_arduino->pinMode(pinNumber, IOType::DIGITAL_INPUT_PULLUP, screenIndex)};
        if (secondResult.first == OPERATION_FAILURE) {
            std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_PIN_MODE_STRING << std::endl;
        } else {
            if (secondResult.second == IOType::DIGITAL_INPUT_PULLUP) {
                pspb->setStyleSheet(DIGITAL_INPUT_PULLUP_UP_STYLESHEET);
                std::cout << PIN_NUMBER_LABEL << pinNumber << SUCCESSFULLY_PULLED_UP_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(SUCCESSFULLY_PULLED_UP_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
                pspb->setToolTip(toQString(PIN_IS_PULLED_UP_LABEL_STRING));
            } else {
                std::cout << PIN_NUMBER_LABEL << pinNumber << FAILED_PULL_DOWN_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(SUCCESSFULLY_PULLED_UP_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            }
        }
    }
}

void ManualScreen::onAnalogRawStatePushButtonClicked(int pinNumber, AnalogIORawStatePushButton* aorspb, int screenIndex, bool checked)
{
    (void)pinNumber;
    (void)checked;
    (void)screenIndex;
    QWidget *temp{aorspb->parentWidget()};
    IOStatusView *tempView{dynamic_cast<IOStatusView*>(temp)};
    if (tempView) {
        tempView->setRawState(!tempView->rawState());
    } else {
        if (temp) {
            temp = temp->parentWidget();
            tempView = dynamic_cast<IOStatusView*>(temp);
            if (tempView) {
                tempView->setRawState(!tempView->rawState());
            }
        }
    }
}

/*
void ManualScreen::mousePressEvent(QMouseEvent *event)
{
    if (event->type() == QMouseEvent::MouseButtonRelease) {
        if (event->buttons() == Qt::RightButton) {
            QWidget *item{this->
        }
    }
}
*/

void ManualScreen::synchronizeGPIOTypes(IOReport ioReport)
{
    for (auto &it : ioReport.digitalInputResults()) {
        if (this->m_ioStatusViews.find(it.first) != this->m_ioStatusViews.end()) {
            IOStatusView *tempIosv{this->m_ioStatusViews.find(it.first)->second};
            if ((tempIosv->ioType() != IOType::DIGITAL_INPUT) && (tempIosv->ioType() != IOType::DIGITAL_INPUT_PULLUP)) {
                this->onChangeIOTypeActionTriggered(tempIosv, IOType::DIGITAL_INPUT);
            }
        }
    }

    for (auto &it : ioReport.digitalOutputResults()) {
        if (this->m_ioStatusViews.find(it.first) != this->m_ioStatusViews.end()) {
            IOStatusView *tempIosv{this->m_ioStatusViews.find(it.first)->second};
            if (tempIosv->ioType() != IOType::DIGITAL_OUTPUT) {
                this->onChangeIOTypeActionTriggered(tempIosv, IOType::DIGITAL_OUTPUT);
            }
        }
    }

    for (auto &it : ioReport.analogInputResults()) {
        if (this->m_ioStatusViews.find(it.first) != this->m_ioStatusViews.end()) {
            IOStatusView *tempIosv{this->m_ioStatusViews.find(it.first)->second};
            if (tempIosv->ioType() != IOType::ANALOG_INPUT) {
                this->onChangeIOTypeActionTriggered(tempIosv, IOType::ANALOG_OUTPUT);
            }
        }
    }

    for (auto &it : ioReport.analogOutputResults()) {
        if (this->m_ioStatusViews.find(it.first) != this->m_ioStatusViews.end()) {
            IOStatusView *tempIosv{this->m_ioStatusViews.find(it.first)->second};
            if (tempIosv->ioType() != IOType::ANALOG_OUTPUT) {
                this->onChangeIOTypeActionTriggered(tempIosv, IOType::ANALOG_OUTPUT);
            }
        }
    }
}

void ManualScreen::updateManualScreen()
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    if (this->m_ioReportFuture->isCompleted()) {
        pauseUpdates();
        IOReport ioReport{this->m_ioReportFuture->result()};
        if ((ioReport.digitalInputResults().size() == 0) &&
            (ioReport.digitalOutputResults().size() == 0) &&
            (ioReport.analogInputResults().size() == 0) &&
            (ioReport.analogOutputResults().size() == 0)) {
            resumeUpdates();
            return;
        }
        this->m_synchronizeGpioTimer->update();
        if (this->m_synchronizeGpioTimer->totalMilliseconds() >= ManualScreen::s_SYNCHRONIZE_GPIO_TIMEOUT) {
            this->synchronizeGPIOTypes(ioReport);
        }
        for (auto &it : ioReport.digitalInputResults()) {
            if (this->m_ioStatusViews.find(it.first) != this->m_ioStatusViews.end()) {
                DIOStatePushButton *cpb{dynamic_cast<DIOStatePushButton*>(this->m_ioStatusViews.find(it.first)->second->itemAt(0)->widget())};
                if (cpb) {
                    cpb->setIcon( it.second ? this->m_apciPtr->DIO_HIGH_STATE_ICON : this->m_apciPtr->DIO_LOW_STATE_ICON);
                }
            }
        }
        for (auto &it : ioReport.digitalOutputResults()) {
            if (this->m_ioStatusViews.find(it.first) != this->m_ioStatusViews.end()) {
                DIOStatePushButton *cpb{dynamic_cast<DIOStatePushButton*>(this->m_ioStatusViews.find(it.first)->second->itemAt(0)->widget())};
                if (cpb) {
                    if (!cpb->blockIconChange()) {
                        cpb->setIcon( it.second ? this->m_apciPtr->DIO_HIGH_STATE_ICON : this->m_apciPtr->DIO_LOW_STATE_ICON);
                    }
                }
            }
        }
        for (auto &it : ioReport.analogInputResults()) {
            bool raw{false};
            if (this->m_ioStatusViews.find(it.first) != this->m_ioStatusViews.end()) {
                AnalogIORawStatePushButton *ai{dynamic_cast<AnalogIORawStatePushButton*>(this->m_ioStatusViews.find(it.first)->second->pinNumberLabel())};
                if (ai) {
                    raw = ai->rawState();
                }
                QLabel *ql{dynamic_cast<QLabel*>(this->m_ioStatusViews.find(it.first)->second->itemAt(0)->widget())};
                if (ql) {
                    if (raw) {
                        ql->setText(toQString(it.second));
                        if (it.second >= this->m_arduino->analogToDigitalThreshold()) {
                            ql->setStyleSheet(ANALOG_IO_DIGITAL_STATE_HIGH_STYLESHEET);
                        } else {
                            ql->setStyleSheet(ANALOG_IO_DIGITAL_STATE_LOW_STYLESHEET);
                        }
                    } else {
                        ql->setText(toQString(std::to_string(this->m_arduino->analogToVoltage(it.second)).substr(0, 5)));
                        if (it.second >= this->m_arduino->analogToDigitalThreshold()) {
                            ql->setStyleSheet(ANALOG_IO_DIGITAL_STATE_HIGH_STYLESHEET);
                        } else {
                            ql->setStyleSheet(ANALOG_IO_DIGITAL_STATE_LOW_STYLESHEET);
                        }
                    }
                }
            }
        }
        for (auto &it : ioReport.analogOutputResults()) {
            if (this->m_ioStatusViews.find(it.first) != this->m_ioStatusViews.end()) {
                bool raw{false};
                AnalogIORawStatePushButton *ai{dynamic_cast<AnalogIORawStatePushButton*>(this->m_ioStatusViews.find(it.first)->second->itemAt(1)->widget())};
                if (ai) {
                    raw = ai->rawState();
                }
                QLineEdit *ql{dynamic_cast<QLineEdit*>(this->m_ioStatusViews.find(it.first)->second->itemAt(0)->widget())};
                if ((ql) && (!ql->hasFocus())) {
                    if (raw) {
                        ql->setText(toQString(it.second));
                        if (it.second >= this->m_arduino->analogToDigitalThreshold()) {
                            ql->setStyleSheet(ANALOG_IO_DIGITAL_STATE_HIGH_STYLESHEET);
                        } else {
                            ql->setStyleSheet(ANALOG_IO_DIGITAL_STATE_HIGH_STYLESHEET);
                        }
                    } else {
                        ql->setText(toQString(std::to_string(this->m_arduino->analogToVoltage(it.second)).substr(0, 5)));
                        ql->setText(toQString(it.second));
                        if (it.second >= this->m_arduino->analogToDigitalThreshold()) {
                            ql->setStyleSheet(ANALOG_IO_DIGITAL_STATE_HIGH_STYLESHEET);
                        } else {
                            ql->setStyleSheet(ANALOG_IO_DIGITAL_STATE_HIGH_STYLESHEET);
                        }
                    }
                }
            }
        }
        resumeUpdates();
    }
}

void ManualScreen::onAnalogOutputLineEditReturnPressed(int pinNumber, AnalogOutputEntryBox *aoeb, int screenIndex)
{
    using namespace MathUtilities;
    using namespace ArduinoPCStrings;
    if (!aoeb) {
        return;
    }
    bool rawState{false};
    IOStatusView *view{aoeb->parentIOStatusView()};
    if (view) {
        rawState = view->rawState();
    }
    if (rawState) {
        int state{0};
        try {
            state = std::stoi(aoeb->text().toStdString());
        } catch (std::exception &e) {
            (void)e;
            std::unique_ptr<QMessageBox> warningBox;
            warningBox->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
            warningBox->setWindowTitle(INVALID_ANALOG_STATE_WINDOW_TITLE);
            warningBox->setText(INVALID_ANALOG_STATE_BASE_STRING + aoeb->text());
            warningBox->exec();
            return;
        }
        if (state > Arduino::ANALOG_MAX) {
            state = Arduino::ANALOG_MAX;
        }
        aoeb->setText(toQString(state));
        aoeb->setStyleSheet(ANALOG_OUTPUT_ENTRY_BOX_PENDING_CHANGE_STYLE_SHEET);
        std::pair<IOStatus, double> result{this->m_arduino->analogWriteRaw(pinNumber, state, screenIndex)};
        if (result.first == IOStatus::OPERATION_FAILURE) {
            std::cout << PIN_LABEL_BASE << pinNumber << FAILED_ANALOG_WRITE_RAW_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
        } else {
            if (state == result.second) {
                std::cout << PIN_LABEL_BASE << pinNumber << ANALOG_WRITE_RAW_SUCCESSFUL_BASE_STRING << state << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(ANALOG_WRITE_RAW_SUCCESSFUL_BASE_STRING) + toQString(state) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            } else {
                std::cout << PIN_LABEL_BASE << pinNumber << FAILED_ANALOG_WRITE_RAW_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_ANALOG_WRITE_RAW_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            }
        }
        aoeb->setStyleSheet(ANALOG_OUTPUT_ENTRY_BOX_STABLE_STYLE_SHEET);
    } else {
        double state{0.00};
        try {
            state = std::stof(aoeb->text().toStdString());
        } catch (std::exception &e) {
            (void)e;
            std::unique_ptr<QMessageBox> warningBox;
            warningBox->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
            warningBox->setWindowTitle(INVALID_ANALOG_STATE_WINDOW_TITLE);
            warningBox->setText(INVALID_ANALOG_STATE_BASE_STRING + aoeb->text());
            warningBox->exec();
            return;
        }
        if (state > Arduino::VOLTAGE_MAX) {
            state = Arduino::VOLTAGE_MAX;
        }
        aoeb->setText(toQString(std::to_string(state).substr(0,5)));
        aoeb->setStyleSheet(ANALOG_OUTPUT_ENTRY_BOX_PENDING_CHANGE_STYLE_SHEET);
        std::pair<IOStatus, double> result{this->m_arduino->analogWrite(pinNumber, state, screenIndex)};
        if (result.first == IOStatus::OPERATION_FAILURE) {
            std::cout << PIN_LABEL_BASE << pinNumber << FAILED_ANALOG_WRITE_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
            this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(ANALOG_WRITE_SUCCESSFUL_BASE_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
        } else {
            if (approximatelyEquals(state, result.second, 0.3)) {
                std::cout << PIN_LABEL_BASE << pinNumber << ANALOG_WRITE_SUCCESSFUL_BASE_STRING << state << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(ANALOG_WRITE_SUCCESSFUL_BASE_STRING) + toQString(state) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            } else {
                std::cout << PIN_LABEL_BASE << pinNumber << FAILED_ANALOG_WRITE_STRING << " (" << this->m_arduino->serialPort()->portName() << ")" << std::endl;
                this->m_uiPtr->statusBar->showMessage(toQString(PIN_NUMBER_LABEL) + toQString(pinNumber) + toQString(FAILED_ANALOG_WRITE_STRING) + " (" + toQString(this->m_arduino->serialPort()->portName()) + ")", ManualScreen::s_MANUAL_SCREEN_STATUS_BAR_TIMEOUT);
            }
        }
        aoeb->setStyleSheet(ANALOG_OUTPUT_ENTRY_BOX_STABLE_STYLE_SHEET);
    }
}


void ManualScreen::bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget)
{
    this->m_qDesktopWidget.reset();
    this->m_qDesktopWidget = qDesktopWidget;
}

void ManualScreen::bindArduinoPCIcons(std::shared_ptr<ArduinoPCIcons> apciPtr)
{
    this->m_apciPtr.reset();
    this->m_apciPtr = apciPtr;
}

void ManualScreen::bindGlobalLogger(std::shared_ptr<GlobalLogger> globalLogger)
{
    this->m_globalLogger.reset();
    this->m_globalLogger = globalLogger;
}

int ManualScreen::screenIndex() const
{
    return this->m_screenIndex;
}

void ManualScreen::setScreenIndex(int screenIndex)
{
    this->m_screenIndex = screenIndex;
}

void ManualScreen::closeEvent(QCloseEvent *ce)
{
    emit (aboutToClose(this->m_screenIndex));
    QMainWindow::closeEvent(ce);
}

std::shared_ptr<Ui::ManualScreen> ManualScreen::uiPtr() const
{
    return this->m_uiPtr;
}

void ManualScreen::centerAndFitWindow()
{
    calculateXYPlacement();
    this->move(this->m_xPlacement, this->m_yPlacement);
}

int ManualScreen::xPlacement() const
{
    return this->m_xPlacement;
}

int ManualScreen::yPlacement() const
{
    return this->m_yPlacement;
}

void ManualScreen::calculateXYPlacement()
{
    std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qDesktopWidget->availableGeometry())};
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
    this->m_yPlacement = (avail->height()/2)-(this->height()/2) - MainWindow::TASKBAR_HEIGHT();
}
