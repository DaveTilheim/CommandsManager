#include "Command.hpp"

map<string, Command *> Command::commands = map<string, Command *>();
const int Command::DYNAMIC = -1;
const int Command::EXIT_TERMINAL = -3;
const string Command::std_help_name = "help";
const string Command::std_list_name = "list";
const string Command::std_exit_name = "exit";
const int Command::IGNORE_CMD = -2;

//init help command
Command Command::std_help_cmp = Command(Command::std_help_name,
[](Command& self)
{
	self.setDescription("Display information about commands (name, description, prototypes, sub commands)");
	self.func(F
	{
		Command::helpAll();
		return 0;
	}).func(F
	{
		if(Command::commands.find(args[args.getIndex()]) == Command::commands.end())
		{
			throw CommandException("no info for '" + args[args.getIndex()] + "'");
		}
		Command *c = Command::commands[(string)args];
		try
		{
			vector<string> cs;
			while(not args.end()) //help sub commands
			{
				cs.push_back(c->name);
				c = &c->getSub(args);
			}
			cout << endl;
			for(auto it : cs) cout << it << " ";
			c->help();
			cout << endl;
		}
		catch(const CommandException& e)
		{
			cout << e.getMsg() << endl;
		}
		return 0;
	}, Command::DYNAMIC);
});


Command Command::std_list_cmp = Command(Command::std_list_name,
[](Command& self)
{
	self.setDescription("Display a list of commands\nlist() display the name and the prototypes of the commands");
	self.func(F
	{
		for(auto it : Command::commands)
		{
			it.second->list();
		}
		cout << endl;
		return 0;
	});
	self.sub("name").func(F
	{
		cout << endl;
		for(auto it : Command::commands)
		{
			cout << it.second->name << endl;
			it.second->listSubs(it.second->name);
		}
		cout << endl;
		return 0;
	}).setDescription("Display the name of all the existing commands");
});

Command Command::std_exit_cmp = Command(Command::std_exit_name,
[](Command& self)
{
	self.setDescription("Exit the terminal");
	self.func(F
	{
		cout << endl << "Bye!" << endl << endl;
		return Command::EXIT_TERMINAL;
	});
});



Command::Command(string name, function<void(Command&)> init, char root)
{
	this->name = Tokens(name)[0];
	if(root)
	{
		Command::commands[this->name] = this;
	}
	init(*this);
}

Command::Command(string name, char root)
{
	this->name = Tokens(name)[0];
	if(root)
	{
		Command::commands[this->name] = this;
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

Command& Command::getSub(string name) noexcept(false)
{
	if(subCommands)
	{
		if(subCommands->find(name) != subCommands->end())
		{
			return *(*subCommands)[name];
		}
		throw SubCommandNotExistsException(name, this->name, *subCommands);
	}
	throw CommandException("no subCommands for '" + this->name + "' command");
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
			if(not functions)
			{
				throw SubCommandNotExistsException(subname, name, *subCommands);
			}
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
		else
		{
			if(functions->find(-1) != functions->end()) //unknown number of params
			{
				return (*functions)[-1](args);
			}
		}
		throw BadCommandParametersException(name, i, *functions);
	}
	throw NotAFunctionalCommandException(name);
}

void Command::list(string root) const
{
	if(root.size())
		cout << "\n\033[4m" + root + " " + name + "\033[0m\n";
	else
		cout << "\n\033[4m" + name + "\033[0m\n";
	listFuncs(root);
	if(subCommands)
	{
		for(auto s : *subCommands)
		{
			if(root.size())
				s.second->list(root +  " " + name);
			else
				s.second->list(name);
		}
	}
}

void Command::listFuncs(string root) const
{
	if(functions)
	{
		for(auto f : *functions)
		{
			if(f.first == -1)
			{
				if(root.size())
					cout << root << " " << name << "(...)" << endl;
				else
					cout << name << "(...)" << endl;
			}
			else
			{
				if(root.size())
					cout << root << " " << name << "(";
				else
					cout << name << "(";
				int n = 0, i;
				for(i = 0; i < f.first-1; i++) cout << "arg" << n++ << ", ";
				if(i == f.first-1) cout << "arg" << n;
				cout << ")" << endl;
			}
		}
	}
	else
	{
		cout << "can not be called" << endl;
	}
}

void Command::listSubs(string root) const
{
	if(subCommands)
	{
		for(auto s : *subCommands)
		{
			if(s.second->subCommands)
			{
				if(s.second->functions)
				{
					cout << root << " " << s.first << endl;
				}
				s.second->listSubs(root + " " + s.first);
			}
			else cout << root << " " << s.first << endl;
		}
	}
}

void Command::help(int tabs) const
{
	for(int i = 0; i < tabs; i++) cout << "\t";
	cout << "\033[4m" << name << "\033[0m > " << endl;
	helpDescription(tabs + 1);
	cout << endl;
	helpFuncs(tabs + 1);
	cout << endl;
	helpSubs(tabs + 1);
}

void Command::helpDescription(int tabs) const
{
	string tabs_s = "";
	for(int i = 0; i < tabs; i++) tabs_s += "\t";
	cout << tabs_s;
	cout << "\033[7m Description of \033[3m'" << name << "'\033[0m"<< endl;
	Tokens tmp_d(description, '\n');
	while(not tmp_d.end())
	{
		cout << tabs_s << (string)tmp_d << endl;
	}
}

void Command::helpSubs(int tabs) const
{
	int i;
	for(i = 0; i < tabs; i++) cout << "\t";
	cout << "\033[7m Sub commands of \033[3m'" << name << "'\033[0m"<< endl; 
	if(subCommands)
	{
		for(auto it : *subCommands)
		{
			it.second->help(tabs);
		}
	}
	else
	{
		for(i = 0; i < tabs; i++) cout << "\t";
		cout << "no sub commands" << endl;
	}
}

void Command::helpFuncs(int tabs) const
{
	for(int i = 0; i < tabs; i++) cout << "\t";
	cout << "\033[7m Prototypes of \033[3m'" << name << "' \033[0m"<< endl;
	if(functions)
	{
		int n;
		for(auto it : *functions)
		{
			switch(it.first)
			{
				int i;
				case Command::DYNAMIC:
					for(i = 0; i < tabs; i++) cout << "\t";
					cout << name << "(...)" << endl;
					break;

				default:
					for(i = 0; i < tabs; i++) cout << "\t";
					cout << name << "(";
					n = 0;
					for(i = 0; i < it.first-1; i++) cout << "arg" << n++ << ", ";
					if(i == it.first-1) cout << "arg" << n;
					cout << ")" << endl;
					break;
			}
		}
	}
	else
	{
		for(int i = 0; i < tabs; i++) cout << "\t";
		cout << "no prototypes" << endl;
	}
}

void Command::setDescription(string d)
{
	description = d;
}

string Command::getDescription() const
{
	return description;
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
	if(command.count())
	{
		return Command::exe(command);
	}
	return Command::IGNORE_CMD;
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
		catch(const CommandException& e)
		{
			cout << e.getMsg() << endl;
		}
	}
	return nbCommands;
}

void Command::helpAll()
{
	cout << endl;
	cout << "\033[7m Commands \033[0m"<< endl << endl; 
	for(auto it : Command::commands)
	{
		it.second->help();
		cout << endl << endl;
	}
	cout << endl;
}

const map<string, Command *>& Command::getCommands()
{
	return Command::commands;
}


