#include "Command.hpp"


int main(int argc, char const *argv[])
{
	Command command("show");
	Command c("quit");
	c.func(F
	{
		return Command::EXIT_TERMINAL;
	});

	command.sub("all").func(F
	{
		Command::showAll();
		return 0;
	});
	command.sub("nothing").func(F
	{
		cout << "nothing :3" << endl;
		return 0;
	});
	
	cout << Command::terminal() << endl;

	return 0;
}





