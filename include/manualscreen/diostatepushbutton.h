#ifndef ARDUINOPC_DIOSTATEPUSHBUTTON_H
#define ARDUINOPC_DIOSTATEPUSHBUTTON_H

#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>

class DIOStatePushButton : public QPushButton
{
    Q_OBJECT

public:
    DIOStatePushButton(int pinNumber, int screenIndex, bool blockClicks = false);
    void mousePressEvent(QMouseEvent *e);
    int pinNumber() const;
    bool blockClicks() const;
    void setBlockClicks(bool blockClocks);
    void setScreenIndex(int screenIndex);
    int screenIndex() const;

    void setBlockIconChange(bool blockIconChange);
    bool blockIconChange() const;
    void setIcon(const QIcon &icon);

    void forceSetIcon(const QIcon &icon);
signals:
    void clicked(int pinNumber, DIOStatePushButton *qpb, bool checked = false);
    void clicked(int pinNumber, DIOStatePushButton *qpb, int screenIndex, bool checked = false);

private slots:
    void unsetBlockIconChange();
private:
    int m_pinNumber;
    int m_screenIndex;
    bool m_blockClicks;
    bool m_blockIconChange;

    static const int s_UNSET_BLOCK_ICON_CHANGE_TIMEOUT;
};

#endif //ARDUINOPC_DIOSTATEPUSHBUTTON_H
