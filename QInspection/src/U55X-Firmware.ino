/* 
 * Standard C++ headers
 * Special thanks to maniacbug Mfor the
 * nice port of the c++ stdlib 
 * https://github.com/maniacbug/StandardCplusplus
 */
#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <vector>
#include <sstream>
#include <iterator>

#define BTSerial Serial1

/*<iostream> declares cout/cerr, but the application must define them*/
namespace std { ohserialstream cout{Serial}; }
namespace std { ohserialstream btcout{BTSerial}; }

/*Arduino standard library*/
#include <Arduino.h>

/*Serial & CAN shield libraries*/
#include <SPI.h>
#include "mcp_can.h"
#include "mcp_can_dfs.h"
#include "include/arduinoinit.h"

#include "include/u55xswitch.h"
#include "include/pn75K308.h"
#include "include/pn75K309.h"
#include "include/pn75K313.h"
#include "include/pn75K314.h"
#include "include/stringutilities.h"

#ifndef INT32U
#define INT32U unsigned long
#endif

#ifndef INT8U
#define INT8U byte
#endif

const int SERIAL_TIMEOUT = 100;
const int SERIAL_BAUD = 9600;
const int STANDARD_MESSAGE_WIDTH = 2;
const int ID_WIDTH = 3;
#ifndef ON
    #defined ON true
#endif

#ifndef OFF
    #defined OFF false
#endif
const int MILLISECONDS_PER_SECOND = 1000;

int knownOffsetCommandsLength;
void canInit();
void serialInit(long long baudRate);
void logString(const std::string &outputString);

const std::vector<int> KNOWN_OFFSET_COMMANDS {static_cast<int>('z'), static_cast<int>('x'), static_cast<int>('c'), static_cast<int>('v'), static_cast<int>('b')};
void updateCanStatus(U55XSwitch *u55xSwitch, int serialCommand);
void sendCurrentCanStatus(U55XSwitch *u55xSwitch, MCP_CAN *canController);
void receiveCanMessages(U55XSwitch *u55xSwitch);
String toFixedWidth(const String &inputString, int fixedWidth);
std::string toFixedWidth(const std::string &inputString, int fixedWidth);
void printCanMessage(unsigned char canID, unsigned char receivedDataPacket[8], int receivedPacketLength);
bool partNumberChangeRequest(int potentialCommand);
void attemptPartNumberChangeRequest(int serialCommand, PartNumber &currentPartNumber, U55XSwitch *partNumber);

// the cs pin of the version after v1.1 is default to D9 v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
MCP_CAN *canController;

int main(void)
{
    long long int startTime = 0;
    long long int endTime = 0;
    arduinoInit();
    canInit();
    serialInit(SERIAL_BAUD);
    U55XSwitch *u55xSwitch = new pn75K308();  
    u55xSwitch->setInspectionMode(true);  
    int serialCommand = 0;
    int btSerialCommand = 0;
    PartNumber currentPartNumber = PartNumber::pn75K308;

    startTime = millis();
    endTime = millis();
    while (!false) {
        Serial.setTimeout(SERIAL_TIMEOUT);
        BTSerial.setTimeout(SERIAL_TIMEOUT);
        serialCommand = static_cast<int>(Serial.read());
        btSerialCommand = static_cast<int>(BTSerial.read());
        if (serialCommand != -1) {
            if (partNumberChangeRequest(serialCommand)) {
                attemptPartNumberChangeRequest(serialCommand, currentPartNumber, u55xSwitch);
            } else {
                serialCommand -= 48;
                updateCanStatus(u55xSwitch, serialCommand);
            }
        }
        if (btSerialCommand != -1) {
            if (partNumberChangeRequest(btSerialCommand)) {
                attemptPartNumberChangeRequest(btSerialCommand, currentPartNumber, u55xSwitch);
            } else {
                btSerialCommand -= 48;
                updateCanStatus(u55xSwitch, btSerialCommand);
            }
        }        
        /*
        endTime = millis();
        if ((endTime - startTime) >= STANDARD_CAN_TRANSMIT_DELAY) {
            sendCurrentCanStatus(u55xSwitch, canController);
            startTime = millis();
        }
        */
        receiveCanMessages(u55xSwitch);
        sendCurrentCanStatus(u55xSwitch, canController);
    
        if (serialEventRun) serialEventRun();
    }
    delete canController;
    delete u55xSwitch;
    return 0;
}

void sendCurrentCanStatus(U55XSwitch *u55xSwitch, MCP_CAN *canController)
{
    u55xSwitch->sendCanCommands(canController);
}

