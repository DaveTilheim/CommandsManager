#include "Command.hpp"


map<string, Command *> Command::armedCommands = map<string, Command *>();


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
		return super->getFullName() + "." + name;
	return getName();
}

Command& Command::proto(Prototype *proto, int nargs)
{
	if(prototypes.find(nargs) != prototypes.end())
	{
		delete prototypes[nargs];
	}
	prototypes[nargs] = proto;
	return *this;
}

Command& Command::proto(function<string(Args)> func, int nargs, string description)
{
	return proto(new Prototype(func, description), nargs);
}

Command& Command::proto(function<string(Args)> func, string form, string description)
{
	Tokens tmp(form);
	int args;
	if(tmp[tmp.count() - 1] == "...")
	{
		args = -1;
	}
	else
	{
		args = tmp.count() - 1; // - command name
	}
	return proto(new Prototype(func, description), args);
}

bool Command::isSubCommand(Tokens tokens) const
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

bool Command::isCommand(Tokens tokens) const
{
	for(auto com : Command::armedCommands)
	{
		if(com.second->getName() == tokens[tokens.getIndex()])
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
	return addSub(name);
}

Tokens Command::copyNeededTokens(const Tokens& tokens) const
{
	if(tokens.end())
	{
		return Tokens("");
	}
	string buff = tokens.getCurrent();
	if(buff.size())
	{
		if(buff[buff.size() - 1] == '|')
		{
			return Tokens(buff.substr(0, buff.size() - 1));
		}
	}
	int i = tokens.getIndex() + 1;
	while(i < tokens.count())
	{
		if(tokens[i].size() > 1 and tokens[i][tokens[i].size() - 1] == '|')
		{
			buff += tokens.getSeparator() + tokens[i].substr(0, tokens[i].size() - 1);
			break;
		}
		else
		{
			buff += tokens.getSeparator() + tokens[i];
		}
		i++;
	}
	return Tokens(buff);
}

string Command::call(Args args)
{
	cout << "CALL " + getFullName() + "(" << args.partial() << ")" << endl;;
	try
	{
		if(isSubCommand(args))
		{
			return sub(args.getToken()).call(args);
		}
		
		string buff;
		//trouver un nouvel algorithme qui prélève les tokens en fonction du nombre de params (recursif)
		while(not args.end())
		{
			if(isCommand(args))
			{
				string command = args;
				Tokens needtok = Command::armedCommands[command]->copyNeededTokens(args);
				args.setIndex(args.getIndex() + needtok.count());
				string ret = Command::armedCommands[command]->call(needtok) + " ";
				cout << "RET: " << ret + " => " + getFullName() << endl;
				buff += ret;
			}
			else
			{
				buff += (string)args + " ";
			}
		}
		for(int i = args.getIndex(); i < args.count(); i++)
			buff += args[i] + " ";
		if(buff.size())
		{
			buff.erase(buff.size() - 1);
		}
		cout << "ARGS: " << buff << endl;
		args = Tokens(buff);
		
		
		int nargs = args.count() - args.getIndex();
		if(prototypes.find(nargs) != prototypes.end()) //args number match
		{
			if(nargs != NONE)
			{
				args.removeSurrounded("[");
				args.freeLock();
			}
			return prototypes[nargs]->call(args);
		}
		else
		{
			if(nargs and prototypes.find(UNDEFINED) != prototypes.end()) //undefined args number
			{
				args.removeSurrounded("[");
				args.freeLock();
				return prototypes[UNDEFINED]->call(args);
			}
			else
			{
				throw CommandException("Bad prototype call: " + to_string(nargs));
			}
		}
	}
	catch(const CommandException& err)
	{
		throw CommandException(getFullName() + " => " + err.what());
	}
}

Command& Command::arm()
{
	Command::armedCommands[name] = this;
	return *this;
}

string Command::operator()(Args args)
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

//static
string Command::exe(string scommand)
{
	Tokens tcommand(scommand);
	if(tcommand.count())
	{
		if(Command::armedCommands.find(tcommand.getCurrent()) != Command::armedCommands.end()) //command exists
		{
			try
			{
				return Command::armedCommands[tcommand]->call(tcommand);
			}
			catch(const CommandException& err)
			{
				cout << err.what() << endl;
			}
		}
	}
	
	return "(null)";
}




