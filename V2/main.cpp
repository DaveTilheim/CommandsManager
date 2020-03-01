#include <iostream>
#include "Prompt.hpp"
#include "StdCommand.hpp"






int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		cerr << "file required" << endl;
		exit(1);
	}
	StdCommand::initStdCommands();
	//Prompt::run(">>> ");
	Command::exeFile(argv[1]);


	return 0;
}
