#include "canterminal/cancommandhistoryaction.h"

CanCommandHistoryAction::CanCommandHistoryAction(const QString &text, int index, const CanMessage &canMessage, QWidget *parent) :
    QAction{text, parent},
    m_index{index},
    m_canMessage{canMessage}
{
    connect(this, SIGNAL(triggered(bool)), this, SLOT(emitCustomSignal(bool)));
}

void CanCommandHistoryAction::setCanMessage(const CanMessage &canMessage)
{
    this->m_canMessage = canMessage;
}

CanMessage CanCommandHistoryAction::canMessage() const
{
    return this->m_canMessage;
}

int CanCommandHistoryAction::index() const
{
    return this->m_index;
}

void CanCommandHistoryAction::setIndex(int index)
{
    this->m_index = index;
}

void CanCommandHistoryAction::emitCustomSignal(bool checked)
{
    emit (triggered(this, checked));
}
