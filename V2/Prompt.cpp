#include "Prompt.hpp"



string Prompt::nlstr = "> ";
Command Prompt::cexit = Command("exit");
Command Prompt::chelp = Command("help");
Command Prompt::csys = Command("sys");
Command Prompt::cnlstr = Command("nlstr");


string command_handler::exit(Args args)
{
	Prompt::exit();
	return "";
}

string command_handler::help0(Args args)
{
	for(auto c : Command::getAll())
	{
		cout << *c.second << endl;
	}
	return "";
}

string command_handler::helpU(Args args)
{
	string name = args;
	if(Command::getAll().find(name) == Command::getAll().end())
	{
		throw CommandException(name + " is not a command");
	}
	Command* super = Command::getAll()[name];
	while(not args.end())
	{
		string cs = args;
		if(super->isSubCommand(cs))
			super = &super->sub(cs);
		else
			throw CommandException(cs + " is not a sub command of " + super->getFullName());
	}
	cout << *super << endl;
	return "";
}

string command_handler::help_name(Args args)
{
	for(auto c : Command::getAll())
	{
		c.second->printNames();
	}
	return "";
}

string command_handler::sys(Args args)
{
	string scmd = "";
	while(not args.end())
	{
		scmd += (string) args + args.getSeparator();
	}
	if(scmd.size())
	{
		scmd.erase(scmd.size() - 1);
		system(scmd.c_str());
	}
	
	return scmd;
}

string command_handler::sys_result(Args args)
{
	string scmd = "";
	while(not args.end())
	{
		scmd += (string) args + args.getSeparator();
	}
	string ret = "";
	if(scmd.size())
	{
		scmd.erase(scmd.size() - 1);
		FILE *cmdr = popen(scmd.c_str(), "r");
		if(cmdr)
		{
			char buf[128] = {0};
			while(fgets(buf, 127, cmdr) != NULL)
			{
				ret += buf;
			}
			pclose(cmdr);
		}
	}
	if(ret.find('\n') != -1)
	{
		ret.erase(ret.size() - 1);
	}
	return ret;
}

string command_handler::nlstr(Args args)
{
	if(args[0].size())
		Prompt::nlstr = (string)args;
	return Prompt::nlstr;
}

void Prompt::init()
{
	cexit
	.proto(command_handler::exit, 0, "exit the prompt");

	chelp
		.proto(command_handler::help0, 0, "print all command descriptions");
	
	chelp
		.proto(command_handler::helpU, UNDEFINED, "print specified command descriptions");

	chelp
		.sub("names")
			.proto(command_handler::help_name, 0, "print the names of all the commands");

	csys
		.proto(command_handler::sys, UNDEFINED, "executes a system terminal command")
		.sub("result")
			.proto(command_handler::sys_result, UNDEFINED, "executes a system terminal command and return it");

	cnlstr
		.proto(command_handler::nlstr, 1, "change the hand catch prompt string");

	cexit.arm();
	chelp.arm();
	csys.arm();
	cnlstr.arm();
}

int Prompt::run(string nlstr)
{
	Prompt::nlstr = nlstr;
	init();
	int cmdCounter = 0;
	while(true)
	{
		cout << Prompt::nlstr;
		try
		{
			string output = Command::exeInput();
			if(output.size())
			{
				cout << "> " + output << endl;
			}
			cmdCounter++;
		}
		catch(const CommandException& err)
		{
			cout << err.what() << endl;
		}
		catch(const PromptException& err)
		{
			cout << "exit" << endl;
			break;
		}
	}
	return cmdCounter;
}

void Prompt::exit() noexcept(false)
{
	throw PromptException();
}

void Prompt::exeFile(string filename)
{
	ifstream file(filename);
	if(file.is_open())
	{
		string command;
		try
		{
			while(getline(file, command))
			{
				string output = Command::exe(command);
			}
		}
		catch(const CommandException& err)
		{
			cout << err.what() << endl;
		}
		file.close();
	}
}
