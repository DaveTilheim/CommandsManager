#include <iostream>
#include "Command.hpp"



int main(int argc, char const *argv[])
{
	
	Command c("print");

	c.prototype([](Args args)
	{
		cout << args[0] << endl;
		return nullptr;
	}, 1)
	.prototype([](Args args)
	{
		cout << args[0] << " <=> " << args[1] << endl;
		return nullptr;
	}, 2);

	c.sub("user").prototype([](Args args)
	{
		cout << "Hello Arthur Detrembleur!" << endl;
		return nullptr;
	}, 0)
	.sub("firstname").prototype([](Args args)
	{
		cout << "Hello Arthur!" << endl;
		return nullptr;
	});

	c(Tokens("user firstname"));
	c(Tokens("\"user firstname\""));
	c(Tokens("user"));
	c(Tokens("\"user\""));
	c(Tokens("[user]"));
	c(Tokens("\"It's me MARIO!"));
	c(Tokens("\"It's me MARIO!\" Hey!"));
	c(Tokens("\"It's me MARIO!\" \"Hey oh!"));
	c(Tokens("\"It's me MARIO! Hey oh!"));
	c(Tokens("[user] \"It's me MARIO!\""));

	return 0;
}
