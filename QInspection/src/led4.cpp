#include "../include/led4.h"

/*
Led4::Led4() :
    _canCommandStatus{false},
    _igOnDataPacket (std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
    _indicatorStatusDataPacket1 (std::vector<unsigned char> {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket2 (std::vector<unsigned char> {0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket3 (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
{
    //Constructor
}
*/

Led4::Led4(CanDataPacket igOnDataPacket, 
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

Led4::~Led4()
{
    //Destructor
}

void Led4::setCanCommandStatus(bool onOrOff)
{
    this->_canCommandStatus = onOrOff;
}


bool Led4::canCommandStatus() const
{
    return this->_canCommandStatus;
}
    
CanDataPacket Led4::igOnDataPacket() const
{
    return this->_igOnDataPacket;
}

CanDataPacket Led4::indicatorStatusDataPacket1() const
{
    return this->_indicatorStatusDataPacket1;
}

CanDataPacket Led4::indicatorStatusDataPacket2() const
{
    return this->_indicatorStatusDataPacket2;
}

CanDataPacket Led4::indicatorStatusDataPacket3() const
{
    return this->_indicatorStatusDataPacket3;
}