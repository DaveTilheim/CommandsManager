#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <map>
#include <ctype.h>
#include <cstdlib>
#include "Prototype.hpp"

#ifdef DEBUG_MODE
#define DBG cerr<<"DBG: "<<__FILE__<<" "<<__func__<<" "<<__LINE__<<endl;
#else
#define DBG
#endif

enum ProtoArgs
{
	UNDEFINED=-1,
	NONE=0
};

class Command final
{
private:
	string name;
	const Command *super;
	map<int, Prototype *> prototypes;
	vector<Command *> subcommands;
	static map<string, Command *> armedCommands;
	static string Tabs(int n);
	static string lastResult;
	static bool skipNextCmd;
public:
	Command() = delete;
	Command(const Command&) = delete;
	Command(string name, Command&, int n, const Command *super=nullptr);
	Command(string name, const Command *super=nullptr);
	~Command();
	Command& proto(Prototype *proto, int nargs=0);
	Command& proto(function<string (Args)>, int nargs=0, string description="<no description>");
	Command& proto(function<string (Args)>, string form, string description="<no description>");
	string getName() const;
	string getFullName() const;
	const Prototype& getProto(int i);
	static string getCallStatus(string& cs);
	int getMaxNargs() const;
	bool isSubCommand(Tokens tokens) const;
	static bool isCommand(Tokens tokens);
	Command& addSub(string name);
	Command& sub(string name);
	vector<Command *>& subs();
	string call(Args& args, string status="") noexcept(false);
	Command& arm();
	string operator()(Args args);
	void printNames() const;
	Command& operator=(const Command&) = delete;
	static string exe(string scommand) noexcept(false);
	static string exeInput() noexcept(false);
	static map<string, Command *>& getAll();
	static string getLastResult();
	static void skip(bool state);
	friend ostream& operator<<(ostream& out, const Command& cmd);
};


#endif
