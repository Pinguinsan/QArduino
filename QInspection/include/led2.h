#ifndef LED2_H
#define LED2_H

#include <string>
#include <vector>
#include "lightingitem.h"
#include "candatapacket.h"

class Led2 : public LightingItem
{
public:
    Led2(CanDataPacket igOnDataPacket, CanDataPacket indicatorStatusDataPacket1, CanDataPacket indicatorStatusDataPacket2, CanDataPacket indicatorStatusDataPacket3);
    ~Led2();
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

#endif //LED2_H
