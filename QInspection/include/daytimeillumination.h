#ifndef DAYTIMEILLUMINATION_H
#define DAYTIMEILLUMINATION_H

#include <string>
#include <vector>
#include <iterator>
#include "lightingitem.h"
#include "candatapacket.h"

class DaytimeIllumination : public LightingItem
{
public:
    DaytimeIllumination(CanDataPacket igOnDataPacket, CanDataPacket indicatorStatusDataPacket1, CanDataPacket indicatorStatusDataPacket2, CanDataPacket indicatorStatusDataPacket3);
    ~DaytimeIllumination();
    void setCanCommandStatus(bool onOrOff);
    bool canCommandStatus() const;
    CanDataPacket igOnDataPacket() const;
    CanDataPacket indicatorStatusDataPacket1() const;
    CanDataPacket indicatorStatusDataPacket2() const;
    CanDataPacket indicatorStatusDataPacket3() const;

private:
    bool _canCommandStatus;
    CanDataPacket _igOnDataPacket;
    CanDataPacket _indicatorStatusDataPacket1;
    CanDataPacket _indicatorStatusDataPacket2;
    CanDataPacket _indicatorStatusDataPacket3;
};

#endif //DAYTIMEILLUMINATION_H
