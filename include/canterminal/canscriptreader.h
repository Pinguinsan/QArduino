#ifndef ARDUINOPC_CANSCRIPTREADER_H
#define ARDUINOPC_CANSCRIPTREADER_H

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
#include "canterminal/cancommand.h"

class CanScriptReader
{
public:
    CanScriptReader(const std::string &scriptFilePath);
    std::shared_ptr<std::vector<CanCommand>> commands() const;

private:
    std::string m_scriptFilePath;
    std::shared_ptr<std::vector<CanCommand>> m_commands;

};

#endif //ARDUINOPC_CANSCRIPTREADER_H
