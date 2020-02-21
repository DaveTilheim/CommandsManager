#include "StdCommand.hpp"


Command StdCommand::ccreate = Command("create");
Command StdCommand::cread = Command("read");
Command StdCommand::cupdate = Command("update");
Command StdCommand::ctype = Command("type");
Command StdCommand::cadd = Command("add");
Command StdCommand::csub = Command("sub");
Command StdCommand::cmul = Command("mul");
Command StdCommand::cdiv = Command("div");
Command StdCommand::cresult = Command("result");
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

inline double add(double v1, double v2)
{
	return v1 + v2;
}

inline double sub(double v1, double v2)
{
	return v1 - v2;
}

inline double mul(double v1, double v2)
{
	return v1 * v2;
}

inline double div_(double v1, double v2)
{
	return v1 / v2;
}

string operation(string val1, string val2, function<double(double, double)> callback)
{
	string type1 = Memory::type(val1);
	string type2 = Memory::type(val2);
	if(type1 == "Integer")
	{
		int i1 = atoi(val1.c_str());
		if(type2 == "Integer")
		{
			int i2 = atoi(val2.c_str());
			return to_string((int)callback(i1, i2));
		}
		else if(type2 == "Float")
		{
			double f2 = atof(val2.c_str());
			return to_string(callback(i1, f2));
		}
	}
	else if(type1 == "Float")
	{
		double f1 = atof(val1.c_str());
		if(type2 == "Integer")
		{
			int i2 = atoi(val2.c_str());
			return to_string(callback(f1, i2));
		}
		else if(type2 == "Float")
		{
			double f2 = atof(val2.c_str());
			return to_string(callback(f1, f2));
		}
	}
	throw CommandException("operation between " + type1 + " and " + type2 + " is not available");
}

string StdCommand::add_command(Args args)
{
	string a1 = args;
	string a2 = args;
	if(root.contains(a1)) a1 = read_command(a1);
	if(root.contains(a2)) a2 = read_command(a2);
	return operation(a1, a2, add);
}

string StdCommand::sub_command(Args args)
{
	string a1 = args;
	string a2 = args;
	if(root.contains(a1)) a1 = read_command(a1);
	if(root.contains(a2)) a2 = read_command(a2);
	return operation(a1, a2, sub);
}

string StdCommand::mul_command(Args args)
{
	string a1 = args;
	string a2 = args;
	if(root.contains(a1)) a1 = read_command(a1);
	if(root.contains(a2)) a2 = read_command(a2);
	return operation(a1, a2, mul);
}

string StdCommand::div_command(Args args)
{
	string a1 = args;
	string a2 = args;
	if(root.contains(a1)) a1 = read_command(a1);
	if(root.contains(a2)) a2 = read_command(a2);
	return operation(a1, a2, div_);
}

string StdCommand::result_command(Args args)
{
	return Command::getLastResult();
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
	cadd.proto(add_command, 2);
	csub.proto(sub_command, 2);
	cmul.proto(mul_command, 2);
	cdiv.proto(div_command, 2);
	cresult.proto(result_command, 0);
	

	cread.arm();
	ccreate.arm();
	cupdate.arm();
	ctype.arm();
	cadd.arm();
	cdiv.arm();
	cmul.arm();
	csub.arm();
	cresult.arm();
}
