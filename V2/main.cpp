#include <iostream>
#include "Command.hpp"



int main(int argc, char const *argv[])
{
	
	Command c("print");

	c.prototype([](Args args)
	{
		cout << "> " << args  << "      " << args.count()<< endl;
		return nullptr;
	}, "print ...")
	.prototype([](Args args)
	{
		cout << "YEAH!" << endl;
		return nullptr;
	}, "print")
	.prototype([](Args args)
	{
		cout << "ONE" << endl;
		return nullptr;
	}, "print one");

	cout << c << endl;

	c(Tokens("Hello world!"));
	c(Tokens(""));
	c(Tokens("Hello"));
	c(Tokens("Hey hey hey!"));
	c(Tokens("\"Hey hey hey!"));
	

	return 0;
}
