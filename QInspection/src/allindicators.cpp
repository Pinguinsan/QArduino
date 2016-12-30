#include "../include/allindicators.h"

AllIndicators::AllIndicators(CanDataPacket igOnDataPacket, 
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

AllIndicators::~AllIndicators()
{
    //Destructor
}

void AllIndicators::setCanCommandStatus(bool onOrOff)
{
    this->_canCommandStatus = onOrOff;
}


bool AllIndicators::canCommandStatus() const
{
    return this->_canCommandStatus;
}

CanDataPacket AllIndicators::igOnDataPacket() const
{
    return this->_igOnDataPacket;
}

CanDataPacket AllIndicators::indicatorStatusDataPacket1() const
{
    return this->_indicatorStatusDataPacket1;
}

CanDataPacket AllIndicators::indicatorStatusDataPacket2() const
{
    return this->_indicatorStatusDataPacket2;
}

CanDataPacket AllIndicators::indicatorStatusDataPacket3() const
{
    return this->_indicatorStatusDataPacket3;
}
