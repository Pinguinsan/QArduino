#include "customcombobox.h"

CustomComboBox::CustomComboBox(int ioStatusViewIndex) :
    m_ioStatusViewIndex{ioStatusViewIndex}
{
    this->installEventFilter(this);
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(emitCustomSignal(QString)));
}

int CustomComboBox::ioStatusViewIndex() const
{
    return this->m_ioStatusViewIndex;
}

void CustomComboBox::setIOStatusViewIndex(int index)
{
    this->m_ioStatusViewIndex = index;
}

bool CustomComboBox::eventFilter(QObject *o, QEvent *e )
{
    if ((e->type() == QEvent::Wheel) && qobject_cast<QAbstractSpinBox*>(o)) {
        e->ignore();
        return true;
    }
    return QComboBox::eventFilter(o, e);
}

void CustomComboBox::emitCustomSignal(QString newIndex)
{
    emit(CustomComboBox::currentIndexChanged(this, newIndex));
}
