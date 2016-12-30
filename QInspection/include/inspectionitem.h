#ifndef INSPECTIONITEM_H
#define INSPECTIONITEM_H

#include <string>
#include <vector>
#include "candatapacket.h"
#include "u55xswitch.h"

class InspectionItem
{
public:
    virtual ~InspectionItem() { }; //Polymorphic destructor
    virtual CanDataPacket igOnDataPacket() const = 0;
};

#endif //INSPECTIONITEM_H
