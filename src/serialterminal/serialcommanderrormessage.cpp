#include "serialterminal/serialcommanderrormessage.h"

SerialCommandErrorMessage::SerialCommandErrorMessage(const std::string &reason,
                                                      int lineNumber,
                                                      const std::string &badLine,
                                                      int howMuchWhitespace,
                                                      const std::string &expectedOrHere) :
    m_errorMessages { std::make_shared<std::vector<std::string>>() }
{
    using namespace GeneralUtilities;
    using namespace ArduinoPCStrings;
    this->m_errorMessages->emplace_back(toString(GENERIC_CONFIG_WARNING_BASE_STRING) + std::to_string(lineNumber) + toString(GENERIC_CONFIG_WARNING_TAIL_STRING));
    this->m_errorMessages->emplace_back(reason);
    this->m_errorMessages->emplace_back(badLine);
    this->m_errorMessages->emplace_back(tWhitespace(howMuchWhitespace) + expectedOrHere);
}

std::string SerialCommandErrorMessage::genericMessageLine() const
{
    return this->m_errorMessages->at(SerialCommandError::GENERIC_MESSAGE_LINE);
}

std::string SerialCommandErrorMessage::reasonLine() const
{
    return this->m_errorMessages->at(SerialCommandError::REASON_LINE);
}

std::string SerialCommandErrorMessage::badCommandLine() const
{
    return this->m_errorMessages->at(SerialCommandError::BAD_COMMAND_LINE);
}

std::string SerialCommandErrorMessage::expectedWhereLine() const
{
    return this->m_errorMessages->at(SerialCommandError::GENERIC_MESSAGE_LINE);
}

std::shared_ptr<std::vector<std::string>> SerialCommandErrorMessage::errorMessages() const
{
    return this->m_errorMessages;
}
