#include "../include/led2.h"

/*
Led2::Led2() :
    _canCommandStatus{false},
    _igOnDataPacket (std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
    _indicatorStatusDataPacket1 (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket2 (std::vector<unsigned char> {0x00, 0x04, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket3 (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
{
    //Constructor
}
*/

Led2::Led2(CanDataPacket igOnDataPacket, 
            CanDataPacket indicatorStatusDataPacket1, 
            CanDataPacket indicatorStatusDataPacket2, 
            CanDataPacket indicatorStatusDataPacket3) :
            _canCommandStatus{false},
            _igOnDataPacket{igOnDataPacket},
            _indicatorStatusDataPacket1{indicatorStatusDataPacket1},
            _indicatorStatusDataPacket2{indicatorStatusDataPacket2},
            _indicatorStatusDataPacket3{indicatorStatusDataPacket3}
{
    //Constructor
}

Led2::~Led2()
{
    //Destructor
}

void Led2::setCanCommandStatus(bool onOrOff)
{
    this->_canCommandStatus = onOrOff;
}


bool Led2::canCommandStatus() const
{
    return this->_canCommandStatus;
}

CanDataPacket Led2::igOnDataPacket() const
{
    return this->_igOnDataPacket;
}

CanDataPacket Led2::indicatorStatusDataPacket1() const
{
    return this->_indicatorStatusDataPacket1;
}

CanDataPacket Led2::indicatorStatusDataPacket2() const
{
    return this->_indicatorStatusDataPacket2;
}

CanDataPacket Led2::indicatorStatusDataPacket3() const
{
    return this->_indicatorStatusDataPacket3;
}