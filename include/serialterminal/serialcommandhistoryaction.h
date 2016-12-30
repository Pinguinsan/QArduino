#ifndef ARDUINOPC_SERIALCOMMANDHISTORYACTION_H
#define ARDUINOPC_SERIALCOMMANDHISTORYACTION_H

#include <QAction>

class SerialCommandHistoryAction : public QAction
{

    Q_OBJECT
public:
    SerialCommandHistoryAction(const QString &text, int index, QWidget *parent = 0);
    int index() const;
    void setIndex(int index);

private slots:
    void emitCustomSignal(bool);

signals:
    void triggered(SerialCommandHistoryAction *serialCommandHistoryAction, bool checked);

private:
    int m_index;

};

#endif //ARDUINOPC_SERIALCOMMANDHISTORYACTION_H
