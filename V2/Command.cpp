#include "Command.hpp"


map<string, Command *> Command::armedCommands = map<string, Command *>();
string Command::lastResult = "";
vector<string> Command::commandFileBuffer = vector<string>();
int Command::currentFileIndex = 0;

Command::Command(string name, const Command *super) : super(super), name(name)
{

}

Command::Command(string name, Command& cmd, int n, const Command *super) : super(super), name(name)
{
	Prototype *proto = new Prototype(cmd.getProto(n));
	prototypes[n] = proto;
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

const Prototype& Command::getProto(int i)
{
	return *prototypes[i];
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

bool Command::isCommand(Tokens tokens)
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

vector<Command *>& Command::subs()
{
	return subcommands;
}

int Command::getMaxNargs() const
{
	if(prototypes.find(UNDEFINED) != prototypes.end())
	{
		return -1;
	}
	int ret = 0;
	for(auto p : prototypes)
	{
		ret = p.first;
	}
	return ret;
}

string Command::Tabs(int n)
{
	string out;
	for(int i = 0; i < n; i++)
		out += "\t";
	return out;
}

string Command::call(Args& args, string status) noexcept(false)
{
	static int tabs = 0;
	tabs++;
	//cout << Tabs(tabs-1) << "CALL " + getFullName() + "(" << args.partial() << ")" << endl;
	//cout << Tabs(tabs-1) << "STATUS: " << status << endl;
	if(status.find('r') != -1)
	{
		status = status.substr(1);
	}
	try
	{
		string nextStatus = "";
		if(not args.end())
		{
			nextStatus = getCallStatus(args.getTokens()[args.getIndex()]);
			if(nextStatus.find('r') == -1 and isSubCommand(args))
			{
				//cout << Tabs(tabs-1) << "SUB: " << args.getCurrent() << endl;
				if(not nextStatus.size()) //nargs lock
				{
					nextStatus = "";
				}
				Tokens tmp = args;
				try
				{
					return sub(args.getToken()).call(args, nextStatus);
				}
				catch(const CommandException& e)
				{
					tabs--;
					args = tmp;
					if(isCommand(args))
					{
						if(nextStatus.size())
							args.getTokens()[args.getIndex()] += ":" + nextStatus;
						//cout << Tabs(tabs-1) << "SUB FAIL => try command" << endl;
					}
					else
					{
						tabs = 0;
						throw e;
					}
				}
			}
			else
			{
				if(nextStatus.size())
					args.getTokens()[args.getIndex()] += ":" + nextStatus.substr(nextStatus.find('r') + 1);
			}
		}
		
		int maxNargs;
		if(status.size())
		{
			maxNargs = atoi(status.c_str());
			if(prototypes.find(maxNargs) == prototypes.end())
			{
				if(prototypes.find(-1) == prototypes.end())
				{
					tabs = 0;
					throw CommandException("Bad prototype call: " + to_string(maxNargs));
				}
			}
		}
		else
		{
			maxNargs = getMaxNargs();
		}
		string buff;
		Tokens finalTokens;
		//cout << Tabs(tabs-1) << "MAX NARGS: " << maxNargs << endl;
		if(not args.end())
		{
			for(int i = 0; (i < maxNargs or maxNargs == UNDEFINED) and not args.end(); i++)
			{
				string nextStatus = getCallStatus(args.getTokens()[args.getIndex()]);
				if(isCommand(args))
				{
					//cout << Tabs(tabs-1) << "CMD: " << args.getCurrent() << endl;
					string ret = Command::armedCommands[args]->call(args, nextStatus);
					finalTokens.getTokens().push_back(ret);
					//cout << Tabs(tabs-1) << "RET: " << ret << endl;
				}
				else
				{
					if(not args.end())
					{
						if(nextStatus.size())
						{
							args.getTokens()[args.getIndex()] += ":" + nextStatus;
						}
						//cout << Tabs(tabs-1) << "CONST: " << args.getCurrent() << endl;
						finalTokens.getTokens().push_back(args);
					}
					else
						break;
				}
			}
		}
		int nargs = finalTokens.count();
		//cout << Tabs(tabs-1) << "NARGS_1: " << nargs << endl;
		if(status.size() and nargs != maxNargs and maxNargs != UNDEFINED)
		{
			tabs = 0;
			throw CommandException("Bad number of arguments: " + to_string(nargs) + " expected " + to_string(maxNargs));
		}
		if(nargs > maxNargs)
		{
			finalTokens << args.partial(maxNargs);
			if(maxNargs != UNDEFINED)
				nargs = maxNargs;
		}
		else
		{
			finalTokens << args;
		}
		//cout << Tabs(tabs-1) << "NARGS_2: " << nargs << endl;
		if(prototypes.find(nargs) != prototypes.end()) //args number match
		{
			//cout << Tabs(tabs-1) << "PROTO: " << nargs << endl;
			if(nargs != NONE)
			{
				finalTokens.removeSurrounded("'");
				finalTokens.freeLock();
			}
			finalTokens.getTokens().erase(finalTokens.getTokens().begin() + nargs, finalTokens.getTokens().begin() + finalTokens.count());
			//cout << Tabs(tabs-1) << "EXE: " << getFullName() << "(" <<  finalTokens << ")" << endl;
			if(super)tabs--;
			tabs--;
			return prototypes[nargs]->call(finalTokens);
		}
		else
		{
			if(nargs and prototypes.find(UNDEFINED) != prototypes.end()) //undefined args number
			{
				if(status.size())
				{
					int n = atoi(status.c_str());
					if(n != -1)
						finalTokens = finalTokens.partial(n);
				}
				//cout << Tabs(tabs-1) << "PROTO: UNDEFINED" << endl;
				finalTokens.removeSurrounded("'");
				finalTokens.freeLock();
				//cout << Tabs(tabs-1) << "EXE: " << getFullName() << "(" <<  finalTokens << ")" << endl;
				if(super)tabs--;
				tabs--;
				return prototypes[UNDEFINED]->call(finalTokens);
			}
			else
			{
				tabs = 0;
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
	out << cmd.getFullName() << endl;
	for(auto pr : cmd.prototypes)
	{
		out << "\tArguments: " << pr.first << " -> " << *pr.second << endl;
	}
	for(auto sub : cmd.subcommands)
	{
		out << endl << *sub << endl;
	}
	return out;
}

string Command::getCallStatus(string& cs)
{
	if(cs.find(':') != -1 and cs.find('\e') == -1)
	{
		string status = cs.substr(cs.find(':') + 1);
		cs.erase(cs.size() - status.size() - 1);
		return status;
	}

	return "";
}

//static
string Command::exe(string scommand) noexcept(false)
{
	Tokens tcommand(scommand);
	//cout << tcommand << endl;
	if(tcommand.count())
	{
		string buf;
		while(not tcommand.end())
		{
			string& com = tcommand.getTokens()[tcommand.getIndex()];
			string status = getCallStatus(com);
			if(Command::armedCommands.find(com) != Command::armedCommands.end()) //command exists
			{
				buf += Command::armedCommands[tcommand]->call(tcommand, status) + " ";
			}
			else
			{
				throw CommandException("Not a command: " + com);
			}
		}
		if(buf.size())
		{
			buf.erase(buf.size() - 1);
		}
		lastResult = buf;
		return buf;
	}
	
	return "";
}

string Command::exeInput() noexcept(false)
{
	string buf;
	getline(cin, buf);
	return Command::exe(buf);
}

void Command::exeFile(string filename, function<bool(string& cmd)> preproc)
{
	ifstream file(filename);
	vector<string>& commands = Command::commandFileBuffer;
	if(file.is_open())
	{
		string command;
		while(getline(file, command))
		{
			if(preproc(command))
				commands.push_back(command);
		}
		file.close();
	}
	else
	{
		cerr << filename << " can not be opened" << endl;
		return;
	}
	try
	{
		for(currentFileIndex = 0; currentFileIndex < commands.size(); currentFileIndex++)
		{
			try
			{
				Command::exe(commands[currentFileIndex]);
			}
			catch(const IgnoreException& e)
			{

			}
		}
	}
	catch(const string& msg)
	{
		cout << msg << endl;
	}
	catch(const CommandException& err)
	{
		cout << err.what() << endl;
	}
}

vector<string>& Command::getFileBuffer()
{
	return commandFileBuffer;
}

int Command::getFileIndex()
{
	return currentFileIndex;
}

void Command::setFileIndex(int i)
{
	currentFileIndex = i - 1;
	if(currentFileIndex < 0) currentFileIndex = -1;
	else if(currentFileIndex >= commandFileBuffer.size()) currentFileIndex = commandFileBuffer.size() - 2;
}


map<string, Command *>& Command::getAll()
{
	return armedCommands;
}


void Command::printNames() const
{
	cout << getFullName() << endl;
	for(auto s : subcommands)
	{
		s->printNames();
	}
}

string Command::getLastResult()
{
	return lastResult;
}

