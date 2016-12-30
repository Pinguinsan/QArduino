#ifndef ARDUINOINIT_H
#define ARDUINOINIT_H

#include <Arduino.h>

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

void arduinoInit()
{
    init();
    initVariant();
    #if defined(USBCON)
      USBDevice.attach();
    #endif
}

#endif //ARDUINOINIT_H

