#include <iostream>
#include "Prompt.hpp"
#include "StdCommand.hpp"






int main(int argc, char const *argv[])
{
	StdCommand::initStdCommands();
	//Prompt::run(">>> ");
	Command::exeFile("main.lg");


	return 0;
}
