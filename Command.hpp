#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <iomanip>
#include "Exceptions.hpp"

typedef const Tokens& Args;
#define F [](Args args)

class Command
{
private:
	static Command std_help_cmp;
	static map<string, Command *> commands;
	static const string std_help_name;
	string name;
	map<int, function<int(const Tokens&)>> *functions = nullptr;
	map<string, Command *> *subCommands = nullptr;
public:
	static const int EXIT_TERMINAL;
	Command() = delete;
	Command(string name, function<void(Command&)> init, char root=true);
	Command(string name, char root=true);
	Command(const Command&) = delete;
	~Command();
	Command& func(function<int(const Tokens&)> f, int params=0);
	Command& sub(string name);
	Command& getSub(string name);
	int run() noexcept(false);
	int run(Tokens&) noexcept(false);
	void show(int tabs=0) const;
	static int exe(string name, Tokens& args);
	static int exe(Tokens& command);
	static Tokens input();
	static int prompt();
	static int prompt(string out);
	static int terminal(string out="> ");
	static void showAll();
};

#endif
