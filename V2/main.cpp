#include <iostream>
#include "Prompt.hpp"
#include "StdCommand.hpp"






int main(int argc, char const *argv[])
{
	StdCommand::initStdCommands();
	Prompt::run(">>> ");
	


	
	

	return 0;
}
