#include "../include/led1.h"
/*
Led1::Led1() :
    _canCommandStatus{false},
    _igOnDataPacket (std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
    _indicatorStatusDataPacket1 (std::vector<unsigned char> {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket2 (std::vector<unsigned char> {0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket3 (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
{
    //Constructor
}
*/

Led1::Led1(CanDataPacket igOnDataPacket, 
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

Led1::~Led1()
{
    //Destructor
}

void Led1::setCanCommandStatus(bool onOrOff)
{
    this->_canCommandStatus = onOrOff;
}

bool Led1::canCommandStatus() const
{
    return this->_canCommandStatus;
}
    
CanDataPacket Led1::igOnDataPacket() const
{
    return this->_igOnDataPacket;
}

CanDataPacket Led1::indicatorStatusDataPacket1() const
{
    return this->_indicatorStatusDataPacket1;
}

CanDataPacket Led1::indicatorStatusDataPacket2() const
{
    return this->_indicatorStatusDataPacket2;
}

CanDataPacket Led1::indicatorStatusDataPacket3() const
{
    return this->_indicatorStatusDataPacket3;
}