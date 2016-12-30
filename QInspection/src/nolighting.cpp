#include "../include/nolighting.h"

/*
NoLighting::NoLighting() :
    _canCommandStatus{false},
    _igOnDataPacket (std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
    _indicatorStatusDataPacket1 (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket2 (std::vector<unsigned char> {0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket3 (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
{
    //Constructor
}
*/

NoLighting::NoLighting(CanDataPacket igOnDataPacket, 
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

NoLighting::~NoLighting()
{
    //Destructor
}

void NoLighting::setCanCommandStatus(bool onOrOff)
{
    this->_canCommandStatus = onOrOff;
}

bool NoLighting::canCommandStatus() const
{
    return this->_canCommandStatus;
}
    
CanDataPacket NoLighting::igOnDataPacket() const
{
    return this->_igOnDataPacket;
}

CanDataPacket NoLighting::indicatorStatusDataPacket1() const
{
    return this->_indicatorStatusDataPacket1;
}

CanDataPacket NoLighting::indicatorStatusDataPacket2() const
{
    return this->_indicatorStatusDataPacket2;
}

CanDataPacket NoLighting::indicatorStatusDataPacket3() const
{
    return this->_indicatorStatusDataPacket3;
}