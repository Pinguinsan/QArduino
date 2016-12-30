#include "manualscreen/aliaschangedialog.h"
#include "ui_aliaschangedialog.h"

AliasChangeDialog::AliasChangeDialog(int pinNumber,
                                     std::shared_ptr<QDesktopWidget> qDesktopWidget,
                                     std::shared_ptr<ArduinoPCIcons> apciPtr,
                                     QWidget *parent) :
    QDialog{parent},
    m_uiPtr{std::make_unique<Ui::AliasChangeDialog>()},
    m_qDesktopWidget{qDesktopWidget},
    m_apciPtr{apciPtr},
    m_pinNumber{pinNumber},
    m_xPlacement{0},
    m_yPlacement{0}

{
    using namespace ArduinoPCStrings;
    this->m_uiPtr->setupUi(this);
    this->setWindowIcon(this->m_apciPtr->MAIN_WINDOW_ICON);
    this->setWindowTitle(toQString(ALIAS_CHANGE_DIALOG_WINDOW_TITLE) + toQString(pinNumber));
    connect(this->m_uiPtr->okayButton, SIGNAL(clicked(bool)), this, SLOT(onOkayButtonClicked(bool)));
    connect(this->m_uiPtr->cancelButton, SIGNAL(clicked(bool)), this, SLOT(onCancelButtonClicked(bool)));
}

void AliasChangeDialog::bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget)
{
    this->m_qDesktopWidget.reset();
    this->m_qDesktopWidget = qDesktopWidget;
}

void AliasChangeDialog::bindArduinoPCIcons(std::shared_ptr<ArduinoPCIcons> apciPtr)
{
    this->m_apciPtr.reset();
    this->m_apciPtr = apciPtr;
}

void AliasChangeDialog::onOkayButtonClicked(bool checked)
{
    (void)checked;
    emit(aliasChangeDialogClosing(this->m_pinNumber, true, this->m_uiPtr->newAliasBox->text()));
    this->close();
    this->deleteLater();
}

void AliasChangeDialog::onCancelButtonClicked(bool checked)
{
    (void)checked;
    emit(aliasChangeDialogClosing(this->m_pinNumber, false, this->m_uiPtr->newAliasBox->text()));
    this->close();
    this->deleteLater();
}

void AliasChangeDialog::centerAndFitWindow()
{
    calculateXYPlacement();
    this->move(this->m_xPlacement, this->m_yPlacement);
}

int AliasChangeDialog::xPlacement() const
{
    return this->m_xPlacement;
}

int AliasChangeDialog::yPlacement() const
{
    return this->m_yPlacement;
}

void AliasChangeDialog::calculateXYPlacement()
{
    std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qDesktopWidget->availableGeometry())};
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
    this->m_yPlacement = (avail->height()/2)-(this->height()/2) - MainWindow::TASKBAR_HEIGHT();
}
