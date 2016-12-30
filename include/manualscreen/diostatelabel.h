#ifndef ARDUINOPC_DIOSTATELABEL_H
#define ARDUINOPC_DIOSTATELABEL_H

#include <string>
#include <QString>
#include <QPushButton>


class DIOStateLabel : public QPushButton
{
    Q_OBJECT

public:
    DIOStateLabel(int pinNumber);

signals:
    void clicked(int pinNumber, bool checked);

private slots:
    void emitCustomSignal(bool checked);


private:
    int m_pinNumber;
};

#endif //ARDUINOPC_DIOSTATELABEL_H
