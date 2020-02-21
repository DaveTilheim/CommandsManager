#ifndef STD_COMMAND_HPP
#define STD_COMMAND_HPP
#include "Command.hpp"
#include "Node.hpp"

class StdCommand
{
private:
	static Command ccreate;
	static Command cread;
	static Node root;
public:
	static void initStdCommands();
};

#endif
