#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <map>
#include <functional>
#include "Tokens.hpp"

typedef Tokens Args;

class Command;

enum ARGS_TYPE
{
	STATIC_ARGS,
	DYNAMIC_ARGS
}

class Prototype
{
private:
	const ARGS_TYPE argsType;
	const int nargs;
	const Command *super;
	string description;
	function<void *(Args)> func;
};


exe2 print "Hello" addall 45 1 666

exe2 => prototypes[-2]
	print => prototypes[1]
		func("Hello")
	addall => prototypes[...]
		func(45, 1, 666)


print "Hello-world"

print
	prototypes[1]
		func("Hello-world") => STATIC_ARGS

print sin 90

print
	prototypes[-1]
		func(sin 90) => DYNAMIC_ARGS
			sin
				prototypes[1]
					func(90) => STATIC_ARGS


class Command
{
private:
	string name;
	Command *super;
	map<int, Prototype *> prototypes;
	vector<Command *> subs;
public:
};

#endif
