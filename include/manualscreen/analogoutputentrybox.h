#ifndef ARDUINOPC_ANALOGOUTPUTENTRYBOX_H
#define ARDUINOPC_ANALOGOUTPUTENTRYBOX_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QIntValidator>
#include <QDoubleValidator>
#include <memory>
#include "iostatusview.h"

class IOStatusView;

class AnalogOutputEntryBox : public QLineEdit
{
    Q_OBJECT

public:
    AnalogOutputEntryBox(int pinNumber, int screenIndex, IOStatusView *parentIOStatusView, QWidget *parent = 0);
    int pinNumber() const;
    int screenIndex() const;

    void setPinNumber(int pinNumber);
    void setScreenIndex(int screenIndex);
    void keyPressEvent(QKeyEvent *qke);
    bool rawState() const;
    void setRawState(bool rawState);

    void bindParentIOStatusView(IOStatusView *parentIOStatusView);
    IOStatusView *parentIOStatusView() const;

signals:
    void editingFinished(int screenIndex);
    void returnPressed(int pinNumber, AnalogOutputEntryBox *aoeb, int screenIndex);

private slots:
    void emitCustomSignal();

private:
    int m_pinNumber;
    int m_screenIndex;
    IOStatusView *m_parentIOStatusView;
    std::shared_ptr<QDoubleValidator> m_inputValidator;
    std::shared_ptr<QIntValidator> m_rawInputValidator;
};

#endif //ARDUINOPC_ANALOGOUTPUTENTRYBOX_H
