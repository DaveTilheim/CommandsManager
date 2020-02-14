#include "CommandException.hpp"


CommandException::CommandException(string msg) : msg(msg)
{

}

CommandException::CommandException(const CommandException& cp) : msg(cp.what())
{

}

string CommandException::what() const
{
	return msg;
}
