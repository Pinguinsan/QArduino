#ifndef ARDUINOPC_CONFIGURATIONFILEREADER_H
#define ARDUINOPC_CONFIGURATIONFILEREADER_H

#include <QFile>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <map>

#include <arduino.h>
#include <fileutilities.h>
#include <generalutilities.h>

#include "arduinopcstrings.h"



class ConfigurationFileReader
{
public:
    ConfigurationFileReader();
    std::map<int, std::string> unoAliasesMap() const;
    std::map<int, std::string> unoIOTypesMap() const;
    std::map<int, std::string> unoInitialStatesMap() const;

    std::map<int, std::string> nanoAliasesMap() const;
    std::map<int, std::string> nanoIOTypesMap() const;
    std::map<int, std::string> nanoInitialStatesMap() const;

    std::map<int, std::string> megaAliasesMap() const;
    std::map<int, std::string> megaIOTypesMap() const;
    std::map<int, std::string> megaInitialStatesMap() const;
    double bluetoothMultiplier() const;

private:
    std::map<int, std::string> m_unoAliasesMap;
    std::map<int, std::string> m_unoIOTypesMap;
    std::map<int, std::string> m_unoInitialStatesMap;
    std::map<int, std::string> m_nanoAliasesMap;
    std::map<int, std::string> m_nanoIOTypesMap;
    std::map<int, std::string> m_nanoInitialStatesMap;
    std::map<int, std::string> m_megaAliasesMap;
    std::map<int, std::string> m_megaIOTypesMap;
    std::map<int, std::string> m_megaInitialStatesMap;
    double m_bluetoothMutiplier;
};

#endif //ARDUINOPC_CONFIGURATIONFILEREADER_H
