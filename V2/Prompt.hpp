#ifndef PROMPT_HPP
#define PROMPT_HPP
#include "Command.hpp"
#include <cstdlib>
#include <fstream>


struct command_handler
{
	static string exit(Args args);
	static string help0(Args args);
	static string helpU(Args args);
	static string help_name(Args args);
	static string sys(Args args);
	static string sys_result(Args args);
	static string nlstr(Args args);
};

class Prompt
{
private:
	static string nlstr;
	static Command cexit;
	static Command chelp;
	static Command csys;
	static Command cnlstr;
	static void init();
	friend struct command_handler;
public:
	static int run(string nlstr="> ");
	static void exit() noexcept(false);
	static void exeFile(string filename);
};


#endif
