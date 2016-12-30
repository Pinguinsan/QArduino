#include "manualscreen/diostatelabel.h"

DIOStateLabel::DIOStateLabel(int pinNumber) :
    m_pinNumber{pinNumber}
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(emitCustomSignal(bool)));
}

void DIOStateLabel::emitCustomSignal(bool checked)
{
    emit clicked(this->m_pinNumber, checked);
}
