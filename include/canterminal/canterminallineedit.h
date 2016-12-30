#ifndef ARDUINOPC_CANTERMINALLINEEDIT_H
#define ARDUINOPC_CANTERMINALLINEEDIT_H

#include <QRegExpValidator>
#include <QLineEdit>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QRegExp>
#include <QEvent>
#include <QString>
#include <memory>
#include <generalutilities.h>
#include "arduinopcstrings.h"

class CanTerminalLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    CanTerminalLineEdit(QWidget *parent = 0);
    unsigned int byteOrderIndex() const;
    int screenIndex() const;
    void setByteOrderIndex(unsigned int byteOrderIndex);
    void setScreenIndex(int screenIndex);
    void setupParameters(unsigned int byteOrderIndex, int screenIndex);

    void keyPressEvent(QKeyEvent *qke) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

signals:
    void editingFinished(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void returnPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void upArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void downArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void leftArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void rightArrowPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void escapePressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void altPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void ctrlAPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void ctrlEPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void ctrlUPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);
    void ctrlGPressed(CanTerminalLineEdit *ctle, unsigned int byteOrderIndex, int screenIndex);

private slots:
    void emitCustomSignal();

private:
    unsigned int m_byteOrderIndex;
    int m_screenIndex;
    QString m_temporaryEnterEventString;
    std::shared_ptr<QRegExp> m_inputValidatorRegEx;
    std::shared_ptr<QRegExpValidator> m_inputValidator;

};

#endif //ARDUINOPC_CANTERMINALLINEEDIT_H
