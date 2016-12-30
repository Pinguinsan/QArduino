#ifndef ARDUINOPC_ANALOGIORAWSTATEPUSHBUTTON_H
#define ARDUINOPC_ANALOGIORAWSTATEPUSHBUTTON_H

#include <QPushButton>
#include <memory>
#include "iostatusview.h"
#include "arduinopcstrings.h"

class IOStatusView;

class AnalogIORawStatePushButton : public QPushButton
{
    Q_OBJECT

public:
    AnalogIORawStatePushButton(int pinNumber, int screenIndex, IOStatusView *parentIOStatusView, QWidget *parent = 0);

    int pinNumber() const;
    int screenIndex() const;
    bool rawState() const;
    void setPinNumber(int pinNumber);
    void setScreenIndex(int screenIndex);
    void bindParentIOStatusView(IOStatusView *parentIOStatusView);
    IOStatusView *parentIOStatusView() const;

signals:
    void clicked(int pinNumber, AnalogIORawStatePushButton *pspb, int screenIndex, bool checked);

private slots:
    void emitCustomSignal();

private:
    int m_pinNumber;
    int m_screenIndex;

    IOStatusView *m_parentIOStatusView;
};

#endif //ARDUINOPC_ANALOGIORAWSTATEPUSHBUTTON_H

