#include "manualscreen/pullupstatepushbutton.h"

PullupStatePushButton::PullupStatePushButton(int pinNumber, int screenIndex) :
    m_pinNumber{pinNumber},
    m_screenIndex{screenIndex}
{
    connect(this, SIGNAL(clicked()), this, SLOT(emitCustomSignal()));
}

int PullupStatePushButton::pinNumber() const
{
    return this->m_pinNumber;
}

int PullupStatePushButton::screenIndex() const
{
    return this->m_screenIndex;
}

void PullupStatePushButton::setPinNumber(int pinNumber)
{
    this->m_pinNumber = pinNumber;
}

void PullupStatePushButton::setScreenIndex(int screenIndex)
{
    this->m_screenIndex = screenIndex;
}

void PullupStatePushButton::emitCustomSignal()
{
    emit clicked(this->m_pinNumber, this, this->m_screenIndex, this->isChecked());
}