void updateCanStatus(U55XSwitch *u55xSwitch, int serialCommand)
{
    if (serialCommand == SerialCommand::LED1) {
        if (!u55xSwitch->canCommandStatusAt(LightingSpecifier::LED1)) {
            logString("Command received to turn LED1 on");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::LED1, ON); 
        } else {
            logString("Command received to turn LED1 off");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::LED1, OFF); 
        }
    } else if (serialCommand == SerialCommand::LED2) {
        if (!u55xSwitch->canCommandStatusAt(LightingSpecifier::LED2)) {
            logString("Command received to turn LED2 on");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::LED2, ON); 
        } else {
            logString("Command received to turn LED2 off");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::LED2, OFF); 
        }
    } else if (serialCommand == SerialCommand::LED3) {
        if (!u55xSwitch->canCommandStatusAt(LightingSpecifier::LED3)) {
            logString("Command received to turn LED3 on");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::LED3, ON); 
        } else {
            logString("Command received to turn LED3 off");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::LED3, OFF); 
        }
    } else if (serialCommand == SerialCommand::LED4) {
        if (!u55xSwitch->canCommandStatusAt(LightingSpecifier::LED4)) {
            logString("Command received to turn LED4 on");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::LED4, ON); 
        } else {
            logString("Command received to turn LED4 off");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::LED4, OFF); 
        }
    } else if (serialCommand == SerialCommand::ALL_INDICATORS) {
        if (!u55xSwitch->canCommandStatusAt(LightingSpecifier::ALL_INDICATORS)) {
            logString("Command received to turn all indicators on");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::ALL_INDICATORS, ON); 
        } else {
            logString("Command received to turn all indicators off");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::ALL_INDICATORS, OFF); 
        }
    } else if (serialCommand == SerialCommand::DAYTIME_ILLUMINATION) {
        if (!u55xSwitch->canCommandStatusAt(LightingSpecifier::DAYTIME_ILLUMINATION)) {
            logString("Command received to turn daytime illumination on");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::DAYTIME_ILLUMINATION, ON); 
        } else {
            logString("Command received to turn daytime illumination off");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::DAYTIME_ILLUMINATION, OFF); 
        }
    } else if (serialCommand == SerialCommand::NIGHTTIME_ILLUMINATION) {
        if (!u55xSwitch->canCommandStatusAt(LightingSpecifier::NIGHTTIME_ILLUMINATION)) {
            logString("Command received to turn nighttime illumination on");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::NIGHTTIME_ILLUMINATION, ON); 
        } else {
            logString("Command received to turn nighttime illumination off");
            u55xSwitch->changeLightingItemStatus(LightingSpecifier::NIGHTTIME_ILLUMINATION, OFF); 
        }
    } else if (serialCommand == SerialCommand::ALL_LIGHTING_OFF) {
        logString("Command received to turn all lighting (indicators and illumination) off");
        u55xSwitch->turnAllLightingOff();         
    } else {
        logString("Invalid command " + tQuoted(toString(serialCommand)) + " received");
    }
}

void attemptPartNumberChangeRequest(int serialCommand, PartNumber &currentPartNumber, U55XSwitch *u55xSwitch)
{
    if (serialCommand == static_cast<int>('z')) {
        if (currentPartNumber != PartNumber::pn75K308) {
            u55xSwitch->turnAllLightingOff();
            delete u55xSwitch;
            u55xSwitch = new pn75K308();
            currentPartNumber = PartNumber::pn75K308;
            logString("Successfully changed part number to " + u55xSwitch->partNumber());
        } else {
            logString("Command received to change part number to " + u55xSwitch->partNumber() + ", but it is already current part number");
        }
    } else if (serialCommand == static_cast<int>('x')) {
        if (currentPartNumber != PartNumber::pn75K309) {
            u55xSwitch->turnAllLightingOff();
            delete u55xSwitch;
            u55xSwitch = new pn75K309();
            currentPartNumber = PartNumber::pn75K309;
            logString("Successfully changed part number to " + u55xSwitch->partNumber());
        } else {
            logString("Command received to change part number to " + u55xSwitch->partNumber() + ", but it is already current part number");
        }
    } else if (serialCommand == static_cast<int>('c')) {
        if (currentPartNumber != PartNumber::pn75K313) {
            u55xSwitch->turnAllLightingOff();
            delete u55xSwitch;
            u55xSwitch = new pn75K313();
            currentPartNumber = PartNumber::pn75K313;
            logString("Successfully changed part number to " + u55xSwitch->partNumber());
        } else {
            logString("Command received to change part number to " + u55xSwitch->partNumber() + ", but it is already current part number");
        }
    } else if (serialCommand == static_cast<int>('v')) {
        if (currentPartNumber != PartNumber::pn75K314) {
            u55xSwitch->turnAllLightingOff();
            delete u55xSwitch;
            u55xSwitch = new pn75K314();
            currentPartNumber = PartNumber::pn75K314;
            logString("Successfully changed part number to " + u55xSwitch->partNumber());
        } else {
            logString("Command received to change part number to " + u55xSwitch->partNumber() + ", but it is already current part number");
        }
    } else {
        logString("Invalid command " + tQuoted(static_cast<char>(serialCommand)) + " received for part number change request");
    } 
}

