#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <map>
#include "Prototype.hpp"

class Command final
{
private:
	string name;
	const Command *super;
	map<int, Prototype *> prototypes;
	vector<Command *> subcommands;
public:
	Command() = delete;
	Command(const Command&) = delete;
	Command(string name, const Command *super=nullptr);
	~Command();
	Command& prototype(Prototype *proto, int nargs=0);
	Command& prototype(function<void *(Args)>, int nargs=0, string description="<no description>");
	Command& prototype(function<void *(Args)>, string form, string description="<no description>");
	string getName() const;
	string getFullName() const;
	bool isSubCommand(Tokens tokens);
	Command& addSub(string name);
	Command& sub(string name);
	void *call(Args args);
	void *operator()(Args args);
	Command& operator=(const Command&) = delete;
	friend ostream& operator<<(ostream& out, const Command& cmd);
};

#endif
