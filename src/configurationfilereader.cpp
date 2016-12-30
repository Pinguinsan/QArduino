#include "configurationfilereader.h"

ConfigurationFileReader::ConfigurationFileReader() :
    m_bluetoothMutiplier{-1}
{
    using namespace FileUtilities;
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    if (!QFile(DEFAULT_CONFIGURATION_FILE).exists() &&
        !QFile(BACKUP_CONFIGURATION_FILE).exists() &&
        !QFile(LAST_CHANCE_CONFIGURATION_FILE).exists())  {
        std::cout << NO_CONFIGURATION_FILE_WARNING_STRING << std::endl;
        return;
    }
    std::vector<std::pair<int, std::string>> buffer;
    std::ifstream readFromFile;
    readFromFile.open(DEFAULT_CONFIGURATION_FILE);
    if (readFromFile.is_open()) {
        std::string tempString{""};
        int lineNumber{0};
        while (std::getline(readFromFile, tempString)) {
            buffer.emplace_back(lineNumber++, tempString);
        }
        readFromFile.close();
    } else {
        if (fileExists(DEFAULT_CONFIGURATION_FILE)) {
            std::cout << UNABLE_TO_OPEN_CONFIGURATION_FILE_STRING_BASE <<
                         tQuoted(DEFAULT_CONFIGURATION_FILE) <<
                         UNABLE_TO_OPEN_CONFIGURATION_FILE_STRING_TAIL <<
                         TRYING_BACKUP_FILE_STRING <<
                         tQuoted(BACKUP_CONFIGURATION_FILE) << std::endl;
        }
        readFromFile.close();
        readFromFile.open(BACKUP_CONFIGURATION_FILE);
        if (readFromFile.is_open()) {
            std::string tempString{""};
            int lineNumber{0};
            while (std::getline(readFromFile, tempString)) {
                buffer.emplace_back(lineNumber++, tempString);
            }
            readFromFile.close();
        } else {
            if (fileExists(BACKUP_CONFIGURATION_FILE)) {
                std::cout << UNABLE_TO_OPEN_CONFIGURATION_FILE_STRING_BASE <<
                             tQuoted(BACKUP_CONFIGURATION_FILE) <<
                             UNABLE_TO_OPEN_CONFIGURATION_FILE_STRING_TAIL <<
                             TRYING_BACKUP_FILE_STRING <<
                             tQuoted(LAST_CHANCE_CONFIGURATION_FILE) << std::endl;
            }
            readFromFile.close();
            readFromFile.open(LAST_CHANCE_CONFIGURATION_FILE);
            if (readFromFile.is_open()) {
                std::string tempString{""};
                int lineNumber{0};
                while (std::getline(readFromFile, tempString)) {
                    buffer.emplace_back(lineNumber++, trimWhitespace(tempString));
                }
                readFromFile.close();
            } else {
                if (fileExists(LAST_CHANCE_CONFIGURATION_FILE))   {
                    std::cout << UNABLE_TO_OPEN_CONFIGURATION_FILE_STRING_BASE <<
                                 tQuoted(LAST_CHANCE_CONFIGURATION_FILE) <<
                                 UNABLE_TO_OPEN_CONFIGURATION_FILE_STRING_TAIL <<
                                 FALL_BACK_ON_DEFAULTS_STRING << std::endl;
                }
                return;
            }
        }
    }
    std::vector<std::pair<int, std::string>> unoBuffer;
    std::vector<std::pair<int, std::string>> nanoBuffer;
    std::vector<std::pair<int, std::string>> megaBuffer;
    //buffer = trimWhitespace(buffer);
    bool uno{false};
    for (auto &it : buffer) {
        if ((it.second.find(ArduinoUno::IDENTIFIER) != std::string::npos) && (it.second.find("{") != std::string::npos)) {
            uno = true;
            continue;
        }
        if (it.second.find("}") != std::string::npos) {
            uno = false;
            continue;
        }
        if (uno) {
            unoBuffer.emplace_back(it);
        }
    }

    bool nano{false};
    for (auto &it : buffer) {
        if ((it.second.find(ArduinoNano::IDENTIFIER) != std::string::npos) && (it.second.find("{") != std::string::npos)) {
            nano = true;
            continue;
        }
        if (it.second.find("}") != std::string::npos) {
            nano = false;
            continue;
        }
        if (nano) {
            nanoBuffer.emplace_back(it);
        }
    }

    bool mega{false};
    for (auto &it : buffer) {
        if ((it.second.find(ArduinoMega::IDENTIFIER) != std::string::npos) && (it.second.find("{") != std::string::npos)) {
            mega = true;
            continue;
        }
        if (it.second.find("}") != std::string::npos) {
            mega = false;
            continue;
        }
        if (mega) {
            megaBuffer.emplace_back(it);
        }
    }
    for (int i = 0; i < 3; i++) {
        buffer.clear();
        if (i == 0) {
            uno = true;
            nano = false;
            buffer = unoBuffer;
        } else if (i == 1) {
            uno = false;
            nano = true;
            buffer = nanoBuffer;
        } else {
            uno = false;
            nano = false;
            buffer = megaBuffer;
        }
        for (auto &it : buffer) {
            try {
                std::string copyString{it.second};
                std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
                //TODO: Replace with regex for searching
                copyString = trimWhitespace(copyString);
                size_t foundAliasPosition{copyString.find(ALIAS_IDENTIFIER)};
                size_t foundIOTypePosition{copyString.find(IO_TYPE_IDENTIFIER)};
                size_t foundStatePosition{copyString.find(INITIAL_STATE_IDENTIFIER)};
                size_t foundBluetoothMultiplierPosition{copyString.find(BLUETOOTH_MULTIPLIER_IDENTIFIER)};
                if (copyString.length() != 0) {
                    std::string otherCopy{trimWhitespace(copyString)};
                    if (startsWith(otherCopy, '#')) {
                        continue;
                    }
                } else {
                    continue;
                }
                if (isWhitespace(copyString)) {
                    continue;
                }

                //long int currentLine{std::distance<std::vector<std::string>::const_iterator>(buffer.begin(), iter)+1};
                long int currentLine{it.first};
                if (foundAliasPosition != std::string::npos) {
                    std::string targetPin{""};
                    std::string targetAlias{""};
                    if (copyString.find_last_of(")") == std::string::npos) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                        continue;
                    }
                    if (copyString.find(",") == std::string::npos) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_PARAMETER_SEPARATING_COMMA_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        if (copyString.find_last_of(")") != std::string::npos) {
                            std::cout << tWhitespace(it.second.find(")")) <<  EXPECTED_HERE_STRING << std::endl;
                        } else {
                            std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                        }
                        continue;
                    }
                    targetPin = copyString.substr(static_cast<std::string>(ALIAS_IDENTIFIER).length());
                    targetPin = targetPin.substr(0, targetPin.find(","));
                    copyString = it.second.substr(static_cast<std::string>(ALIAS_IDENTIFIER).length());
                    copyString = copyString.substr(copyString.find(",")+1);
                    copyString = trimWhitespace(copyString);
                    targetAlias = copyString.substr(0, (copyString.find_last_of(")") != std::string::npos ? copyString.find_last_of(")") : 0));
                    if (targetAlias.length() == 0) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_ALIAS_NAME_SPECIFIED_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.find(")")) << EXPECTED_HERE_STRING << std::endl;
                        continue;
                    }
                    if (targetAlias.find_last_of("\"") == targetAlias.find("\"")) {
                        if (targetAlias.find(targetAlias.find("\"")) != std::string::npos) {
                            std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                            std::cout << NO_CLOSING_QUOTATION_MARKS_FOUND_STRING << std::endl;
                            std::cout << it.second << std::endl;
                            std::cout << tWhitespace(it.second.find("\"")) << EXPECTED_HERE_STRING << std::endl;
                            continue;
                        } else {
                            std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                            std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING  << std::endl;
                            std::cout << it.second << std::endl;
                            std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                            continue;
                        }
                    }
                    targetAlias = stripAllFromString(targetAlias, "\"");
                    try {
                        int pin{0};
                        try {
                            if (uno) {
                                pin = Arduino::staticParseAnalogPin(ArduinoType::UNO, toUppercase(targetPin));
                            } else if (nano) {
                                pin = Arduino::staticParseAnalogPin(ArduinoType::NANO, toUppercase(targetPin));
                            } else {
                                pin = Arduino::staticParseAnalogPin(ArduinoType::MEGA, toUppercase(targetPin));
                            }
                        } catch (std::exception &e) {
                            (void)e;
                        }
                        if (pin == 0) {
                            pin = std::stoi(targetPin);
                        }
                        if (uno) {
                            this->m_unoAliasesMap.emplace(pin, targetAlias);
                        } else if (nano) {
                            this->m_nanoAliasesMap.emplace(pin, targetAlias);
                        } else {
                            this->m_megaAliasesMap.emplace(pin, targetAlias);
                        }
                    } catch (std::exception &e) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << PIN_NUMBER_NOT_AN_INTEGER_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.find(",")-1) << EXPECTED_HERE_STRING << std::endl;
                    }
                } else if (foundIOTypePosition != std::string::npos) {
                    std::string targetPin{""};
                    std::string targetIOType{""};
                    if (copyString.find_last_of(")") == std::string::npos) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                        continue;
                    }
                    if (copyString.find(",") == std::string::npos) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_PARAMETER_SEPARATING_COMMA_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        if (copyString.find_last_of(")") != std::string::npos) {
                            std::cout << tWhitespace(it.second.find(")")) <<  EXPECTED_HERE_STRING << std::endl;
                        } else {
                            std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                        }
                        continue;
                    }
                    targetPin = copyString.substr(static_cast<std::string>(IO_TYPE_IDENTIFIER).length());
                    targetPin = targetPin.substr(0, targetPin.find(","));
                    copyString = it.second.substr(static_cast<std::string>(IO_TYPE_IDENTIFIER).length());
                    copyString = copyString.substr(copyString.find(",")+1);
                    copyString = trimWhitespace(copyString);
                    targetIOType = copyString.substr(0, (copyString.find_last_of(")") != std::string::npos ? copyString.find_last_of(")") : 0));
                    if (targetIOType.length() == 0) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_IO_TYPE_SPECIFIED_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.find(")")) << EXPECTED_HERE_STRING << std::endl;
                        continue;
                    }
                    targetIOType = stripAllFromString(targetIOType, "\"");
                    try {
                        int pin{0};
                        try {
                            if (uno) {
                                pin = Arduino::staticParseAnalogPin(ArduinoType::UNO, toUppercase(targetPin));
                            } else if (nano) {
                                pin = Arduino::staticParseAnalogPin(ArduinoType::NANO, toUppercase(targetPin));
                            } else {
                                pin = Arduino::staticParseAnalogPin(ArduinoType::MEGA, toUppercase(targetPin));
                            }
                        } catch (std::exception &e) {
                            (void)e;
                        }
                        if (pin == 0) {
                            pin = std::stoi(targetPin);
                        }
                        if (uno) {
                            this->m_unoIOTypesMap.emplace(pin, targetIOType);
                        } else if (nano) {
                             this->m_nanoIOTypesMap.emplace(pin, targetIOType);
                        } else {
                            this->m_megaIOTypesMap.emplace(pin, targetIOType);
                        }
                    } catch (std::exception &e) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << PIN_NUMBER_NOT_AN_INTEGER_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.find(",")-1) << EXPECTED_HERE_STRING << std::endl;
                    }
                } else if (foundStatePosition != std::string::npos) {
                    std::string targetPin{""};
                    std::string targetState{""};
                    if (copyString.find_last_of(")") == std::string::npos) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                        continue;
                    }
                    if (copyString.find(",") == std::string::npos) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_PARAMETER_SEPARATING_COMMA_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        if (copyString.find_last_of(")") != std::string::npos) {
                            std::cout << tWhitespace(it.second.find(")")) <<  EXPECTED_HERE_STRING << std::endl;
                        } else {
                            std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                        }
                        continue;
                    }
                    targetPin = copyString.substr(static_cast<std::string>(INITIAL_STATE_IDENTIFIER).length());
                    targetPin = targetPin.substr(0, targetPin.find(","));
                    copyString = it.second.substr(static_cast<std::string>(INITIAL_STATE_IDENTIFIER).length());
                    copyString = copyString.substr(copyString.find(",")+1);
                    copyString = trimWhitespace(copyString);
                    targetState = copyString.substr(0, (copyString.find_last_of(")") != std::string::npos ? copyString.find_last_of(")") : 0));
                    if (targetState.length() == 0) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_STATE_SPECIFIED_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.find(")")) << EXPECTED_HERE_STRING << std::endl;
                        continue;
                    }
                    targetState = stripAllFromString(targetState, "\"");
                    try {
                        int pin{0};
                        try {
                            if (uno) {
                                pin = Arduino::staticParseAnalogPin(ArduinoType::UNO, toUppercase(targetPin));
                            } else if (nano) {
                                pin = Arduino::staticParseAnalogPin(ArduinoType::NANO, toUppercase(targetPin));
                            } else {
                                pin = Arduino::staticParseAnalogPin(ArduinoType::MEGA, toUppercase(targetPin));
                            }
                        } catch (std::exception &e) {
                            (void)e;
                        }
                        if (pin == 0) {
                            pin = std::stoi(targetPin);
                        }
                        if (uno) {
                            this->m_unoInitialStatesMap.emplace(pin, targetState);
                        } else if (nano) {
                            this->m_nanoInitialStatesMap.emplace(pin, targetState);
                        } else {
                            this->m_megaInitialStatesMap.emplace(pin, targetState);
                        }
                    } catch (std::exception &e) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << PIN_NUMBER_NOT_AN_INTEGER_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.find(",")-1) << EXPECTED_HERE_STRING << std::endl;
                    }
                } else if (foundBluetoothMultiplierPosition != std::string::npos) {
                    std::string targetState{""};
                    if (copyString.find_last_of(")") == std::string::npos) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                        continue;
                    }
                    if (copyString.length() > 1) {
                        while ((copyString.length() > 1) && (isWhitespace(copyString[0]))) {
                            copyString = stripFromString(copyString, " ");
                        }
                    }
                    targetState = copyString.substr(copyString.find(BLUETOOTH_MULTIPLIER_IDENTIFIER) + static_cast<std::string>(BLUETOOTH_MULTIPLIER_IDENTIFIER).length());
                    targetState = targetState.substr(0, (targetState.find_last_of(")") != std::string::npos ? copyString.find_last_of(")") : 0));
                    if (targetState.length() == 0) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_BLUETOOTH_MULTIPLIER_SPECIFIED_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.find(")")) << EXPECTED_HERE_STRING << std::endl;
                        continue;
                    }
                    targetState = stripAllFromString(targetState, "\"");
                    try {
                        double state{std::stod(targetState)};
                        if ((state < 1) || (state > 10)) {
                            std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                            std::cout << BLUETOOTH_MULTIPLIER_IS_OUT_OF_RANGE_STRING << std::endl;
                            std::cout << it.second << std::endl;
                            std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                            continue;
                        }
                        this->m_bluetoothMutiplier = state;
                    } catch (std::exception &e) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << BLUETOOTH_MULTIPLIER_IS_NOT_A_NUMBER_STRING << std::endl;
                        std::cout << it.second << std::endl;
                        std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                    }
                } else {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << CONFIG_EXPRESSION_MALFORMED_STRING << std::endl;
                    std::cout << it.second << std::endl;
                    std::cout << tWhitespace(it.second.length()-1) << HERE_STRING << std::endl;
                }
            } catch (std::exception &e) {
                 std::cout << EXCEPTION_IN_CONSTRUCTOR_STRING << e.what() << std::endl;
            }
        }
    }
}

