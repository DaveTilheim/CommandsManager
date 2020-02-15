#include <iostream>
#include "Command.hpp"
#include <math.h>
#include <cstdlib>


int main(int argc, char const *argv[])
{
	

	Command calc("calc");
	calc.proto([](Args args)
	{
		return "no result";
	}, 0);

	calc.sub("add")
	.proto([](Args args)
	{
		return to_string((double)args + (double)args);
	}, 2)
	.proto([](Args args)
	{
		double buf = 0.f;
		while(not args.end())
		{
			buf += (double)args;
		}
		return to_string(buf);
	}, UNDEFINED);

	calc.sub("sub")
	.proto([](Args args)
	{
		return to_string((double)args - (double)args);
	}, 2);

	calc.sub("mul")
	.proto([](Args args)
	{
		return to_string((double)args * (double)args);
	}, 2);

	calc.sub("div")
	.proto([](Args args)
	{
		return to_string((double)args / (double)args);
	}, 2);



	calc.arm();

	Command c("mul");
	c.proto([](Args args)
	{
		return args[0] + "*" + args[1];
	}, 2);
	c.arm();

	
	cout << Command::exe("mul mul 5 6 calc mul:r 2 9") << endl;


	//Command::exe("print size 5 P");


	return 0;
}
