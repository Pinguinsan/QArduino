#ifndef ARDUINOPC_CANCOMMANDHISTORYACTION_H
#define ARDUINOPC_CANCOMMANDHISTORYACTION_H

#include <QAction>
#include <arduino.h>

class CanCommandHistoryAction : public QAction
{

    Q_OBJECT
public:
    CanCommandHistoryAction(const QString &text, int index, const CanMessage &canMessage, QWidget *parent = 0);
    int index() const;
    void setIndex(int index);
    void setCanMessage(const CanMessage &canMessage);
    CanMessage canMessage() const;

private slots:
    void emitCustomSignal(bool);

signals:
    void triggered(CanCommandHistoryAction *canCommandHistoryAction, bool checked);

private:
    int m_index;
    CanMessage m_canMessage;

};

#endif //ARDUINOPC_CANCOMMANDHISTORYACTION_H
