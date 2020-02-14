#include <iostream>
#include "Command.hpp"
#include <math.h>
#include <cstdlib>


int main(int argc, char const *argv[])
{
	Command c("print");
	c.proto([](Args args)
	{
		return "Hello world!";
	}, NONE)
	.proto([](Args args)
	{
		cout << "out: " << args << endl;
		return to_string(args.count());
	}, UNDEFINED)
	.sub("nargs")
	.proto([](Args args)
	{
		cout << "out: " << args << " => " << args.count() << endl;
		return to_string(args.count());
	}, UNDEFINED);

	c.sub("user")
	.proto([](Args args)
	{
		cout << "Hello Arthur Detrembleur!" << endl;
		return ":)";
	}, NONE);

	Command cc("lock");
	cc.proto([](Args args)
	{
		return "{" + args[0] + "}";
	}, 1)
	.proto([](Args args)
	{
		return args[1] + args[0] + args[1];
	}, 2);

	c.arm();
	cc.arm();

	Command sinus("sin");
	sinus.proto([](Args args)
	{
		return to_string(sin((double)args));
	}, 1);

	sinus.arm();

	//string ret;
	Command::exe("print lock sin 100 lock hello sin 100"); //lock_2 (call explicit du proto[2])
	/*print
		lock
			sin
				100
			lock
				hello
				sin
					100
	out: 0.5hello0.50.50.5hello0.5
	*/


	return 0;
}
