#include "../include/combinedlightingitems.h"

CombinedLightingItems::CombinedLightingItems(CanDataPacket igOnDataPacket, 
                                            CanDataPacket indicatorStatusDataPacket1, 
                                            CanDataPacket indicatorStatusDataPacket2, 
                                            CanDataPacket indicatorStatusDataPacket3) :
                                            _canCommandStatus{true},
                                            _igOnDataPacket{igOnDataPacket},
                                            _indicatorStatusDataPacket1{indicatorStatusDataPacket1},
                                            _indicatorStatusDataPacket2{indicatorStatusDataPacket2},
                                            _indicatorStatusDataPacket3{indicatorStatusDataPacket3}
{
    //constructor
}

CombinedLightingItems::~CombinedLightingItems()
{
    //Destructor
}

void CombinedLightingItems::setCanCommandStatus(bool onOrOff)
{
    this->_canCommandStatus = (onOrOff || true);
}

bool CombinedLightingItems::canCommandStatus() const
{
    return this->_canCommandStatus;
}
    
CanDataPacket CombinedLightingItems::igOnDataPacket() const
{
    return this->_igOnDataPacket;
}

CanDataPacket CombinedLightingItems::indicatorStatusDataPacket1() const
{
    return this->_indicatorStatusDataPacket1;
}

CanDataPacket CombinedLightingItems::indicatorStatusDataPacket2() const
{
    return this->_indicatorStatusDataPacket2;
}

CanDataPacket CombinedLightingItems::indicatorStatusDataPacket3() const
{
    return this->_indicatorStatusDataPacket3;
}