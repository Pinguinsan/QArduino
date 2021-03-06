#include "serialterminal/serialscriptreader.h"

SerialScriptReader::SerialScriptReader(const std::string &scriptFilePath) :
    m_scriptFilePath{scriptFilePath},
    m_commands{std::make_shared<std::vector<SerialCommand>>()},
    m_invalidCommandErrorMessages{std::make_shared<std::vector<SerialCommandErrorMessage>>()},
    m_hasInvalidCommands{false}
{
    using namespace FileUtilities;
    using namespace GeneralUtilities;
    using namespace MathUtilities;
    using namespace ArduinoPCStrings;
    if (!fileExists(this->m_scriptFilePath)) {
        throw std::runtime_error(SCRIPT_FILE_DOES_NOT_EXISTS_STRING + tQuoted(this->m_scriptFilePath));
    }
    std::vector<std::string> buffer;
    std::ifstream readFromFile;
    readFromFile.open(this->m_scriptFilePath);
    if (readFromFile.is_open()) {
        std::string tempString{""};
        while (std::getline(readFromFile, tempString)) {
            buffer.emplace_back(tempString);
        }
        readFromFile.close();
    } else {
        throw std::runtime_error(UNABLE_TO_OPEN_SCRIPT_FILE_STRING + tQuoted(this->m_scriptFilePath));
    }

    buffer = trimWhitespace(buffer);
    int loop{0};
    int loopCount{0};
    for (std::vector<std::string>::const_iterator iter = buffer.begin(); iter != buffer.end(); iter++) {
        try {
            std::string copyString{*iter};
            std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
            //TODO: Replace with regex for searching
            size_t foundDelayPosition{copyString.find(DELAY_IDENTIFIER)};
            size_t foundWritePosition{copyString.find(WRITE_IDENTIFIER)};
            size_t foundReadPosition{copyString.find(READ_IDENTIFIER)};
            size_t foundFlushRxPosition{copyString.find(FLUSH_RX_IDENTIFIER)};
            size_t foundFlushTxPosition{copyString.find(FLUSH_TX_IDENTIFIER)};
            size_t foundFlushRxTxPosition{copyString.find(FLUSH_RX_TX_IDENTIFIER)};
            size_t foundFlushTxRxPosition{copyString.find(FLUSH_TX_RX_IDENTIFIER)};
            size_t foundLoopPosition{copyString.find(LOOP_IDENTIFIER)};
            size_t foundClosingLoopPosition{copyString.find(CLOSING_LOOP_IDENTIFIER)};
            if (copyString.length() != 0) {
                std::string otherCopy{copyString};
                unsigned int numberOfWhitespace{0};
                while (otherCopy.length() > 1 && isWhitespace(otherCopy[0])) {
                    stripFromString(otherCopy, ' ');
                    numberOfWhitespace++;
                }
                if (copyString.length() > numberOfWhitespace) {
                    if (copyString[numberOfWhitespace] == '#') {
                        continue;
                    }
                }
            } else {
                continue;
            }
            if (isWhitespace(copyString)) {
                continue;
            }

            long int currentLine{std::distance<std::vector<std::string>::const_iterator>(buffer.begin(), iter)+1};
            if (foundLoopPosition != std::string::npos) {
                std::string targetLoopCount{0};
                if (copyString.find(")") == std::string::npos) {
                    printErrorToStdout(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    addToErrorMessages(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    continue;
                }
                targetLoopCount = copyString.substr(static_cast<std::string>(LOOP_IDENTIFIER).length());
                targetLoopCount = targetLoopCount.substr(0, targetLoopCount.find(")"));
                if (trimWhitespace(targetLoopCount) == "") {
                    loop++;
                    loopCount = -1;
                    this->m_commands->emplace_back(SerialCommandType::LOOP_START, std::to_string(loopCount));
                } else {
                    try {
                        loopCount = tAbs(std::stoi(targetLoopCount));
                        loop++;
                        this->m_commands->emplace_back(SerialCommandType::LOOP_START, targetLoopCount);
                    } catch (std::exception &e) {
                        printErrorToStdout(LOOP_COUNT_PARAMETER_NOT_AN_INTEGER_STRING, currentLine, *iter, iter->find(")")-1, EXPECTED_HERE_STRING);
                        addToErrorMessages(LOOP_COUNT_PARAMETER_NOT_AN_INTEGER_STRING, currentLine, *iter, iter->find(")")-1, EXPECTED_HERE_STRING);
                    }
                }
            } else if ((foundClosingLoopPosition != std::string::npos) && (trimWhitespace(copyString).length() == 1)) {
                if (!loop) {
                    printErrorToStdout(UNEXPECTED_LOOP_CLOSING_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    addToErrorMessages(UNEXPECTED_LOOP_CLOSING_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    continue;
                } else {
                    loop--;
                    this->m_commands->emplace_back(SerialCommandType::LOOP_END, "");
                }
            } else if (foundDelayPosition != std::string::npos) {
                std::string targetDelay{""};
                std::string identifier{""};
                size_t foundDelaySecondsPosition{copyString.find(DELAY_SECONDS_IDENTIFIER)};
                size_t foundDelayMillisecondsPosition{copyString.find(DELAY_MILLISECONDS_IDENTIFIER)};
                size_t foundDelayMicrosecondsPosition{copyString.find(DELAY_MICROSECONDS_IDENTIFIER)};
                SerialCommandType commandType{SerialCommandType::COMMAND_UNSPECIFIED};
                if (foundDelaySecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_SECONDS_IDENTIFIER);
                    commandType = SerialCommandType::DELAY_SECONDS;
                } else if (foundDelayMillisecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_MILLISECONDS_IDENTIFIER);
                    commandType = SerialCommandType::DELAY_MILLISECONDS;
                } else if (foundDelayMicrosecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_MICROSECONDS_IDENTIFIER);
                    commandType = SerialCommandType::DELAY_MICROSECONDS;
                } else {
                    printErrorToStdout(CONFIG_EXPRESSION_MALFORMED_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    addToErrorMessages(CONFIG_EXPRESSION_MALFORMED_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                }
                if (copyString.find(")") == std::string::npos) {
                    printErrorToStdout(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    addToErrorMessages(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    continue;
                }
                targetDelay = copyString.substr(identifier.length());
                targetDelay = targetDelay.substr(0, targetDelay.find(")"));

                try {
                    long long int delay{std::stoll(targetDelay)};
                    (void)delay;
                    this->m_commands->emplace_back(commandType, targetDelay);
                } catch (std::exception &e) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    if (commandType == SerialCommandType::DELAY_SECONDS) {
                        std::cout << DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    } else if (commandType == SerialCommandType::DELAY_MILLISECONDS) {
                        std::cout << DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    } else if (commandType == SerialCommandType::DELAY_MICROSECONDS) {
                        std::cout << DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    }
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->find(")")-1) << EXPECTED_HERE_STRING << std::endl;
                }
            } else if (foundWritePosition != std::string::npos) {
                std::string targetString{""};
                if (copyString.find(")") == std::string::npos) {
                    printErrorToStdout(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    addToErrorMessages(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    continue;
                }
                targetString = iter->substr(static_cast<std::string>(WRITE_IDENTIFIER).length(), iter->find(")"));
                if (targetString.find_last_of("\"") == targetString.find("\"")) {
                    if (targetString.find(targetString.find("\"")) != std::string::npos) {
                        printErrorToStdout(NO_CLOSING_QUOTATION_MARKS_FOUND_STRING, currentLine, *iter, iter->find("\""), EXPECTED_HERE_STRING);
                        addToErrorMessages(NO_CLOSING_QUOTATION_MARKS_FOUND_STRING, currentLine, *iter, iter->find("\""), EXPECTED_HERE_STRING);
                        continue;
                    } else {
                        printErrorToStdout(WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                        addToErrorMessages(WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                        continue;
                    }
                }
                targetString = targetString.substr(targetString.find("\"")+1);
                targetString = targetString.substr(0, targetString.find("\""));
                this->m_commands->emplace_back(SerialCommandType::WRITE, targetString);
            } else if (foundReadPosition != std::string::npos) {
                if (copyString.find(")") == std::string::npos) {
                    printErrorToStdout(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    addToErrorMessages(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    continue;
                }
                this->m_commands->emplace_back(SerialCommandType::READ, "");
            } else if (foundFlushRxPosition != std::string::npos) {
                if (copyString.find(")") == std::string::npos) {
                    printErrorToStdout(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    addToErrorMessages(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    continue;
                }
                this->m_commands->emplace_back(SerialCommandType::FLUSH_RX, "");
            } else if (foundFlushTxPosition != std::string::npos) {
                if (copyString.find(")") == std::string::npos) {
                    printErrorToStdout(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    addToErrorMessages(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    continue;
                }
                this->m_commands->emplace_back(SerialCommandType::FLUSH_TX, "");
            } else if ((foundFlushTxRxPosition != std::string::npos) || (foundFlushRxTxPosition != std::string::npos)) {
                if (copyString.find(")") == std::string::npos) {
                    printErrorToStdout(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    addToErrorMessages(NO_CLOSING_PARENTHESIS_FOUND_STRING, currentLine, *iter, iter->length()-1, HERE_STRING);
                    continue;
                }
                this->m_commands->emplace_back(SerialCommandType::FLUSH_RX_TX, "");
            } else {
                std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                std::cout << CONFIG_EXPRESSION_MALFORMED_STRING << std::endl;
                std::cout << *iter << std::endl;
                std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
            }
        } catch (std::exception &e) {
            std::cout << EXCEPTION_IN_CONSTRUCTOR_STRING << e.what() << std::endl;
            this->m_commands->clear();
            return;
        }
    }
    if (loop) {
        printErrorToStdout(UNTERMINATED_LOOP_STRING, buffer.size(), "", 0, EXPECTED_HERE_STRING);
        addToErrorMessages(UNTERMINATED_LOOP_STRING, buffer.size(), "", 0, EXPECTED_HERE_STRING);
        this->m_commands->clear();
        return;
    }
}

void SerialScriptReader::addToErrorMessages(const std::string &reason,
                                            int lineNumber,
                                            const std::string &badLine,
                                            int howMuchWhitespace,
                                            const std::string &expectedOrHere)
{
    this->m_invalidCommandErrorMessages->emplace_back(reason, lineNumber, badLine, howMuchWhitespace, expectedOrHere);
    this->m_hasInvalidCommands = true;
}

void SerialScriptReader::printErrorToStdout(const std::string &reason,
                                            int lineNumber,
                                            const std::string &badLine,
                                            int howMuchWhitespace,
                                            const std::string &expectedOrHere)
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << lineNumber << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
    std::cout << reason << std::endl;
    std::cout << badLine << std::endl;
    std::cout << tWhitespace(howMuchWhitespace) << expectedOrHere << std::endl;
}

std::shared_ptr<std::vector<SerialCommand>> SerialScriptReader::commands() const
{
    return this->m_commands;
}

bool SerialScriptReader::hasInvalidCommands() const
{
    return this->m_hasInvalidCommands;
}

std::shared_ptr<std::vector<SerialCommandErrorMessage>> SerialScriptReader::invalidCommandErrorMessages() const
{
    return this->m_invalidCommandErrorMessages;
}
