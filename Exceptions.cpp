#include "Exceptions.hpp"


CommandException::CommandException(string msg) : msg(msg)
{

}

CommandException::CommandException(const CommandException& cp)
{
	msg = cp.getMsg();
}

string CommandException::getMsg() const
{
	return msg;
}


NotAFunctionalCommandException::NotAFunctionalCommandException(string cmdName)
: CommandException("'" + cmdName + "' is not functional (do nothing)")
{

}

BadCommandParametersException::BadCommandParametersException(string name, int errargs, const map<int, function<int(const Tokens&)>> &functions)
: CommandException("'" + name + "' don't take " + to_string(errargs) + " parameters")
{
	msg += "\n\ttry: ";
	for(auto it : functions)
	{
		msg += to_string(it.first) + " ";
	}
}

CommandNotExistsException::CommandNotExistsException(string name, const map<string, Command*> &c)
: CommandException("'" + name + "' command not exists")
{
	msg += "\n\ttry:";
	for(auto it : c)
	{
		msg += " " + it.first;
	}
}

SubCommandNotExistsException::SubCommandNotExistsException(string name, string root, const map<string, Command*> &c)
: CommandException("'"+ name + "' sub command not exists for '" + root + "' root command")
{
	msg += "\n\ttry:";
	for(auto it : c)
	{
		msg += " " + it.first;
	}
}
