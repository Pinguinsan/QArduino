#ifndef ARDUINOPC_CUSTOMPUSHBUTTON_H
#define ARDUINOPC_CUSTOMPUSHBUTTON_H

#include <QPushButton>

class CustomPushButton : public QPushButton
{
    Q_OBJECT

public:
    CustomPushButton(int screenIndex);
    void setScreenIndex(int screenIndex);
    int screenIndex() const;

signals:
    void clicked(int screenIndex, bool checked);

private slots:
    void emitCustomSignal(bool checked);

private:
    int m_screenIndex;
};

#endif //ARDUINOPC_CUSTOMPUSHBUTTON_H
