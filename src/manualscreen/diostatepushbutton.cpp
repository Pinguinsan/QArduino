#include "manualscreen/diostatepushbutton.h"

const int DIOStatePushButton::s_UNSET_BLOCK_ICON_CHANGE_TIMEOUT{100};

DIOStatePushButton::DIOStatePushButton(int pinNumber, int screenIndex, bool blockClicks) :
    QPushButton{},
    m_pinNumber{pinNumber},
    m_screenIndex{screenIndex},
    m_blockClicks{blockClicks},
    m_blockIconChange{false}
{

}

void DIOStatePushButton::setIcon(const QIcon &icon)
{
    if (!this->m_blockIconChange) {
        return QPushButton::setIcon(icon);
    } else {
        return;
    }
}

void DIOStatePushButton::forceSetIcon(const QIcon &icon)
{
    QPushButton::setIcon(icon);
    QTimer::singleShot(DIOStatePushButton::s_UNSET_BLOCK_ICON_CHANGE_TIMEOUT, this, SLOT(unsetBlockIconChange()));
}

void DIOStatePushButton::unsetBlockIconChange()
{
    this->m_blockIconChange = false;
}

void DIOStatePushButton::setBlockIconChange(bool blockIconChange)
{
    this->m_blockIconChange = blockIconChange;
}

bool DIOStatePushButton::blockIconChange() const
{
    return this->m_blockIconChange;
}

void DIOStatePushButton::mousePressEvent(QMouseEvent *e)
{
    if (!this->m_blockClicks) {
        if (e->buttons() == Qt::LeftButton) {
            emit clicked(this->m_pinNumber, this, this->m_screenIndex, this->isChecked());
        }
    } else {
        //
    }
}

int DIOStatePushButton::pinNumber() const
{
    return this->m_pinNumber;
}

bool DIOStatePushButton::blockClicks() const
{
    return this->m_blockClicks;
}

void DIOStatePushButton::setScreenIndex(int screenIndex)
{
    this->m_screenIndex = screenIndex;
}

int DIOStatePushButton::screenIndex() const
{
    return this->m_screenIndex;
}

void DIOStatePushButton::setBlockClicks(bool blockClicks)
{
    this->m_blockClicks = blockClicks;
}

