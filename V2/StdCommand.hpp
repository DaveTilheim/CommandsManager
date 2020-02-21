#ifndef STD_COMMAND_HPP
#define STD_COMMAND_HPP
#include "Command.hpp"
#include "Node.hpp"

class StdCommand
{
private:
	static Command ccreate;
	static Command cread;
	static Command cupdate;
	static Command ctype;
	static Node root;
	static string create_command(Args args);
	static string read_command(Args args);
	static string read_memory_command(Args args);
	static string update_command(Args args);
	static string type_command(Args args);
public:
	static void initStdCommands();
};

#endif
