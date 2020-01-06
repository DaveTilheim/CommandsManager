#include "Command.hpp"


int main(int argc, char const *argv[])
{
	Command echo("echo");
	Command c("quit");
	c.func(F
	{
		return Command::EXIT_TERMINAL;
	});

	echo.func(F
	{
		while(not args.end())
		{
			cout << (string)args << " ";
		}
		cout << endl;
		return 0;
	}, Command::DYNAMIC).func(F
	{
		cout << "empty echo" << endl;
		return 0;
	}).sub("user").func(F
	{
		cout << "Arthur Detrembleur" << endl;
		return 0;
	}).setDescription("display the app creator's name");

	echo.getSub("user").sub("test").func(F
	{
		cout << args[0] << " " << args[1] << " " << args[2] << endl;
		return 0;
	}, 3);

	echo.setDescription("display the command parameters\ntest new line");
	Command brik("brik");
	cout << Command::terminal("$[ ") << endl;



	return 0;
}





