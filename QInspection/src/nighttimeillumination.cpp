#include "../include/nighttimeillumination.h"

/*
NighttimeIllumination::NighttimeIllumination() :
    _canCommandStatus{false},
    _igOnDataPacket (std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
    _indicatorStatusDataPacket1 (std::vector<unsigned char> {0x02, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket2 (std::vector<unsigned char> {0x00, 0x04, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
    _indicatorStatusDataPacket3 (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
{
    //Constructor
}
*/

NighttimeIllumination::NighttimeIllumination(CanDataPacket igOnDataPacket, 
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

NighttimeIllumination::~NighttimeIllumination()
{
    //Destructor
}

void NighttimeIllumination::setCanCommandStatus(bool onOrOff)
{
    this->_canCommandStatus = onOrOff;
}


bool NighttimeIllumination::canCommandStatus() const
{
    return this->_canCommandStatus;
}

CanDataPacket NighttimeIllumination::igOnDataPacket() const
{
    return this->_igOnDataPacket;
}

CanDataPacket NighttimeIllumination::indicatorStatusDataPacket1() const
{
    return this->_indicatorStatusDataPacket1;
}

CanDataPacket NighttimeIllumination::indicatorStatusDataPacket2() const
{
    return this->_indicatorStatusDataPacket2;
}

CanDataPacket NighttimeIllumination::indicatorStatusDataPacket3() const
{
    return this->_indicatorStatusDataPacket3;
}
