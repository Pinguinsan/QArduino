#ifndef pn75K308_H
#define pn75K308_H


#include <string>
#include <vector>
#include <iterator>

#include "u55xswitch.h"
#include "lightingitem.h"
#include "led1.h"
#include "led2.h"
#include "led3.h"
#include "led4.h"
#include "allindicators.h"
#include "daytimeillumination.h"
#include "nighttimeillumination.h"
#include "nolighting.h"
#include "combinedlightingitems.h"
#include "arduinomega.h"

#ifndef ON
    #define ON true
#endif

#ifndef OFF
    #define OFF false
#endif

class pn75K308 : public U55XSwitch
{
public:
    pn75K308();
    ~pn75K308();
    void setInspectionMode(bool inspectionMode);
    void changeLightingItemStatus(LightingSpecifier ls, bool onOrOff);
    void sendCanCommands(MCP_CAN *canController);
    bool canCommandStatusAt(LightingSpecifier ls) const;
    void turnAllLightingOff();
    bool inspectionMode() const;
    std::string partNumber() const;
    bool rearDiffLockToggleState() const;
    void buttonPressEvent(ButtonSpecifier bs);
    void turnAllLightingExceptIlluminationOff();

    unsigned char allButtonsReleased() const;
    unsigned char topLeftButtonPressed() const;
    unsigned char topRightButtonPressed() const;
    unsigned char bottomLeftButtonPressed() const;
    unsigned char bottomRightButtonPressed() const;

protected: 
    const std::string _partNumber;
    unsigned char _allButtonsReleased;
    unsigned char _topLeftButtonPressed;
    unsigned char _topRightButtonPressed;
    unsigned char _bottomLeftButtonPressed;
    unsigned char _bottomRightButtonPressed;
    bool _inspectionMode;
    bool _rearDiffLockToggleState;
    LightingItem *lightingItemAt(LightingSpecifier ls) const;
    std::vector<LightingItem *> _lightingItemVector;
    std::shared_ptr<ArduinoMega> _arduino;
    void sendLightingItemMessages(LightingItem *li, MCP_CAN *canController) const;
};

#endif //PN75K308_H
