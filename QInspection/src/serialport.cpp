#include "serialport.h"

void SerialPort::sendString(const std::string &str)
{
    this->writeData(str.c_str());
}

void SerialPort::popBuffer()
{
    this->_readData.clear();
}

std::string SerialPort::currentBuffer()
{
    return this->_readData.toStdString();
}

std::string SerialPort::popAndReturnCurrentBuffer()
{
    std::string returnString{this->_readData.toStdString()};
    this->_readData.clear();
    return returnString;
}

void SerialPort::handleReadyRead()
{
    this->_readData.append(this->readAll());
    emit(readyToRead());

    if (!this->_timer.isActive())
        this->_timer.start(5000);
}

void SerialPort::handleTimeout()
{
    if (this->_readData.isEmpty()) {
        std::cout << QObject::tr("No data was currently available for reading from port %1").arg(this->portName()) << endl;
    } else {
        std::cout << QObject::tr("Data successfully received from port %1").arg(this->portName()) << endl;
        std::cout << this->_readData << endl;
    }

    //QCoreApplication::quit();
}

void SerialPort::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        std::cout << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(this->_serialPort->portName()).arg(this->_serialPort->errorString()) << endl;
    }
}
