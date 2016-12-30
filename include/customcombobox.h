#ifndef ARDUINOPC_CUSTOMCOMBOBOX_H
#define ARDUINOPC_CUSTOMCOMBOBOX_H

#include <QComboBox>
#include <QSpinBox>
#include <QEvent>
#include <QString>

class CustomComboBox : public QComboBox
{

    Q_OBJECT
public:
    CustomComboBox(int ioStatusViewIndex);
    bool eventFilter(QObject *o, QEvent *e) override;

    int ioStatusViewIndex() const;
    void setIOStatusViewIndex(int index);

signals:
    void currentIndexChanged(CustomComboBox*, QString);

private slots:
    void emitCustomSignal(QString newIndex);

private:
    int m_ioStatusViewIndex;
};

#endif //ARDUINOPC_CUSTOMCOMBOBOX_H
