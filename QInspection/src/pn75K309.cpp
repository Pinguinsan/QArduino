
#include "../include/pn75K309.h"

pn75K309::pn75K309() : 
    _partNumber{"75K309"},
    _allButtonsReleased{0xad},
    _topLeftButtonPressed{0x08},
    _topRightButtonPressed{0xa0},
    _bottomLeftButtonPressed{0x28},
    _bottomRightButtonPressed{0x68}, 
    _inspectionMode{false},
    _rearDiffLockToggleState{false}
{
    _lightingItemVector.push_back(new Led1((std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
                                           (std::vector<unsigned char> {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                           (std::vector<unsigned char> {0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                           (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})));

    _lightingItemVector.push_back(new Led2 ((std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
                                           (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}),
                                           (std::vector<unsigned char> {0x00, 0x04, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}),
                                           (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})));


    _lightingItemVector.push_back(new Led3((std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
                                           (std::vector<unsigned char> {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                           (std::vector<unsigned char> {0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                           (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})));

    _lightingItemVector.push_back(new Led4((std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
                                           (std::vector<unsigned char> {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                           (std::vector<unsigned char> {0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                           (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})));

    _lightingItemVector.push_back(new AllIndicators((std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
                                                            (std::vector<unsigned char> {0x02, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00}), 
                                                            (std::vector<unsigned char> {0x00, 0x04, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                                            (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})));

    _lightingItemVector.push_back(new NighttimeIllumination((std::vector<unsigned char> {0x40, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00}),
                                                            (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                                            (std::vector<unsigned char> {0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                                            (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})));                                                          

    _lightingItemVector.push_back(new DaytimeIllumination((std::vector<unsigned char> {0x40, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00}),
                                                          (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                                          (std::vector<unsigned char> {0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                                          (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})));
    
    _lightingItemVector.push_back(new NoLighting((std::vector<unsigned char> {0x40, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00}),
                                                 (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                                 (std::vector<unsigned char> {0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00}), 
                                                 (std::vector<unsigned char> {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})));
}

unsigned char pn75K309::allButtonsReleased() const
{
    return this->_allButtonsReleased;
}

unsigned char pn75K309::topLeftButtonPressed() const
{
    return this->_topLeftButtonPressed;
}

unsigned char pn75K309::topRightButtonPressed() const
{
    return this->_topRightButtonPressed;
}

unsigned char pn75K309::bottomLeftButtonPressed() const
{
    return this->_bottomLeftButtonPressed;
}

unsigned char pn75K309::bottomRightButtonPressed() const
{
    return this->_bottomRightButtonPressed;
}

void pn75K309::setInspectionMode(bool inspectionMode)
{
    this->_inspectionMode = inspectionMode;
    if (this->_inspectionMode) {
        turnAllLightingOff();
        changeLightingItemStatus(LightingSpecifier::LED1, ON);
    } 
}

bool pn75K309::inspectionMode() const 
{
    return this->_inspectionMode;
}

bool pn75K309::canCommandStatusAt(LightingSpecifier ls) const
{
    return this->lightingItemAt(ls)->canCommandStatus(); 
}

void pn75K309::changeLightingItemStatus(LightingSpecifier ls, bool onOrOff)
{
    if (ls != LightingSpecifier::ALL_LIGHTING_OFF) {
        this->lightingItemAt(LightingSpecifier::ALL_LIGHTING_OFF)->setCanCommandStatus(false);    
    }
    this->lightingItemAt(ls)->setCanCommandStatus(onOrOff); 
}

void pn75K309::buttonPressEvent(ButtonSpecifier bs)
{
    if (!this->_inspectionMode) {
        if (bs == ButtonSpecifier::TOP_LEFT) {
            if (!this->_rearDiffLockToggleState) {
                turnAllLightingExceptIlluminationOff();
                changeLightingItemStatus(LightingSpecifier::LED1, ON);
            }
        } else if (bs == ButtonSpecifier::TOP_RIGHT) {
            if (this->_rearDiffLockToggleState) {
                this->_rearDiffLockToggleState = false;
                this->_allButtonsReleased = 0xa0;
                this->_topLeftButtonPressed = 0x00;
                this->_topRightButtonPressed = 0xa8;
                this->_bottomLeftButtonPressed = 0x20;
                this->_bottomRightButtonPressed = 0x60;
                turnAllLightingExceptIlluminationOff();
                changeLightingItemStatus(LightingSpecifier::LED2, OFF);
                changeLightingItemStatus(LightingSpecifier::LED1, ON);
            } else {
                this->_rearDiffLockToggleState = true;
                this->_allButtonsReleased = 0xa8;
                this->_topLeftButtonPressed = 0x08;
                this->_topRightButtonPressed = 0xa0;
                this->_bottomLeftButtonPressed = 0x28;
                this->_bottomRightButtonPressed = 0x68;
                turnAllLightingExceptIlluminationOff();
                changeLightingItemStatus(LightingSpecifier::LED2, ON);                        
            }
        } else if (bs == ButtonSpecifier::BOTTOM_LEFT) {
            if (!this->_rearDiffLockToggleState) {
                turnAllLightingExceptIlluminationOff();
                changeLightingItemStatus(LightingSpecifier::LED3, ON);            
            }
        } else if (bs == ButtonSpecifier::BOTTOM_RIGHT) {
            if (!this->_rearDiffLockToggleState) {
                turnAllLightingExceptIlluminationOff();
                changeLightingItemStatus(LightingSpecifier::LED4, ON);
            }
        }     
    } else {
        if (bs == ButtonSpecifier::TOP_LEFT) {
            if (!this->_rearDiffLockToggleState) {
                turnAllLightingExceptIlluminationOff();
                changeLightingItemStatus(LightingSpecifier::LED1, ON);
            }
        } else if (bs == ButtonSpecifier::TOP_RIGHT) {
            if (this->_rearDiffLockToggleState) {
                this->_rearDiffLockToggleState = false;
                this->_allButtonsReleased = 0xa0;
                this->_topLeftButtonPressed = 0x00;
                this->_topRightButtonPressed = 0xa8;
                this->_bottomLeftButtonPressed = 0x20;
                this->_bottomRightButtonPressed = 0x60;
                turnAllLightingExceptIlluminationOff();
                changeLightingItemStatus(LightingSpecifier::LED2, OFF);
                changeLightingItemStatus(LightingSpecifier::LED1, ON);
            } else {
                this->_rearDiffLockToggleState = true;
                this->_allButtonsReleased = 0xa8;
                this->_topLeftButtonPressed = 0x08;
                this->_topRightButtonPressed = 0xa0;
                this->_bottomLeftButtonPressed = 0x28;
                this->_bottomRightButtonPressed = 0x68;
                turnAllLightingExceptIlluminationOff();
                changeLightingItemStatus(LightingSpecifier::LED2, ON);                        
            }
        } else if (bs == ButtonSpecifier::BOTTOM_LEFT) {
            if (!this->_rearDiffLockToggleState) {
                turnAllLightingExceptIlluminationOff();
                changeLightingItemStatus(LightingSpecifier::LED3, ON);            
            }
        } else if (bs == ButtonSpecifier::BOTTOM_RIGHT) {
            if (!this->_rearDiffLockToggleState) {
                turnAllLightingExceptIlluminationOff();
                changeLightingItemStatus(LightingSpecifier::LED4, ON);
            }
        }   
    }     
}

LightingItem *pn75K309::lightingItemAt(LightingSpecifier ls) const
{
    if (ls == LightingSpecifier::LED1) {
        return _lightingItemVector.at(0);
    } else if (ls == LightingSpecifier::LED2) {
        return _lightingItemVector.at(1);
    } else if (ls == LightingSpecifier::LED3) {
        return _lightingItemVector.at(2);
    } else if (ls == LightingSpecifier::LED4) {
        return _lightingItemVector.at(3);
    } else if (ls == LightingSpecifier::ALL_INDICATORS) {
        return _lightingItemVector.at(4);
    } else if (ls == LightingSpecifier::NIGHTTIME_ILLUMINATION) {
        return _lightingItemVector.at(5);
    } else if (ls == LightingSpecifier::DAYTIME_ILLUMINATION) {
        return _lightingItemVector.at(6);
    } else if (ls == LightingSpecifier::ALL_LIGHTING_OFF) {
        return _lightingItemVector.at(7);
    } else {
        return _lightingItemVector.at(7);
    }
}

void pn75K309::sendCanCommands(MCP_CAN *canController)
{
    CanDataPacket igOnDataPacket = CanDataPacket(std::vector<unsigned char>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    CanDataPacket indicatorStatusDataPacket1 = CanDataPacket(std::vector<unsigned char>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    CanDataPacket indicatorStatusDataPacket2 = CanDataPacket(std::vector<unsigned char>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    CanDataPacket indicatorStatusDataPacket3 = CanDataPacket(std::vector<unsigned char>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    
    for (std::vector<LightingItem *>::const_iterator iter = _lightingItemVector.begin(); iter != _lightingItemVector.end(); iter++) {
        if ((*iter)->canCommandStatus()) {
            igOnDataPacket = CanDataPacket::combineDataPackets(igOnDataPacket, (*iter)->igOnDataPacket());
            indicatorStatusDataPacket1 = CanDataPacket::combineDataPackets(indicatorStatusDataPacket1, (*iter)->indicatorStatusDataPacket1());
            indicatorStatusDataPacket2 = CanDataPacket::combineDataPackets(indicatorStatusDataPacket2, (*iter)->indicatorStatusDataPacket2());
            indicatorStatusDataPacket3 = CanDataPacket::combineDataPackets(indicatorStatusDataPacket3, (*iter)->indicatorStatusDataPacket3()); 
        }
    }
    LightingItem *li = new CombinedLightingItems(igOnDataPacket, indicatorStatusDataPacket1, indicatorStatusDataPacket2, indicatorStatusDataPacket3);
    sendLightingItemMessages(li, canController);
    delete li;
}

void pn75K309::sendLightingItemMessages(LightingItem *li, MCP_CAN *canController) const
{
    /*First declare empty arrays to hold the can commands*/
    unsigned char igOnDataPacket[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char indicatorStatusDataPacket1[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char indicatorStatusDataPacket2[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char indicatorStatusDataPacket3[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    /*Convert all of the arrays to the correct unsigned char array*/
    li->igOnDataPacket().toBasicArray(igOnDataPacket);
    li->indicatorStatusDataPacket1().toBasicArray(indicatorStatusDataPacket1);
    li->indicatorStatusDataPacket2().toBasicArray(indicatorStatusDataPacket2);
    li->indicatorStatusDataPacket3().toBasicArray(indicatorStatusDataPacket3);

    /*Send 'em all*/
    int returnVal;
    returnVal = canController->sendMsgBuf(IG_ON_ID, STANDARD_FRAME, STANDARD_MESSAGE_LENGTH, igOnDataPacket);
    returnVal = canController->sendMsgBuf(INDICATOR_STATUS_1_ID, STANDARD_FRAME, STANDARD_MESSAGE_LENGTH, indicatorStatusDataPacket1);
    returnVal = canController->sendMsgBuf(INDICATOR_STATUS_2_ID, STANDARD_FRAME, STANDARD_MESSAGE_LENGTH, indicatorStatusDataPacket2);
    returnVal = canController->sendMsgBuf(INDICATOR_STATUS_3_ID, STANDARD_FRAME, STANDARD_MESSAGE_LENGTH, indicatorStatusDataPacket3);
    (void)returnVal;       
}

void pn75K309::turnAllLightingOff()
{
    for (std::vector<LightingItem *>::const_iterator iter = _lightingItemVector.begin(); iter != _lightingItemVector.end(); iter++) {
        if (iter+1 == _lightingItemVector.end()) {
            (*iter)->setCanCommandStatus(true);
        } else {
            (*iter)->setCanCommandStatus(false);
        }
    }
}

void pn75K309::turnAllLightingExceptIlluminationOff()
{
    for (std::vector<LightingItem *>::const_iterator iter = _lightingItemVector.begin(); iter != _lightingItemVector.end(); iter++) {
        if (*iter == _lightingItemVector.at(5)) {
            (*iter)->setCanCommandStatus(false);
        } else {
            (*iter)->setCanCommandStatus(false);
        }
    }
}

bool pn75K309::rearDiffLockToggleState() const
{
    return this->_rearDiffLockToggleState;
}

std::string pn75K309::partNumber() const
{
    return this->_partNumber;
}

pn75K309::~pn75K309()
{
    for (std::vector<LightingItem *>::iterator iter = _lightingItemVector.begin(); iter != _lightingItemVector.end(); iter++) {
        delete *iter;
    }
}