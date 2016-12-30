#ifndef ARDUINOPC_CANCOMMAND_H
#define ARDUINOPC_CANCOMMAND_H

#include <string>
#include "serialterminal/serialcommandhistoryaction.h"

enum class CanCommandType { DELAY_SECONDS, DELAY_MILLISECONDS, DELAY_MICROSECONDS, WRITE, READ, FLUSH_RX, FLUSH_TX, FLUSH_RX_TX, LOOP_START, LOOP_END, COMMAND_UNSPECIFIED };

class CanCommand
{
public:
    CanCommand(CanCommandType commandType, const std::string &commandArgument);
    CanCommandType commandType() const;
    std::string commandArgument() const;
    void setCommandType(CanCommandType commandType);
    void setCommandArgument(const std::string &commandArgument);

private:
    CanCommandType m_commandType;
    std::string m_commandArgument;
};

#endif //ARDUINOPC_CANCOMMAND_H
