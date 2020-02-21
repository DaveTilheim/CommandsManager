#include "StdCommand.hpp"


Command StdCommand::ccreate = Command("create");
Command StdCommand::cread = Command("read");
Command StdCommand::cupdate = Command("update");
Command StdCommand::ctype = Command("type");
Node StdCommand::root = Node();


string StdCommand::create_command(Args args)
{
	Token id = args;
	Token value = args;
	string type = Memory::type(value);
	if(type == "Integer")
	{
		root.addMemory(id, new Integer(value));
		return to_string(atoi(value.c_str()));
	}
	else if(type == "Float")
	{
		root.addMemory(id, new Float(value));
		return to_string(atof(value.c_str()));
	}
	else if(type == "String")
	{
		root.addMemory(id, new String(value));
	}
	return value;
}

string StdCommand::read_command(Args args)
{
	string id = args;
	Memory& mem = root.readMemory(id);
	string type = mem.getType();
	if(type == "Integer")
	{
		return to_string(mem.read<int>());
	}
	else if(type == "Float")
	{
		return to_string(mem.read<double>());
	}
	else if(type == "String")
	{
		return mem.read<string>();
	}
	return "null";
}

string StdCommand::update_command(Args args)
{
	Token id = args;
	Token value = args;
	Memory& mem = root.readMemory(id);
	string type = Memory::type(value);
	string memType = mem.getType();
	if(memType == "Integer")
	{
		if(type == "Float") mem = (int)atof(value.c_str());
		else if(type == "Integer") mem = atoi(value.c_str());
		else throw CommandException(value + " is " + type + ", " + memType + " expected");
		return to_string(atoi(value.c_str()));
	}
	else if(memType == "Float")
	{
		if(type == "Float") mem = atof(value.c_str());
		else if(type == "Integer") mem = (double)atoi(value.c_str());
		else throw CommandException(value + " is " + type + ", " + memType + " expected");
		return to_string(atof(value.c_str()));
	}
	else if(memType == "String")
	{
		mem = value;
	}
	return value;
}

string StdCommand::type_command(Args args)
{
	return root.readMemory(args).getType();
}

string StdCommand::read_memory_command(Args args)
{
	string buf;
	for(auto mem : root.memory)
	{
		cout << mem.first << " " << read_command(mem.first) << endl;
	}
	return buf;
}


void StdCommand::initStdCommands()
{
	Memory::addType("Integer");
	Memory::addType("Float");
	Memory::addType("String");
	
	ccreate.proto(create_command, 2);
	cread.proto(read_command, 1);
	cread.sub("memory").proto(read_memory_command, 0);
	cupdate.proto(update_command, 2);
	ctype.proto(type_command, 1);
	

	cread.arm();
	ccreate.arm();
	cupdate.arm();
	ctype.arm();
}