std::map<int, std::string> ConfigurationFileReader::unoAliasesMap() const
{
    return this->m_unoAliasesMap;
}

std::map<int, std::string> ConfigurationFileReader::unoIOTypesMap() const
{
    return this->m_unoIOTypesMap;
}

std::map<int, std::string> ConfigurationFileReader::unoInitialStatesMap() const
{
    return this->m_unoInitialStatesMap;
}

std::map<int, std::string> ConfigurationFileReader::nanoAliasesMap() const
{
    return this->m_nanoAliasesMap;
}

std::map<int, std::string> ConfigurationFileReader::nanoIOTypesMap() const
{
    return this->m_nanoIOTypesMap;
}

std::map<int, std::string> ConfigurationFileReader::nanoInitialStatesMap() const
{
    return this->m_nanoInitialStatesMap;
}

std::map<int, std::string> ConfigurationFileReader::megaAliasesMap() const
{
    return this->m_megaAliasesMap;
}

std::map<int, std::string> ConfigurationFileReader::megaIOTypesMap() const
{
    return this->m_megaIOTypesMap;
}

std::map<int, std::string> ConfigurationFileReader::megaInitialStatesMap() const
{
    return this->m_megaInitialStatesMap;
}

double ConfigurationFileReader::bluetoothMultiplier() const
{
    return this->m_bluetoothMutiplier;
}
