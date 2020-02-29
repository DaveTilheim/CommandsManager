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
Command StdCommand::cless = Command("less");
Command StdCommand::cjump = Command("jump");
Command StdCommand::clabel = Command("label");
Command StdCommand::cignore = Command("ignore");
Command StdCommand::cwhile = Command("while");
Command StdCommand::cfunction = Command("function");
Command StdCommand::creturn = Command("return");
Command StdCommand::ccall = Command("call");
Command StdCommand::cret = Command("ret");
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
			VectorMemory *v = new VectorMemory(*dynamic_cast<VectorMemory *>(&root.readMemory(value)));
			root.addMemory(id, v);
			return id;
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
			VectorMemory& vec = *dynamic_cast<VectorMemory *>(&root.readMemory(otherId));
			v->add(new VectorMemory(vec));
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
	string value = args;
	if(root.contains(value))
		value = read_command(value);
	return vec.get(atoi(value.c_str())).reads();
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

bool ifSuccess = true;
string StdCommand::if_command(Args args)
{
	string value = args;
	if((Float::isFloat(value) and atof(value.c_str()) == (double)0.0) or (Integer::isInteger(value) and atoi(value.c_str()) == (int)0))
	{
		ifSuccess = false;
		int blockCounter = 1, i;
		for(i = Command::getFileIndex() + 1; i < Command::getFileBuffer().size(); i++)
		{
			Tokens cmd = Command::getFileBuffer()[i];
			if(cmd[0] == "if" or cmd[0] == "while") blockCounter++;
			else if(cmd[0] == "end")
			{
				blockCounter--;
				if(blockCounter == 0)
					break;
			}
			else if(cmd[0] == "else")
			{
				blockCounter--;
				if(blockCounter == 0)
				{
					i--;
					break;
				}
			}
		}
		Command::setFileIndex(i+1);
		return "false";
	}
	ifSuccess = true;
	begin_command_0(args);
	return "true";
}

string StdCommand::else_command(Args args)
{
	if_command(Tokens(to_string(not ifSuccess)));
	return "";
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

string StdCommand::jump_to_command(Args args)
{
	if(Integer::isInteger(args[0]))
	{
		int step = args;
		Command::setFileIndex(step);
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

string StdCommand::ignore_command(Args args) noexcept(false)
{
	throw IgnoreException();
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

string StdCommand::less_command(Args args)
{
	string a1 = args;
	string a2 = args;
	if(root.contains(a1)) a1 = read_command(a1);
	erase_indentifier(a1);
	if(root.contains(a2)) a2 = read_command(a2);
	erase_indentifier(a2);
	return to_string(Memory::less(a1, a2));
}

int whileDepth = 0;
map<int, int> whileMap;
string StdCommand::while_command(Args args)
{
	int curi = Command::getFileIndex();
	if_command(args);
	if(ifSuccess)
	{
		whileDepth++;
		whileMap[whileDepth] = Command::getFileIndex();
	}
	ifSuccess = true;
	return "";
}

string StdCommand::end_while_command(Args args)
{
	if(not whileDepth) throw string("not in while block");
	end_command_0(args);
	jump_to_command(Tokens(to_string(whileMap[whileDepth])));
	whileMap.erase(whileDepth);
	whileDepth--;
	return "";
}

struct __Function
{
	int location;
	int nargs;
	vector<string> args;
	__Function(){}
	__Function(int l, int n) : location(l), nargs(n){}
	__Function(const __Function&f) : location(f.location), nargs(f.nargs){}
};
map<string, __Function> functionMap;
vector<string> nodeStack;
string StdCommand::function_command(Args args)
{
	string fname = args;
	Args arguments = args.partial();
	if(functionMap.find(fname) != functionMap.end())
	{
		const __Function& func = functionMap[fname];
		if(arguments.count() != func.args.size()) throw CommandException(to_string(func.args.size()) + " gave but expected " + to_string(func.nargs));
		begin_command_0(args);
		nodeStack.push_back(root.getCurrentName());
		for(int i = 0; i < func.nargs; i++)
		{
			Tokens toks(arguments[i]);
			toks.getTokens().push_back(func.args[i]);
			//cout << "create " + arguments[i] + " " + func.args[i] << endl;
			create_command(toks);
		}
	}
	else
	{
		functionMap[fname] = __Function(Command::getFileIndex(), arguments.count());
		int i;
		for(i = Command::getFileIndex(); i < Command::getFileBuffer().size(); i++)
		{
			if(Command::getFileBuffer()[i] == "end function" and Command::getFileBuffer()[i].size() == string("end function").size())
			{
				break;
			}
		}
		Command::setFileIndex(i+1);
	}
	return "";
}

vector<string> returnValues;
string StdCommand::return_command(Args args)
{
	string ret = args;
	if(root.contains(ret))
		ret = read_command(ret);
	returnValues.push_back(ret);
	end_function_command(args);
	return "";
}

vector<int> callLocations;
string StdCommand::call_command(Args args)
{
	string fname = args;
	if(functionMap.find(fname) == functionMap.end()) throw CommandException(fname + " is not defined");
	__Function& func = functionMap[fname];
	func.args.clear();
	while(not args.end()) func.args.push_back(args);
	callLocations.push_back(Command::getFileIndex());
	jump_to_command(Tokens(to_string(func.location)));
	return to_string(func.location);
}

string StdCommand::end_function_command(Args args)
{
	end_command_1(Tokens(nodeStack.back()));
	nodeStack.pop_back();
	Command::setFileIndex(callLocations.back() + 1);
	callLocations.pop_back();
	return "";
}

string StdCommand::ret_command_0(Args args)
{
	if(returnValues.size() == 0) return "null";
	string ret = returnValues.back();
	returnValues.pop_back();
	return ret;
}

string StdCommand::ret_command_1(Args args)
{
	int i = args;
	if(returnValues.size() <= i or i < 0) return "null";
	string ret = returnValues[i];
	returnValues.erase(returnValues.begin() + i);
	return ret;
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
	cend.sub("while").proto(end_while_command, 0);
	cend.sub("function").proto(end_function_command, 0);
	cbegin.proto(begin_command_0, 0);
	cbegin.proto(begin_command_1, 1);
	celse.proto(else_command, 0);
	cequal.proto(equal_command, 2);
	cjump.proto(jump_command, 1);
	cjump.sub("to").proto(jump_to_command, 1);
	clabel.proto(label_command, 1);
	cignore.proto(ignore_command, 0);
	cwhile.proto(while_command, 1);
	cfunction.proto(function_command, -1);
	creturn.proto(return_command, 1);
	ccall.proto(call_command, -1);
	cret.proto(ret_command_0, 0);
	cret.proto(ret_command_1, 1);
	cless.proto(less_command, 2);


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
	cignore.arm();
	cwhile.arm();
	cfunction.arm();
	creturn.arm();
	ccall.arm();
	cret.arm();
	cless.arm();
}
