#include "include/canmessage.h"

CanMessage::CanMessage(uint32_t id, uint8_t frame, uint8_t len, CanDataPacket data)
{

}

uint32_t CanMessage::id() const
{
    return this->_id;
}

uint8_t CanMessage::frame() const
{
    return this->_frame;
}

uint8_t CanMessage::length() const
{
    return this->_length;
}

CanDataPacket CanMessage::data() const
{
    return this->_data;
}
