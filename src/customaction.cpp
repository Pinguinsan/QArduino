#include "customaction.h"

CustomAction::CustomAction(CustomActionType customActionType, int screenIndex) :
    m_customActionType{customActionType},
    m_screenIndex{screenIndex},
    m_blockClicks{false},
    m_isTriggered{false}
{
    connect(this, SIGNAL(triggered(bool)), this, SLOT(emitCustomSignal()));
    this->setCheckable(true);
}

CustomActionType CustomAction::customActionType() const
{
    return this->m_customActionType;
}

void CustomAction::setScreenIndex(int screenIndex)
{
    this->m_screenIndex = screenIndex;
}

void CustomAction::setBlockClicks(bool blockClicks)
{
    this->m_blockClicks = blockClicks;
}

int CustomAction::screenIndex() const
{
    return this->m_screenIndex;
}

bool CustomAction::blockClicks() const
{
    return this->m_blockClicks;
}

void CustomAction::emitCustomSignal()
{
    if (this->m_blockClicks) {
        this->setChecked(this->m_isTriggered);
    } else {
        this->m_isTriggered = true;
        emit(triggered(this->m_customActionType, this->m_screenIndex, this->isChecked()));
    }
}
