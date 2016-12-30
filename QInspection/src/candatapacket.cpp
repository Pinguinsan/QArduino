#include "../include/candatapacket.h"

CanDataPacket::CanDataPacket(const std::vector<unsigned char> &packet) :
    _dataPacket(packet)
{

}

void CanDataPacket::toBasicArray(unsigned char copyArray[8]) const
{
    int i = 0;
    for (std::vector<unsigned char>::const_iterator iter = _dataPacket.begin(); iter != _dataPacket.end(); iter++) {
        copyArray[i] = *iter;
        i++;
    }
}

std::vector<unsigned char> CanDataPacket::dataPacket() const
{
    return _dataPacket;
}

CanDataPacket CanDataPacket::combineDataPackets(const CanDataPacket &first, const CanDataPacket &second)
{
    std::vector<unsigned char> constructorArg;
    std::vector<unsigned char> firstCopy = first.dataPacket();
    std::vector<unsigned char> secondCopy = second.dataPacket();
    std::vector<unsigned char>::const_iterator firstIter = firstCopy.begin();
    std::vector<unsigned char>::const_iterator secondIter = secondCopy.begin();
    for (; firstIter != firstCopy.end(); firstIter++) {
        constructorArg.push_back((*firstIter) | (*secondIter));
        secondIter++;
    }
    return CanDataPacket(constructorArg);
}
