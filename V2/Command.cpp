#include "Command.hpp"


Command::Command(string name, const Command *super) : super(super), name(name)
{

}

Command::~Command()
{
	for(auto pr : prototypes)
	{
		delete pr.second;
	}

	for(auto sub : subcommands)
	{
		delete sub;
	}
}

string Command::getName() const
{
	return name;
}

string Command::getFullName() const
{
	if(super)
		return super->getFullName() + " " + name;
	return getName();
}

Command& Command::prototype(Prototype *proto, int nargs)
{
	if(prototypes.find(nargs) != prototypes.end())
	{
		delete prototypes[nargs];
	}
	prototypes[nargs] = proto;
	return *this;
}

Command& Command::prototype(function<void *(Args)> func, int nargs, string description)
{
	return prototype(new Prototype(func, description), nargs);
}

bool Command::isSubCommand(Tokens tokens)
{
	for(auto sub : subcommands)
	{
		if(sub->getName() == tokens[tokens.getIndex()])
		{
			return true;
		}
	}
	return false;
}

Command& Command::addSub(string name)
{
	for(auto sub : subcommands)
	{
		if(sub->getName() == name)
		{
			//exception
		}
	}
	Command *sub = new Command(name, this);
	subcommands.push_back(sub);
	return *sub;
}

Command& Command::sub(string name)
{
	for(auto sub : subcommands)
	{
		if(sub->getName() == name)
		{
			return *sub;
		}
	}
	Command& sub = addSub(name);
	return sub;
}

void *Command::call(Args args)
{
	if(isSubCommand(args))
	{
		//cout << getFullName() + ": subcommand: " + args.getCurrent() << endl;
		return sub(args.getToken()).call(args);
	}
	int nargs = args.count() - args.getIndex();
	if(prototypes.find(nargs) != prototypes.end())
	{
		//cout << getFullName() + ": constant call" << endl;
		if(nargs)
		{
			args.removeSurrounded("[");
			args.freeLock();
		}
		return prototypes[nargs]->call(args);
	}
	return nullptr;
}

void *Command::operator()(Args args)
{
	return call(args);
}

ostream& operator<<(ostream& out, const Command& cmd)
{
	out << cmd.name << endl;
	for(auto pr : cmd.prototypes)
	{
		out << "\tArguments: " << pr.first << " -> " << *pr.second << endl;
	}
	for(auto sub : cmd.subcommands)
	{
		out << "sub =>" << endl << *sub << endl;
	}
	return out;
}
