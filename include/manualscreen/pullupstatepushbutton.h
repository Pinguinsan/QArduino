#ifndef ARDUINOPC_PULLUPSTATEPUSHBUTTON_H
#define ARDUINOPC_PULLUPSTATEPUSHBUTTON_H

#include <QPushButton>

class PullupStatePushButton : public QPushButton
{
    Q_OBJECT

public:
    PullupStatePushButton(int pinNumber, int screenIndex);

    int pinNumber() const;
    int screenIndex() const;
    void setPinNumber(int pinNumber);
    void setScreenIndex(int screenIndex);

signals:
    void clicked(int pinNumber, PullupStatePushButton *pspb, int screenIndex, bool checked);

private slots:
    void emitCustomSignal();

private:
    int m_pinNumber;
    int m_screenIndex;
};

#endif //ARDUINOPC_PULLUPSTATEPUSHBUTTON_H
