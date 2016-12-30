#ifndef CANMESSAGE_H
#define CANMESSAGE_H

#include "candatapacket.h"

class CanMessage
{
public:
    CanMessage(uint32_t id, uint8_t frame, uint8_t length, CanDataPacket data);
    uint32_t id() const;
    uint8_t frame() const;
    uint8_t length() const;
    CanDataPacket data() const;

private:
    uint32_t _id;
    uint8_t _frame;
    uint8_t _length;
    CanDataPacket _data;
};

#endif // CANMESSAGE_H
