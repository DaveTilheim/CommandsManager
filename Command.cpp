#include "Command.hpp"

map<string, Command *> Command::commands = map<string, Command *>();
const int Command::EXIT_TERMINAL = -3;
const string Command::std_help_name = "help";

//init help command
Command Command::std_help_cmp = Command(Command::std_help_name,
[](Command& self)
{
	self.func(F
	{
		Command::showAll();
		return 0;
	}).func(F
	{
		if(Command::commands.find(args[0]) == Command::commands.end())
		{
			throw CommandException("no information about '" + args[0] + "'");
		}
		cout << endl;
		Command::commands[args[0]]->show();
		cout << endl;
		return 0;
	}, 1);
});

Command::Command(string name, function<void(Command&)> init, char root) : name(name)
{
	if(root)
	{
		Command::commands[name] = this;
	}
	init(*this);
}

Command::Command(string name, char root) : name(name)
{
	if(root)
	{
		Command::commands[name] = this;
	}
}

Command::~Command()
{
	if(Command::commands.find(name) != Command::commands.end())
	{
		Command::commands.erase(name);
	}
	if(functions)
	{
		delete functions;
	}
	if(subCommands)
	{
		for(auto it : *subCommands)
		{
			delete it.second;
		}
	}
}

Command& Command::sub(string name)
{
	if(not subCommands)
	{
		subCommands = new map<string, Command *>();
	}
	if(subCommands->find(name) == subCommands->end())
	{
		(*subCommands)[name] = new Command(name, false);
	}
	return *(*subCommands)[name];
}

Command& Command::getSub(string name)
{
	return *(*subCommands)[name];
}

Command& Command::func(function<int(const Tokens&)> f, int params)
{
	if(not functions)
	{
		functions = new map<int, function<int(const Tokens&)>>();
	}
	(*functions)[params] = f;
	return *this;
}

int Command::run() noexcept(false)
{
	if(functions)
	{
		if(functions->find(0) != functions->end())
		{
			return (*functions)[0](Tokens());
		}
		throw BadCommandParametersException(name, 0, *functions);
	}
	throw NotAFunctionalCommandException(name);
}

int Command::run(Tokens& args) noexcept(false)
{
	if(subCommands)
	{
		string subname = args[args.getIndex()];
		if(subname.size())
		{
			if(subCommands->find(subname) != subCommands->end())
			{
				args.pop();
				return (*subCommands)[args]->run(args);
			}
			throw SubCommandNotExistsException(subname, name, *subCommands);
		}
	}
	if(functions)
	{
		args.pop();
		int i = args.count();
		if(functions->find(i) != functions->end())
		{
			return (*functions)[i](args);
		}
		throw BadCommandParametersException(name, i, *functions);
	}
	throw NotAFunctionalCommandException(name);
}

void Command::show(int tabs) const
{
	for(int i = 0; i < tabs; i++) cout << "\t";
	cout << "\033[4m" << name << "\033[0m > ";
	if(functions)
	{
		for(auto it : *functions)
		{
			cout << it.first << "   ";
		}
	}
	else
	{
		cout << "do nothing";
	}
	cout << endl;
	if(subCommands)
	{
		for(auto it : *subCommands)
		{
			it.second->show(tabs+1);
		}
	}
}

int Command::exe(string name, Tokens& args)
{
	if(Command::commands.find(name) != Command::commands.end())
	{
		return Command::commands[name]->run(args);
	}
	throw CommandNotExistsException(name, Command::commands);
}

int Command::exe(Tokens& command)
{
	string name = command;
	if(Command::commands.find(name) != Command::commands.end())
	{
		return Command::commands[name]->run(command);
	}
	throw CommandNotExistsException(name, Command::commands);
}

Tokens Command::input()
{
	string buff;
	getline(cin, buff);
	return Tokens(buff);
}

int Command::prompt()
{
	Tokens command = Command::input();
	return Command::exe(command);
}

int Command::prompt(string out)
{
	cout << out;
	return Command::prompt();
}

int Command::terminal(string out)
{
	int nbCommands = 0;
	int ret = 0;
	while(ret != Command::EXIT_TERMINAL)
	{
		try
		{
			ret = Command::prompt(out);
			nbCommands++;
		}
		catch(const NotAFunctionalCommandException& e)
		{
			cout << e.getMsg() << endl;
		}
		catch(const BadCommandParametersException& e)
		{
			cout << e.getMsg() << endl;
		}
		catch(const CommandNotExistsException& e)
		{
			cout << e.getMsg() << endl;
		}
		catch(const SubCommandNotExistsException& e)
		{
			cout << e.getMsg() << endl;
		}
		catch(const CommandException& e)
		{
			cout << e.getMsg() << endl;
		}
	}
	return nbCommands;
}

void Command::showAll()
{
	cout << endl;
	for(auto it : Command::commands)
	{
		it.second->show();
		cout << endl;
	}
	cout << endl;
}