void receiveCanMessages(U55XSwitch *u55xSwitch)
{
    unsigned char receivedPacketLength = 0;
    unsigned char receivedDataPacket[8];
    unsigned char canID = 0;    
    
    if (CAN_MSGAVAIL == canController->checkReceive()) {
        canController->readMsgBuf(&receivedPacketLength, receivedDataPacket);
        canID = canController->getCanId();
        printCanMessage(canID, receivedDataPacket, receivedPacketLength);
    }
    
    if (canID == PUSH_BUTTON_ID) {
      /*
        if ((receivedDataPacket[0] == u55xSwitch->topLeftButtonPressed()) || (receivedDataPacket[0]-0x10 == u55xSwitch->topLeftButtonPressed())) {
            logString("Top left button pressed");
            u55xSwitch->buttonPressEvent(ButtonSpecifier::TOP_LEFT);
        } else if ((receivedDataPacket[0] == u55xSwitch->topRightButtonPressed()) || (receivedDataPacket[0]-0x10 == u55xSwitch->topRightButtonPressed())){
            logString("Top right button pressed");
            u55xSwitch->buttonPressEvent(ButtonSpecifier::TOP_RIGHT);  
        } else if ((receivedDataPacket[0] == u55xSwitch->bottomLeftButtonPressed()) || (receivedDataPacket[0]-0x10 == u55xSwitch->bottomLeftButtonPressed())) {
            logString("Bottom left button pressed");
            u55xSwitch->buttonPressEvent(ButtonSpecifier::BOTTOM_LEFT);
        } else if ((receivedDataPacket[0] == u55xSwitch->bottomRightButtonPressed()) || (receivedDataPacket[0]-0x10 == u55xSwitch->bottomRightButtonPressed())) {
            logString("Bottom right button pressed");
            u55xSwitch->buttonPressEvent(ButtonSpecifier::BOTTOM_RIGHT);       
        }
        */
    }
}

void printCanMessage(unsigned char canID, unsigned char receivedDataPacket[8], int receivedPacketLength)
{
    std::string serialString = "Receiving {0x" + toFixedWidth(toHexString(canID), ID_WIDTH) + "}: {";
    for (int i = 0; i < receivedPacketLength; i++) {
        serialString += " 0x" + toFixedWidth(toHexString(receivedDataPacket[i]), STANDARD_MESSAGE_WIDTH);         
    }
    serialString += " }";
    logString(serialString);      
}

bool partNumberChangeRequest(int potentialCommand)
{
    for (std::vector<int>::const_iterator iter = KNOWN_OFFSET_COMMANDS.begin(); iter != KNOWN_OFFSET_COMMANDS.end(); iter++) {
        if (potentialCommand == *iter) {
            return true;
        }
    }
    return false;
}

bool offsetCommandReceived(int offsetCommand, int &offsetPtr)
{
    for (int i = 0; i < knownOffsetCommandsLength; i++) {
        if (offsetCommand == KNOWN_OFFSET_COMMANDS[i]) {
            offsetPtr = i;
            return true;
        }
    }
    return false;
}

String toFixedWidth(const String &inputString, int fixedWidth)
{
    String copyString = inputString;
    int copyLength = copyString.length();
    if (copyLength < fixedWidth) {
        copyLength = fixedWidth - copyLength;
        while (copyLength--) {
            copyString = "0" + copyString;
        }
    }
    return copyString;
}

std::string toFixedWidth(const std::string &inputString, int fixedWidth)
{
    std::string copyString = inputString;
    int copyLength = copyString.length();
    if (copyLength < fixedWidth) {
        copyLength = (fixedWidth - copyLength);
        while (copyLength--) {
            copyString = "0" + copyString;
        }
    }
    return copyString;
}

void canInit()
{
    canController = new MCP_CAN(SPI_CS_PIN);
    unsigned int startTime = millis();
    unsigned int endTime;
    while (canController->begin(CAN_500KBPS) != CAN_OK) {
        endTime = millis();
        if ((endTime - startTime) >= MILLISECONDS_PER_SECOND) {
            logString("CAN BUS Shield initialization fail");
            logString("Attempting initialization again...");
            startTime = millis();
        }
    }
    logString("CAN BUS Shield initialization successful!");
}

void serialInit(long long baudRate)
{
    Serial.begin(baudRate);
    BTSerial.begin(baudRate);
}

void logString(const std::string &outputString)
{
    std::cout << outputString << std::endl;
    std::btcout << outputString << std::endl;
}

