#include "canterminal/cancommand.h"

CanCommand::CanCommand(CanCommandType commandType, const std::string &commandArgument) :
    m_commandType{commandType},
    m_commandArgument{commandArgument}
{

}

CanCommandType CanCommand::commandType() const
{
    return this->m_commandType;
}

std::string CanCommand::commandArgument() const
{
    return this->m_commandArgument;
}

void CanCommand::setCommandType(CanCommandType commandType)
{
    this->m_commandType = commandType;
}

void CanCommand::setCommandArgument(const std::string &commandArgument)
{
    this->m_commandArgument = commandArgument;
}
