#include "../include/led3.h"

/*
Led3::Led3() :
    _canCommandStatus{false},
    _igOnDataPacket (std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
    _indicatorStatusDataPacket1 (std::vector<unsigned char> {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket2 (std::vector<unsigned char> {0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket3 (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
{
    //Constructor
}
*/

Led3::Led3(CanDataPacket igOnDataPacket, 
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

Led3::~Led3()
{
    //Destructor
}

void Led3::setCanCommandStatus(bool onOrOff)
{
    this->_canCommandStatus = onOrOff;
}


bool Led3::canCommandStatus() const
{
    return this->_canCommandStatus;
}
    
CanDataPacket Led3::igOnDataPacket() const
{
    return this->_igOnDataPacket;
}

CanDataPacket Led3::indicatorStatusDataPacket1() const
{
    return this->_indicatorStatusDataPacket1;
}

CanDataPacket Led3::indicatorStatusDataPacket2() const
{
    return this->_indicatorStatusDataPacket2;
}

CanDataPacket Led3::indicatorStatusDataPacket3() const
{
    return this->_indicatorStatusDataPacket3;
}