#ifndef LED3_H
#define LED3_H

#include <string>
#include <vector>
#include "lightingitem.h"
#include "candatapacket.h"

class Led3 : public LightingItem
{
public:
    Led3(CanDataPacket igOnDataPacket, CanDataPacket indicatorStatusDataPacket1, CanDataPacket indicatorStatusDataPacket2, CanDataPacket indicatorStatusDataPacket3);
    ~Led3();
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

#endif //LED3_H
