#ifndef U55XSWITCH_H
#define U55XSWITCH_H

#include <string>
#include <vector>
#include "lightingitem.h"

const unsigned long STATUS_ID{0x62A};
const unsigned long INSPECTION_ID{0x628};
const unsigned long IG_ON_ID{0x3B3};
const unsigned long INDICATOR_STATUS_1_ID{0x263};
const unsigned long INDICATOR_STATUS_2_ID{0x44C};
const unsigned long INDICATOR_STATUS_3_ID{0x41E};
const unsigned long PUSH_BUTTON_ID{0x060};
const unsigned long STANDARD_CAN_TRANSMIT_DELAY{100};
const unsigned char STANDARD_FRAME{0};
const unsigned char STANDARD_MESSAGE_LENGTH{8};

enum class LightingSpecifier { LED1, LED2, LED3, LED4, ALL_INDICATORS, DAYTIME_ILLUMINATION, NIGHTTIME_ILLUMINATION, ALL_LIGHTING_OFF };
enum class PartNumber { pn75K308, pn75K309, pn75K313, pn75K314 };
enum SerialCommand { LED1, LED2, LED3, LED4, ALL_INDICATORS, DAYTIME_ILLUMINATION, NIGHTTIME_ILLUMINATION, ALL_LIGHTING_OFF };
enum class ButtonSpecifier { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

class U55XSwitch
{
public:
    virtual ~U55XSwitch() {} //Polymorphic destructor
    virtual void setInspectionMode(bool inspectionMode) = 0;
    virtual void changeLightingItemStatus(LightingSpecifier ls, bool onOrOff) = 0;
    virtual void sendCanCommands(MCP_CAN *canController) = 0;
    virtual bool canCommandStatusAt(LightingSpecifier ls) const = 0;
    virtual void turnAllLightingOff() = 0;
    virtual bool inspectionMode() const = 0;
    virtual std::string partNumber() const = 0;
    virtual void buttonPressEvent(ButtonSpecifier bs) = 0;
    virtual bool rearDiffLockToggleState() const = 0;

    virtual unsigned char allButtonsReleased() const = 0;
    virtual unsigned char topLeftButtonPressed() const = 0;
    virtual unsigned char topRightButtonPressed() const = 0;
    virtual unsigned char bottomLeftButtonPressed() const = 0;
    virtual unsigned char bottomRightButtonPressed() const = 0;

protected:
    virtual LightingItem * lightingItemAt(LightingSpecifier ls) const = 0;
    virtual void sendLightingItemMessages(LightingItem *li, MCP_CAN *canController) const = 0;
};

#endif //U55XSWITCH_H
