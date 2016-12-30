#ifndef ARDUINOPC_SERIALCOMMANDERRORMESSAGE_H
#define ARDUINOPC_SERIALCOMMANDERRORMESSAGE_H

enum SerialCommandError { GENERIC_MESSAGE_LINE, REASON_LINE, BAD_COMMAND_LINE, EXPECTED_WHERE_LINE };

#include <string>
#include <vector>
#include <memory>
#include <generalutilities.h>
#include "arduinopcstrings.h"

class SerialCommandErrorMessage
{
public:
    SerialCommandErrorMessage(const std::string &reason,
                              int lineNumber,
                              const std::string &badLine,
                              int howMuchWhitespace,
                              const std::string &expectedOrHere);
    std::string genericMessageLine() const;
    std::string reasonLine() const;
    std::string badCommandLine() const;
    std::string expectedWhereLine() const;
    std::shared_ptr<std::vector<std::string>> errorMessages() const;

private:
    std::shared_ptr<std::vector<std::string>> m_errorMessages;
};

#endif //ARDUINOPC_SERIALCOMMANDERRORMESSAGE_H
