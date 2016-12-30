#ifndef ARDUINOPC_IOSTATUSVIEW_H
#define ARDUINOPC_IOSTATUSVIEW_H

#include <QFrame>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <memory>
#include <vector>
#include <arduino.h>

#include "manualscreen/analogiorawstatepushbutton.h"
#include "manualscreen/analogoutputentrybox.h"
#include "manualscreen/diostatepushbutton.h"

#include "arduinopcstrings.h"
#include "arduinopcicons.h"

class AnalogIORawStatePushButton;

class IOStatusView : public QFrame
{
    Q_OBJECT

public:

    IOStatusView(std::shared_ptr<GPIO> gpio,
                 std::shared_ptr<Arduino> arduino,
                 QWidget *parent = 0);
    std::shared_ptr<GPIO> gpio() const;
    QLayoutItem *itemAt(int index);
    QWidget *stateLabel();
    QWidget *pinNumberLabel();

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }
    QString toQString(const QString &qstr) { return qstr; }

    bool rawState() const;
    void setRawState(bool rawState);
    QMenu &contextMenu() const;
    void addWidget(QWidget *widget);

    void mouseReleaseEvent(QMouseEvent *event);
    int pinNumber() const;
    IOType ioType() const;
public slots:
    void onAnalogIORawStatePushButtonClicked(int pinNumber, AnalogIORawStatePushButton *aorspb, int screenIndex, bool checked);

signals:
    void ioStatusViewContextMenuRequested(IOStatusView *iosv, QPoint pos);
    void changeAliasActionTriggered(IOStatusView *iosv);
    void changeIOTypeActionTriggered(IOStatusView *iosv, IOType newIOType);

private slots:
    void onContextMenuActionTriggered(QAction *action);
private:
    std::shared_ptr<GPIO> m_gpio;
    std::shared_ptr<Arduino> m_arduino;
    std::unique_ptr<QMenu> m_contextMenu;
    QAction *m_inactiveLabelAction;
    QAction *m_changeAliasAction;
    std::vector<QAction*> m_ioTypeChangeActions;
    bool m_rawState;
    void generateCustomContextMenu(int pinNumber, IOType ioType);
};

#endif //ARDUINOPC_IOSTATUSVIEW_H
