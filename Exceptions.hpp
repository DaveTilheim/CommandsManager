#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <string>
#include <functional>
#include <map>
#include "Tokens.hpp"

using namespace std;

class CommandException
{
protected:
	string msg;
public:
	CommandException(string msg="CommandException throwed");
	CommandException(const CommandException& cp);
	string getMsg() const;
};

class NotAFunctionalCommandException : public CommandException
{
public:
	NotAFunctionalCommandException(string cmdName);
};

class BadCommandParametersException : public CommandException
{
public:
	BadCommandParametersException(string name, int errargs, const map<int, function<int(const Tokens&)>> &functions);
};

class Command;

class CommandNotExistsException : public CommandException
{
public:
	CommandNotExistsException(string name, const map<string, Command*> &c);
};

class SubCommandNotExistsException : public CommandException
{
public:
	SubCommandNotExistsException(string name, string root, const map<string, Command*> &c);
};

#endif
