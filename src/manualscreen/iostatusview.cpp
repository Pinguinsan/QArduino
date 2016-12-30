#include "manualscreen/iostatusview.h"

using namespace ArduinoPCStrings;
using namespace GeneralUtilities;

IOStatusView::IOStatusView(std::shared_ptr<GPIO> gpio,
                           std::shared_ptr<Arduino> arduino,
                           QWidget *parent) :
    QFrame{parent},
    m_gpio{gpio},
    m_arduino{arduino},
    m_contextMenu{std::make_unique<QMenu>(parent)},
    m_inactiveLabelAction{new QAction{toQString(PIN_LABEL_BASE + toQString(this->m_gpio->pinNumber()) + " (" + toQString(this->m_arduino->parseIOType(this->m_gpio->ioType())) + ")")}},
    m_changeAliasAction{new QAction{CHANGE_ALIAS_ACTION_STRING}},
    m_ioTypeChangeActions{new QAction{toQString(CHANGE_IO_TYPE_TO_ACTION_STRING) + toQString(toLowercase(DIGITAL_INPUT_LABEL)), this},
                          new QAction{toQString(CHANGE_IO_TYPE_TO_ACTION_STRING) + toQString(toLowercase(DIGITAL_OUTPUT_LABEL)), this},
                          new QAction{toQString(CHANGE_IO_TYPE_TO_ACTION_STRING) + toQString(toLowercase(ANALOG_INPUT_LABEL)), this},
                          new QAction{toQString(CHANGE_IO_TYPE_TO_ACTION_STRING) + toQString(toLowercase(ANALOG_OUTPUT_LABEL)), this}},
    m_rawState{false}
{
    using namespace ArduinoPCStrings;
    this->m_inactiveLabelAction->setEnabled(false);
    this->setLayout(new QHBoxLayout{parent});
    generateCustomContextMenu(gpio->pinNumber(), gpio->ioType());
    connect(this->m_contextMenu.get(), SIGNAL(triggered(QAction *)), this, SLOT(onContextMenuActionTriggered(QAction *)));
}

void IOStatusView::onContextMenuActionTriggered(QAction *action)
{
    using namespace ArduinoPCStrings;
    if (action) {
        if (action->text().toStdString().find(toLowercase(DIGITAL_INPUT_LABEL)) != std::string::npos) {
            emit (changeIOTypeActionTriggered(this, IOType::DIGITAL_INPUT));
        } else if (action->text().toStdString().find(toLowercase(DIGITAL_OUTPUT_LABEL)) != std::string::npos) {
            emit (changeIOTypeActionTriggered(this, IOType::DIGITAL_OUTPUT));
        } else if (action->text().toStdString().find(toLowercase(ANALOG_INPUT_LABEL)) != std::string::npos) {
            emit (changeIOTypeActionTriggered(this, IOType::ANALOG_INPUT));
        } else if (action->text().toStdString().find(toLowercase(ANALOG_OUTPUT_LABEL)) != std::string::npos) {
            emit (changeIOTypeActionTriggered(this, IOType::ANALOG_OUTPUT));
        } else {
            emit (changeAliasActionTriggered(this));
        }
    }
}

void IOStatusView::onAnalogIORawStatePushButtonClicked(int pinNumber, AnalogIORawStatePushButton *aorspb, int screenIndex, bool checked)
{
    (void)pinNumber;
    (void)aorspb;
    (void)screenIndex;
    (void)checked;
    this->setRawState(!this->rawState());
}

int IOStatusView::pinNumber() const
{
    return this->m_gpio->pinNumber();
}

IOType IOStatusView::ioType() const
{
    return this->m_gpio->ioType();
}

std::shared_ptr<GPIO> IOStatusView::gpio() const
{
    return this->m_gpio;
}

QLayoutItem *IOStatusView::itemAt(int index)
{
    return this->layout()->itemAt(index);
}

QWidget *IOStatusView::stateLabel()
{
    return this->itemAt(0)->widget();
}

QWidget *IOStatusView::pinNumberLabel()
{
    return this->itemAt(1)->widget();
}

void IOStatusView::addWidget(QWidget *widget)
{
    this->layout()->addWidget(widget);
}

void IOStatusView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event) {
        if (event->button() == Qt::RightButton) {
            emit (ioStatusViewContextMenuRequested(this, event->pos()));
        }
    }
    return QFrame::mouseReleaseEvent(event);
}

void IOStatusView::setRawState(bool rawState)
{
    this->m_rawState = rawState;
    QLayoutItem *layoutItem{this->itemAt(0)};
    QWidget *qWidget{nullptr};
    if (layoutItem) {
        qWidget = layoutItem->widget();
    }
    AnalogOutputEntryBox *tempBox{dynamic_cast<AnalogOutputEntryBox*>(qWidget)};
    if (tempBox) {
        tempBox->setRawState(this->m_rawState);
    }
}

bool IOStatusView::rawState() const
{
    return this->m_rawState;
}

QMenu &IOStatusView::contextMenu() const
{
    return *this->m_contextMenu;
}

void IOStatusView::generateCustomContextMenu(int pinNumber, IOType ioType)
{
    using namespace ArduinoPCStrings;
    this->m_contextMenu->clear();
    this->m_inactiveLabelAction->setText(toQString(PIN_LABEL_BASE + toQString(this->m_gpio->pinNumber()) + " (" + toQString(this->m_arduino->parseIOType(ioType)) + ")"));
    this->m_contextMenu->addAction(this->m_inactiveLabelAction);
    this->m_contextMenu->addAction(this->m_changeAliasAction);
    if ((ioType == IOType::DIGITAL_INPUT) || (ioType == IOType::DIGITAL_INPUT_PULLUP)) {
        if (this->m_arduino->isValidDigitalOutputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::DIGITAL_OUTPUT));
        }
        if (this->m_arduino->isValidAnalogInputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::ANALOG_INPUT));
        }
        if (this->m_arduino->isValidAnalogOutputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::ANALOG_OUTPUT));
        }
    } else if (ioType == IOType::DIGITAL_OUTPUT) {
        if (this->m_arduino->isValidDigitalInputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::DIGITAL_INPUT));
        }
        if (this->m_arduino->isValidAnalogInputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::ANALOG_INPUT));
        }
        if (this->m_arduino->isValidAnalogOutputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::ANALOG_OUTPUT));
        }
    } else if (ioType == IOType::ANALOG_INPUT) {
        if (this->m_arduino->isValidDigitalInputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::DIGITAL_INPUT));
        }
        if (this->m_arduino->isValidDigitalOutputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::DIGITAL_OUTPUT));
        }
        if (this->m_arduino->isValidAnalogOutputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::ANALOG_OUTPUT));
        }
    } else if (ioType == IOType::ANALOG_OUTPUT) {
        if (this->m_arduino->isValidDigitalInputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::DIGITAL_INPUT));
        }
        if (this->m_arduino->isValidDigitalOutputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::DIGITAL_OUTPUT));
        }
        if (this->m_arduino->isValidAnalogInputPin(pinNumber)) {
            this->m_contextMenu->addAction(this->m_ioTypeChangeActions.at(IOType::ANALOG_INPUT));
        }
    }
}
