#ifndef COMMAND_EXCEPTION_HPP
#define COMMAND_EXCEPTION_HPP
#include <string>

using namespace std;


class CommandException
{
private:
	const string msg;
public:
	CommandException() = delete;
	CommandException(string msg);
	CommandException(const CommandException& cp);
	string what() const;
};


#endif
