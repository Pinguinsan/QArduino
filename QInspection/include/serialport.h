#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>


class SerialPort : public QSerialPort
{
    Q_OBJECT

public:
    void popBuffer();
    std::string currentBuffer();
    std::string popAndReturnCurrentBuffer();
    void sendString(const std::string &str);

signals:
    void readyToRead();

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

private:
    std::shared_ptr<QTimer> _timer;
    QByteArray  _readData;
};

#endif
