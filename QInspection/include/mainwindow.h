#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <arduinomega.h>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::unique_ptr<QSerialPort> serialPort, QWidget *parent = 0);
    ~MainWindow();

private:
    std::unique_ptr<Ui::MainWindow> _ui;
    std::unique_ptr<ArduinoMega> _arduino;
};

#endif // MAINWINDOW_H
