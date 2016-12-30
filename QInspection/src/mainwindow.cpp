#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(std::unique_ptr<QSerialPort> serialPort, QWidget *parent) :
    QMainWindow{parent},
    _ui{std::make_unique<Ui::MainWindow>()},
    _arduino{std::make_unique<ArduinoMega>(std::move(serialPort))}

{
    this->_ui->setupUi(this);
}

MainWindow::~MainWindow()
{

}
