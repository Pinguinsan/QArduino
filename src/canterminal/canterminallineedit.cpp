#include "canterminal/canterminallineedit.h"

CanTerminalLineEdit::CanTerminalLineEdit(QWidget *parent) :
    QLineEdit{parent},
    m_byteOrderIndex{255},
    m_screenIndex{-1},
    m_temporaryEnterEventString{""},
    m_inputValidatorRegEx{std::make_shared<QRegExp>()},
    m_inputValidator{std::make_shared<QRegExpValidator>()}
{
    using namespace ArduinoPCStrings;
    this->m_inputValidatorRegEx->setPattern(CAN_BYTE_LINE_EDIT_REGEX);
}

void CanTerminalLineEdit::focusInEvent(QFocusEvent *event)
{
    this->m_temporaryEnterEventString = this->text();
    return QLineEdit::focusInEvent(event);
}

void CanTerminalLineEdit::focusOutEvent(QFocusEvent *event)
{
    using namespace GeneralUtilities;
    if ((this->text() == "") || (isWhitespace(this->text().toStdString()))) {
        this->setText(this->m_temporaryEnterEventString);
    }
    return QLineEdit::focusOutEvent(event);
}

void CanTerminalLineEdit::enterEvent(QEvent *event)
{
    return QLineEdit::enterEvent(event);
    //this->m_temporaryEnterEventString = this->text();
}

void CanTerminalLineEdit::leaveEvent(QEvent *event)
{
    return QLineEdit::leaveEvent(event);
}

void CanTerminalLineEdit::setupParameters(unsigned int byteOrderIndex, int screenIndex)
{
    this->m_byteOrderIndex = byteOrderIndex;
    this->m_screenIndex = screenIndex;
    connect(this, SIGNAL(editingFinished()), this, SLOT(emitCustomSignal()));
}

unsigned int CanTerminalLineEdit::byteOrderIndex() const
{
    return this->m_byteOrderIndex;
}

int CanTerminalLineEdit::screenIndex() const
{
    return this->m_screenIndex;
}

void CanTerminalLineEdit::setByteOrderIndex(unsigned int byteOrderIndex)
{
    this->m_byteOrderIndex = byteOrderIndex;
}

void CanTerminalLineEdit::setScreenIndex(int screenIndex)
{
    this->m_screenIndex = screenIndex;
}


void CanTerminalLineEdit::keyPressEvent(QKeyEvent *qke)
{
    if (qke) {
        if ((qke->key() == Qt::Key_Enter) || (qke->key() == Qt::Key_Return)) {
            this->parentWidget()->setFocus();
            emit returnPressed(this, this->m_byteOrderIndex, this->m_screenIndex);
        } else if (qke->key() == Qt::Key_Up) {
            emit (upArrowPressed(this, this->m_byteOrderIndex, this->m_screenIndex));
        } else if (qke->key() == Qt::Key_Down) {
            emit (downArrowPressed(this, this->m_byteOrderIndex, this->m_screenIndex));
        } else if (qke->key() == Qt::Key_Left) {
            emit (leftArrowPressed(this, this->m_byteOrderIndex, this->m_screenIndex));
        } else if (qke->key() == Qt::Key_Right) {
            emit (rightArrowPressed(this, this->m_byteOrderIndex, this->m_screenIndex));
        } else if (qke->key() == Qt::Key_Escape) {
            emit (escapePressed(this, this->m_byteOrderIndex, this->m_screenIndex));
        } else if (qke->key() == Qt::Key_Alt) {
            emit (altPressed(this, this->m_byteOrderIndex, this->m_screenIndex));
        } else if ((qke->key() == Qt::Key_A) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            emit (ctrlAPressed(this, this->m_byteOrderIndex, this->m_screenIndex));
        } else if ((qke->key() == Qt::Key_E) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            emit (ctrlEPressed(this, this->m_byteOrderIndex, this->m_screenIndex));
        } else if ((qke->key() == Qt::Key_U) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            emit (ctrlUPressed(this, this->m_byteOrderIndex, this->m_screenIndex));
        } else if ((qke->key() == Qt::Key_G) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            emit (ctrlGPressed(this, this->m_byteOrderIndex, this->m_screenIndex));
        }
        else {
            return QLineEdit::keyPressEvent(qke);
        }
    }
}

void CanTerminalLineEdit::emitCustomSignal()
{
    emit (editingFinished(this, this->m_byteOrderIndex, this->m_screenIndex));
}
