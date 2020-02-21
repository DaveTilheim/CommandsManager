#include "StdCommand.hpp"


Command StdCommand::ccreate = Command("create");
Command StdCommand::cread = Command("read");
Node StdCommand::root = Node();


void StdCommand::initStdCommands()
{
	Memory::addType("Integer");
	Memory::addType("String");
	
	ccreate.proto([](Args args)
	{
		string type = "none";
		string name = args;
		string value = args;
		for(int i = 0; i < value.size(); i++)
		{
			if(not isdigit(value[i]))
			{
				type = "String";
			}
			else
			{
				type = "Integer";
			}
		}

		Memory *mem =nullptr;
		if(type == "Integer")
		{
			mem = new PrimitiveMemory(atoi(value.c_str()));
			mem->setType("Integer");
		}
		else
		{
			mem = new ObjectMemory((string)value);
			mem->setType("String");
		}
		
		
		root.addMemory(name, mem);
		return "";
	}, 2);

	cread.proto([](Args args)
	{
		string var = args;
		if(root.readMemory(var).getType() == "Integer")
		{
			return to_string(root.readMemory(var).read<int>());
		}
		return root.readMemory(var).read<string>();
	}, 1);

	cread.arm();
	ccreate.arm();
}
