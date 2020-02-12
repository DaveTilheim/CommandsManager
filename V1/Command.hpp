#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <iomanip>
#include "Exceptions.hpp"

typedef const Tokens& Args;
#define F [](Args args)
#define std_cmd

class Command
{
private std_cmd:
	static Command std_help_cmp;
	static const string std_help_name;
	static Command std_list_cmp;
	static const string std_list_name;
	static Command std_exit_cmp;
	static const string std_exit_name;
private:
	static map<string, Command *> commands;
	static const int IGNORE_CMD;
	string name;
	string description = "no description";
	map<int, function<int(const Tokens&)>> *functions = nullptr;
	map<string, Command *> *subCommands = nullptr;
public:
	static const int EXIT_TERMINAL;
	static const int DYNAMIC;
	Command() = delete;
	Command(string name, function<void(Command&)> init, char root=true);
	Command(string name, char root=true);
	Command(const Command&) = delete;
	~Command();
	Command& func(function<int(const Tokens&)> f, int params=0);
	Command& sub(string name);
	Command& getSub(string name) noexcept(false);
	int run() noexcept(false);
	int run(Tokens&) noexcept(false);
	void help(int tabs=0) const;
	void list(string root="") const;
	void listFuncs(string root="") const;
	void listSubs(string root="") const;
	void helpDescription(int tabs=0) const;
	void helpFuncs(int tabs=0) const;
	void helpSubs(int tabs=0) const;
	void setDescription(string d);
	string getDescription() const;
	static int exe(string name, Tokens& args);
	static int exe(Tokens& command);
	static Tokens input();
	static int prompt();
	static int prompt(string out);
	static int terminal(string out="> ");
	static void helpAll();
	static const map<string, Command *>& getCommands();
};

#endif
