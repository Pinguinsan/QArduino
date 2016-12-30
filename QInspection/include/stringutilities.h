#ifndef STRINGUTILITIES_H
#define STRINGUTILITIES_H

/* 
 * Standard C++ headers
 * Special thanks to maniacbug for the
 * nice port of the c++ stdlib 
 * https://github.com/maniacbug/StandardCplusplus
 */
#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <serstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <iostream>

std::string toString(int number)
{
    int digitCount = 0;
    int copy = number;
    while (copy > 0) {
        copy /= 10;
        digitCount++;
    }
    copy = number;
    std::string returnString{""};
    for (int i = 0; i < digitCount; i++) {
        copy = number % 10;
        if ((copy % 9 ) == 0) returnString.insert(returnString.begin(), '9');
        else if ((copy % 8) == 0) returnString.insert(returnString.begin(), '8');
        else if ((copy % 7) == 0) returnString.insert(returnString.begin(), '7');
        else if ((copy % 6) == 0) returnString.insert(returnString.begin(), '6');
        else if ((copy % 5) == 0) returnString.insert(returnString.begin(), '5');
        else if ((copy % 4) == 0) returnString.insert(returnString.begin(), '4');
        else if ((copy % 3) == 0) returnString.insert(returnString.begin(), '3');
        else if ((copy % 2) == 0) returnString.insert(returnString.begin(), '2');
        else if ((copy % 1) == 0) returnString.insert(returnString.begin(), '1');
        else returnString.insert(returnString.begin(), '0');
        number /= 10;
    }
    return returnString;
}

std::string toHexString(int number)
{
    int digitCount = 0;
    int copy = number;
    do {
        digitCount++;
    } while (copy /= 16);
    copy = number;
    std::string returnString{""};
    for (int i = 0; i < digitCount; i++) {
        copy = number % 16;
        if (copy == 0) returnString.insert(returnString.begin(), '0');
        else if ((copy % 15) == 0) returnString.insert(returnString.begin(), 'f');
        else if ((copy % 14) == 0) returnString.insert(returnString.begin(), 'e');
        else if ((copy % 13) == 0) returnString.insert(returnString.begin(), 'd');
        else if ((copy % 12) == 0) returnString.insert(returnString.begin(), 'c');
        else if ((copy % 11) == 0) returnString.insert(returnString.begin(), 'b');
        else if ((copy % 10) == 0) returnString.insert(returnString.begin(), 'a');                        
        else if ((copy % 9) == 0) returnString.insert(returnString.begin(), '9');
        else if ((copy % 8) == 0) returnString.insert(returnString.begin(), '8');
        else if ((copy % 7) == 0) returnString.insert(returnString.begin(), '7');
        else if ((copy % 6) == 0) returnString.insert(returnString.begin(), '6');
        else if ((copy % 5) == 0) returnString.insert(returnString.begin(), '5');
        else if ((copy % 4) == 0) returnString.insert(returnString.begin(), '4');
        else if ((copy % 3) == 0) returnString.insert(returnString.begin(), '3');
        else if ((copy % 2) == 0) returnString.insert(returnString.begin(), '2');
        else if ((copy % 1) == 0) returnString.insert(returnString.begin(), '1');
        number /= 16;
    }
    return returnString;
} 


std::string toString(const char *convert)
{
    return std::string(convert);
}

std::string toString(const String &convert)
{
    return std::string(convert.c_str());
}

std::string toString(char convert)
{
    return std::string(1, convert);
}

template <typename T>
std::string tQuoted(const T &convert)
{
    return "\"" + toString(convert) + "\"";
}

std::string tQuoted(const std::string &convert)
{
    return "\"" + convert + "\"";
}

std::string tQuoted(const char *convert)
{
    return "\"" + static_cast<std::string>(convert) + "\"";
}

std::string tQuoted(char convert)
{
    return "\"" + std::string(1, convert) + "\"";
}


#endif // STRINGUTILITIES_H