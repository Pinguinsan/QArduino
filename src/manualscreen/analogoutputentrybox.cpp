#include "manualscreen/analogoutputentrybox.h"

AnalogOutputEntryBox::AnalogOutputEntryBox(int pinNumber, int screenIndex, IOStatusView *parentIOStatusView, QWidget *parent) :
    QLineEdit{parent},
    m_pinNumber{pinNumber},
    m_screenIndex{screenIndex},
    m_parentIOStatusView{parentIOStatusView},
    m_inputValidator{std::make_shared<QDoubleValidator>()},
    m_rawInputValidator{std::make_shared<QIntValidator>()}
{
    connect(this, SIGNAL(editingFinished()), this, SLOT(emitCustomSignal()));
    this->m_rawInputValidator->setBottom(0);
    this->m_inputValidator->setBottom(0.00);
    this->m_rawInputValidator->setTop(Arduino::ANALOG_MAX);
    this->m_inputValidator->setTop(Arduino::VOLTAGE_MAX);
    this->m_inputValidator->setNotation(QDoubleValidator::StandardNotation);
    this->setValidator(this->m_inputValidator.get());
    this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

int AnalogOutputEntryBox::pinNumber() const
{
    return this->m_pinNumber;
}

void AnalogOutputEntryBox::setPinNumber(int pinNumber)
{
    this->m_pinNumber = pinNumber;
}

int AnalogOutputEntryBox::screenIndex() const
{
    return this->m_screenIndex;
}

void AnalogOutputEntryBox::keyPressEvent(QKeyEvent *qke)
{
    if (qke) {
        if ((qke->key() == Qt::Key_Enter) || (qke->key() == Qt::Key_Return)) {
            this->parentWidget()->setFocus();
            emit returnPressed(this->m_pinNumber, this, this->m_screenIndex);
        }
    }
    return QLineEdit::keyPressEvent(qke);
}

bool AnalogOutputEntryBox::rawState() const
{
    return this->m_parentIOStatusView->rawState();
}


void AnalogOutputEntryBox::setRawState(bool rawState)
{
    if (rawState) {
        this->setValidator(this->m_rawInputValidator.get());
    } else {
        this->setValidator(this->m_inputValidator.get());
    }
}

void AnalogOutputEntryBox::setScreenIndex(int screenIndex)
{
    this->m_screenIndex = screenIndex;
}

void AnalogOutputEntryBox::emitCustomSignal()
{
    emit (editingFinished(this->m_screenIndex));
}

IOStatusView *AnalogOutputEntryBox::parentIOStatusView() const
{
    return this->m_parentIOStatusView;
}

void AnalogOutputEntryBox::bindParentIOStatusView(IOStatusView *parentIOStatusView)
{
    this->m_parentIOStatusView = parentIOStatusView;
}
