#include "arduinopcicons.h"

ArduinoPCIcons::ArduinoPCIcons() :
    MAIN_WINDOW_ICON{QIcon{":/icons/icons/arduinopc.png"}},
    DIO_HIGH_STATE_ICON{QIcon{":/icons/icons/high.png"}},
    DIO_LOW_STATE_ICON{QIcon{":/icons/icons/low.png"}}
{
    //Constructor
}

ArduinoPCIcons::~ArduinoPCIcons()
{
    //Destructor
}
