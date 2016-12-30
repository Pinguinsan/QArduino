#include "serialterminal/serialcommandhistoryaction.h"

SerialCommandHistoryAction::SerialCommandHistoryAction(const QString &text, int index, QWidget *parent) :
    QAction{text, parent},
    m_index{index}
{
    connect(this, SIGNAL(triggered(bool)), this, SLOT(emitCustomSignal(bool)));
}

int SerialCommandHistoryAction::index() const
{
    return this->m_index;
}

void SerialCommandHistoryAction::setIndex(int index)
{
    this->m_index = index;
}

void SerialCommandHistoryAction::emitCustomSignal(bool checked)
{
    emit (triggered(this, checked));
}
