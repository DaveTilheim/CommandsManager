#include "StdCommand.hpp"


Command StdCommand::ccreate = Command("create");
Command StdCommand::cread = Command("read");
Command StdCommand::cprint = Command("print");
Command StdCommand::cupdate = Command("update");
Command StdCommand::ctype = Command("type");
Command StdCommand::cadd = Command("add");
Command StdCommand::csub = Command("sub");
Command StdCommand::cmul = Command("mul");
Command StdCommand::cdiv = Command("div");
Command StdCommand::cresult = Command("result");
Command StdCommand::crepeat = Command("repeat");
Command StdCommand::cinput = Command("input");
Command StdCommand::cstr = Command("str");
Node StdCommand::root = Node();


void erase_indentifier(string& target)
{
	if(target[target.size() -1] == '\a')
	{
		target.erase(target.size() - 1);
	}
}

string StdCommand::create_command(Args args)
{
	Token id = args;
	Token value = args;
	if(root.contains(value))
	{
		if(root.readMemory(value).getType() == "Vector")
		{
			args.setIndex(0);
			return create_vector_command(args);
		}
		value = read_command(value);
	}
	erase_indentifier(value);
	root.addMemory(id, Memory::create(value));
	return id;
}

string StdCommand::create_vector_command(Args args)
{
	string id = args;
	VectorMemory *v = new VectorMemory();
	for(int i = args.getIndex(); i < args.count(); i++)
	{
		Token value = args[i];
		string type;
		string otherId;
		if(root.contains(value))
		{
			otherId = value;
			type = root.readMemory(value).getType();
			value = read_command(otherId);
		}
		else
		{
			erase_indentifier(value);
			type = Memory::type(value);
		}
		
		if(type == "Integer")
		{
			v->add(new Integer(value));
		}
		else if(type == "Float")
		{
			v->add(new Float(value));
		}
		else if(type == "String")
		{
			v->add(new String(value));
		}
		else if(type == "Vector")
		{
			v->add(new VectorMemory(*dynamic_cast<VectorMemory *>(&root.readMemory(otherId))));
		}
	}
	root.addMemory(id, v);
	return id;
}

string StdCommand::read_command(Args args)
{
	string id = args;
	Memory& mem = root.readMemory(id);
	return mem.reads();
}

string StdCommand::read_vector_command(Args args)
{
	string id = args;
	VectorMemory& vec = *dynamic_cast<VectorMemory *>(&root.readMemory(id));
	int index = args;
	return vec.get(index).reads();
}

string StdCommand::update_command(Args args)
{
	Token id = args;
	Token value = args;
	Memory& mem = root.readMemory(id);
	if(root.contains(value))
		value = read_command(value);
	erase_indentifier(value);
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
	else if(memType == "Vector")
	{
		Tokens toks = value;
		dynamic_cast<VectorMemory *>(&mem)->clear();
		while(not toks.end())
		{
			dynamic_cast<VectorMemory *>(&mem)->add(Memory::create(toks));
		}
	}
	return id;
}

string StdCommand::type_command(Args args)
{
	string expr = args;
	if(root.contains(expr))
		return root.readMemory(expr).getType();
	erase_indentifier(expr);
	return Memory::type(expr);
}

string StdCommand::print_command(Args args)
{
	for(int i = 0; i < args.count(); i++)
	{
		string elm = args[i];
		if(root.contains(elm))
			cout << read_command(elm);
		else
		{
			erase_indentifier(elm);
			cout <<elm;
		}
		if(i != args.count() - 1) cout << " ";
	}
	cout << endl;
	return "";
}

string StdCommand::print_memory_command(Args args)
{
	for(auto mem : root.memory)
	{
		cout << mem.first << " " << read_command(mem.first) << " " << type_command(mem.first) << endl;
	}
	return "";
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
	erase_indentifier(a1);
	if(root.contains(a2)) a2 = read_command(a2);
	erase_indentifier(a2);
	return operation(a1, a2, add);
}

string StdCommand::sub_command(Args args)
{
	string a1 = args;
	string a2 = args;
	if(root.contains(a1)) a1 = read_command(a1);
	erase_indentifier(a1);
	if(root.contains(a2)) a2 = read_command(a2);
	erase_indentifier(a2);
	return operation(a1, a2, sub);
}

string StdCommand::mul_command(Args args)
{
	string a1 = args;
	string a2 = args;
	if(root.contains(a1)) a1 = read_command(a1);
	erase_indentifier(a1);
	if(root.contains(a2)) a2 = read_command(a2);
	erase_indentifier(a2);
	return operation(a1, a2, mul);
}

string StdCommand::div_command(Args args)
{
	string a1 = args;
	string a2 = args;
	if(root.contains(a1)) a1 = read_command(a1);
	erase_indentifier(a1);
	if(root.contains(a2)) a2 = read_command(a2);
	erase_indentifier(a2);
	return operation(a1, a2, div_);
}

string StdCommand::result_command(Args args)
{
	return Command::getLastResult();
}

string StdCommand::repeat_command(Args args)
{
	int n = args;
	string cmd;
	if(args.count() <= 1) throw CommandException("has " + to_string(args.count()) + " args, expected 2 min");
	for(int i = args.getIndex(); i < args.count(); i++) cmd += args[i] + " ";
	cmd.erase(cmd.size()-1);
	for(int i = 0; i < n; i++)
	{
		Command::exe(cmd);
	}
	return to_string(n);
}

string StdCommand::input_command(Args args)
{
	string buff;
	getline(cin, buff);
	return buff;
}

string StdCommand::str_command(Args args)
{
	return (string)args + "\a";
}

void StdCommand::initStdCommands()
{
	Memory::addType("Integer");
	Memory::addType("Float");
	Memory::addType("String");
	Memory::addType("Vector");
	
	ccreate.proto(create_command, 2);
	ccreate.sub("vector").proto(create_vector_command, -1);
	cread.proto(read_command, 1);
	cread.sub("vector").proto(read_vector_command, 2);
	cprint.proto(print_command, -1);
	cprint.sub("memory").proto(print_memory_command, 0);
	cupdate.proto(update_command, 2);
	ctype.proto(type_command, 1);
	cadd.proto(add_command, 2);
	csub.proto(sub_command, 2);
	cmul.proto(mul_command, 2);
	cdiv.proto(div_command, 2);
	cresult.proto(result_command, 0);
	crepeat.proto(repeat_command, -1);
	cinput.proto(input_command, 0);
	cstr.proto(str_command, 1);
	

	cread.arm();
	ccreate.arm();
	cprint.arm();
	cupdate.arm();
	ctype.arm();
	cadd.arm();
	cdiv.arm();
	cmul.arm();
	csub.arm();
	cresult.arm();
	crepeat.arm();
	cinput.arm();
	cstr.arm();
}
