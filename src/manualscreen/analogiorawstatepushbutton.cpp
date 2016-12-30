#include "manualscreen/analogiorawstatepushbutton.h"

AnalogIORawStatePushButton::AnalogIORawStatePushButton(int pinNumber, int screenIndex, IOStatusView *parentIOStatusView, QWidget *parent) :
    QPushButton{parent},
    m_pinNumber{pinNumber},
    m_screenIndex{screenIndex},
    m_parentIOStatusView{parentIOStatusView}
{
    connect(this, SIGNAL(clicked()), this, SLOT(emitCustomSignal()));
    connect(this, SIGNAL(clicked(int,AnalogIORawStatePushButton*,int,bool)), this->m_parentIOStatusView, SLOT(onAnalogIORawStatePushButtonClicked(int,AnalogIORawStatePushButton*,int,bool)));
}

int AnalogIORawStatePushButton::pinNumber() const
{
    return this->m_pinNumber;
}

int AnalogIORawStatePushButton::screenIndex() const
{
    return this->m_screenIndex;
}

void AnalogIORawStatePushButton::setPinNumber(int pinNumber)
{
    this->m_pinNumber = pinNumber;
}

void AnalogIORawStatePushButton::setScreenIndex(int screenIndex)
{
    this->m_screenIndex = screenIndex;
}

bool AnalogIORawStatePushButton::rawState() const
{
    return this->m_parentIOStatusView->rawState();
}

IOStatusView *AnalogIORawStatePushButton::parentIOStatusView() const
{
    return this->m_parentIOStatusView;
}

void AnalogIORawStatePushButton::bindParentIOStatusView(IOStatusView *parentIOStatusView)
{
    this->m_parentIOStatusView = parentIOStatusView;
}

void AnalogIORawStatePushButton::emitCustomSignal()
{
    using namespace ArduinoPCStrings;
    emit clicked(this->m_pinNumber, this, this->m_screenIndex, this->isChecked());
    if (this->m_parentIOStatusView->rawState()) {
        this->setStyleSheet(ANALOG_OUTPUT_STATE_NORMAL_STYLESHEET);
        this->setToolTip(ANALOG_OUTPUT_STATE_RAW_STRING);
    } else {
        this->setStyleSheet(ANALOG_OUTPUT_STATE_RAW_STYLESHEET);
        this->setToolTip(ANALOG_OUTPUT_STATE_NORMAL_STRING);
    }
}
