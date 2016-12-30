#include "custompushbutton.h"

CustomPushButton::CustomPushButton(int screenIndex) :
    m_screenIndex{screenIndex}
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(emitCustomSignal(bool)));
}

void CustomPushButton::emitCustomSignal(bool checked)
{
    emit (clicked(this->m_screenIndex, checked));
}

int CustomPushButton::screenIndex() const
{
    return this->m_screenIndex;
}

void CustomPushButton::setScreenIndex(int screenIndex)
{
    this->m_screenIndex = screenIndex;
}


