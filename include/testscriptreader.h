#ifndef ARDUINOPC_SERIALSCRIPTREADER_H
#define ARDUINOPC_SERIALSCRIPTREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>

#include <fileutilities.h>
#include <generalutilities.h>
#include <mathutilities.h>

#include "arduinopcstrings.h"
#include "serialterminal/serialcommand.h"
#include "serialterminal/serialcommanderrormessage.h"

class SerialScriptReader
{
public:
    SerialScriptReader(const std::string &scriptFilePath);
    std::shared_ptr<std::vector<SerialCommand>> commands() const;

    std::shared_ptr<std::vector<SerialCommandErrorMessage>> invalidCommandErrorMessages() const;
    bool hasInvalidCommands() const;
private:
    std::string m_scriptFilePath;
    std::shared_ptr<std::vector<SerialCommand>> m_commands;
    std::shared_ptr<std::vector<SerialCommandErrorMessage>> m_invalidCommandErrorMessages;
    bool m_hasInvalidCommands;

    void addToErrorMessages(const std::string &reason,
                            int lineNumber,
                            const std::string &badLine,
                            int howMuchWhitespace,
                            const std::string &expectedOrHere);

    void printErrorToStdout(const std::string &reason,
                            int lineNumber,
                            const std::string &badLine,
                            int howMuchWhitespace,
                            const std::string &expectedOrHere);
};

#endif //ARDUINOPC_SERIALSCRIPTREADER_H
