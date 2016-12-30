#ifndef ARDUINOPC_ALIASCHANGEDIALOG_H
#define ARDUINOPC_ALIASCHANGEDIALOG_H

#include <QDesktopWidget>
#include <QDialog>
#include <QString>
#include <string>
#include <memory>
#include "ui_aliaschangedialog.h"

#include "arduinopcstrings.h"
#include "mainwindow.h"
#include "arduinopcicons.h"

namespace Ui
{
    class AliasChangeDialog;
}

class AliasChangeDialog : public QDialog
{
    Q_OBJECT
public:
    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }
    QString toQString(const QString &qstr) { return qstr; }

    AliasChangeDialog(int pinNumber,
                      std::shared_ptr<QDesktopWidget> qDesktopWidget,
                      std::shared_ptr<ArduinoPCIcons> apciPtr,
                      QWidget *parent = 0);

    void centerAndFitWindow();
    void bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget);
    void bindArduinoPCIcons(std::shared_ptr<ArduinoPCIcons> apciPtr);

    int xPlacement() const;
    int yPlacement() const;

private slots:
    void onOkayButtonClicked(bool checked);
    void onCancelButtonClicked(bool checked);

signals:
    void aliasChangeDialogClosing(int pinNumber, bool change, const QString &newAlias);

private:
    std::unique_ptr<Ui::AliasChangeDialog> m_uiPtr;
    std::shared_ptr<QDesktopWidget> m_qDesktopWidget;
    std::shared_ptr<ArduinoPCIcons> m_apciPtr;
    int m_pinNumber;
    int m_xPlacement;
    int m_yPlacement;

    void calculateXYPlacement();

};

#endif //ARDUINOPC_ALIASCHANGEDIALOG_H
