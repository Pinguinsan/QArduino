#ifndef LIGHTINGITEM_H
#define LIGHTINGITEM_H


#include "candatapacket.h"

class LightingItem
{
public:
    virtual ~LightingItem() { } //Polymorphic destructor
    virtual void setCanCommandStatus(bool onOrOff) = 0;
    virtual bool canCommandStatus() const = 0;
    virtual CanDataPacket igOnDataPacket() const = 0;
    virtual CanDataPacket indicatorStatusDataPacket1() const = 0;
    virtual CanDataPacket indicatorStatusDataPacket2() const = 0;
    virtual CanDataPacket indicatorStatusDataPacket3() const = 0;
};

#endif //LIGHTINGITEM_H
