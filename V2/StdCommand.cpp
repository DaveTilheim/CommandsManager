#include "StdCommand.hpp"


Command StdCommand::cexit = Command("exit");
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
Command StdCommand::cif = Command("if");
Command StdCommand::cend = Command("end");
Command StdCommand::cbegin = Command("begin");
Command StdCommand::celse = Command("else");
Command StdCommand::cequal = Command("equal");
Command StdCommand::cjump = Command("jump");
Command StdCommand::clabel = Command("label");
Node StdCommand::root = Node(nullptr, "root");
int StdCommand::nodeCounter = 0;
map<string, int> StdCommand::labels = map<string, int>();


void erase_indentifier(string& target)
{
	if(target[target.size() -1] == '\a')
	{
		target.erase(target.size() - 1);
	}
}

string StdCommand::exit_command(Args args)
{
	if(args.end())
		throw CommandException("success");
	throw CommandException(args);
	return "error";
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
	auto hashMap = root.readAllMemory();
	for(auto mem : hashMap)
	{
		cout << mem.first << " " << mem.second->reads() << " " << mem.second->getType() << endl;
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

string StdCommand::if_command(Args args)
{
	string value = args;
	if(Float::isFloat(value) and atof(value.c_str()) == (double)0.0)
	{
		return "false";
	}
	if(Integer::isInteger(value) and atoi(value.c_str()) == (int)0)
	{
		return "false";
	}
	root.addNode();
	return "true";
}

string StdCommand::end_command_0(Args args)
{
	root.removeNode();
	return "";
}

string StdCommand::end_command_1(Args args)
{
	root.removeNode(args);
	return "";
}

string StdCommand::begin_command_0(Args args)
{
	root.addNode();
	return "";
}

string StdCommand::begin_command_1(Args args)
{
	root.addNode(args);
	return "";
}

string StdCommand::else_command(Args args)
{
	
	return "";
}

string StdCommand::jump_command(Args args)
{
	if(Integer::isInteger(args[0]))
	{
		int step = args;
		Command::setFileIndex(Command::getFileIndex() + step);
	}
	else
	{
		string label = args;
		if(labels.find(label) == labels.end())
		{
			auto& buffer = Command::getFileBuffer();
			for(int i = 0; i < buffer.size(); i++)
			{
				if(buffer[i].find(' ') != string::npos)
				{
					string cmd = buffer[i].substr(0, buffer[i].find(' '));
					string name = buffer[i].substr(buffer[i].find(' ')+1);
					if(cmd == "label" and name == label)
					{
						int tmp = Command::getFileIndex();
						Command::setFileIndex(i+1);
						Command::exe(buffer[i]);
						Command::setFileIndex(tmp);
						break;
					}
				}
			}
		}
		Command::setFileIndex(labels[label]);
	}
	return "";
}

string StdCommand::label_command(Args args)
{
	string name = args;
	if(labels.find(name) == labels.end())
	{
		//cout << "Label (" + name + ") set at instruction n°" << Command::getFileIndex() << endl;
		labels[name] = Command::getFileIndex();
		return name;
	}
	return "null";
}


string StdCommand::equal_command(Args args)
{
	string a1 = args;
	string a2 = args;
	if(root.contains(a1)) a1 = read_command(a1);
	erase_indentifier(a1);
	if(root.contains(a2)) a2 = read_command(a2);
	erase_indentifier(a2);
	return to_string(Memory::equal(a1, a2));
}

void StdCommand::initStdCommands()
{
	Memory::addType("Integer");
	Memory::addType("Float");
	Memory::addType("String");
	Memory::addType("Vector");
	
	cexit.proto(exit_command, -1);
	cexit.proto(exit_command, 0);
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
	cif.proto(if_command, 1);
	cend.proto(end_command_0, 0);
	cend.proto(end_command_1, 1);
	cbegin.proto(begin_command_0, 0);
	cbegin.proto(begin_command_1, 1);
	celse.proto(else_command, 0);
	cequal.proto(equal_command, 2);
	cjump.proto(jump_command, 1);
	clabel.proto(label_command, 1);

	cexit.arm();
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
	cif.arm();
	cend.arm();
	cbegin.arm();
	celse.arm();
	cequal.arm();
	cjump.arm();
	clabel.arm();
}
