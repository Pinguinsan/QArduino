#ifndef CANDATAPACKET_H
#define CANDATAPACKET_H

#include <string>
#include <vector>
#include <iostream>


class CanDataPacket
{
public:
    CanDataPacket(const std::vector<unsigned char> &packet);
    void toBasicArray(unsigned char copyArray[8]) const;
    std::vector<unsigned char> dataPacket() const;
    static CanDataPacket combineDataPackets(const CanDataPacket &first, const CanDataPacket &second);

private:
    std::vector<unsigned char> _dataPacket;
};

#endif //CANDATAPACKET_H
