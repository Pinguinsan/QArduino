#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "mainwindow.h"
#include "generalutilities.h"


int main(int argc, char *argv[])
{
    using namespace GeneralUtilities;
    QApplication qApplication(argc, argv);
    std::unique_ptr<QSerialPort> sp{std::make_unique<QSerialPort>("/dev/ttyACM0")};
    sp->setBaudRate(9600);

    MainWindow mainWindow(std::move(sp));
    mainWindow.show();

    return qApplication.exec();
}
