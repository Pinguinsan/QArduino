#ifndef ARDUINOPC_CUSTOMACTION_H
#define ARDUINOPC_CUSTOMACTION_H

#include <QAction>
#include <QEvent>
#include <QMouseEvent>

enum class CustomActionType { MANUAL_SCREEN, SERIAL_TERMINAL, CAN_TERMINAL };

class CustomAction : public QAction
{
    Q_OBJECT

public:
    CustomAction(CustomActionType customActionType, int screenIndex);
    CustomActionType actionType() const;
    int screenIndex() const;
    bool blockClicks() const;
    void setScreenIndex(int screenIndex);
    void setBlockClicks(bool blockClicks);
    void setActionType(CustomActionType actionType);

    CustomActionType customActionType() const;
signals:
    void triggered(CustomActionType actionType, int screenIndex, bool checked);

private slots:
    void emitCustomSignal();

private:
    CustomActionType m_customActionType;
    int m_screenIndex;
    bool m_blockClicks;
    bool m_isTriggered;
};

#endif //ARDUINOPC_CUSTOMACTION_H
