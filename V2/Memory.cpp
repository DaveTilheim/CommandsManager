#include "Memory.hpp"

map<string, int> Memory::types = map<string, int>();
int Memory::counter = 0;

Memory::Memory() : pdata(nullptr)
{

}

Memory::~Memory()
{
	cout << "Memory destroyed" << endl;
}

void Memory::setType(string type)
{
	typeId = Memory::types[type];
}

string Memory::getType() const
{
	for(auto t : types)
	{
		if(t.second == typeId)
		{
			return t.first;
		}
	}
	return "none";
}

string Memory::type(string arg)
{
	if(Integer::isInteger(arg)) return "Integer";
	if(Float::isFloat(arg)) return "Float";
	return "String";
}

bool Memory::isNull() const
{
	return pdata == nullptr;
}

void Memory::addType(string type)
{
	if(types.find(type) == types.end())
	{
		types[type] = ++counter;
	}
}






PrimitiveMemory::~PrimitiveMemory()
{
	if(not isNull())
	{
		free(pdata);
	}
	cerr << "PrimitiveMemory destroyed" << endl;
}

bool PrimitiveMemory::isPrimitive(string arg)
{
	if(Integer::isInteger(arg)) return true;
	if(Float::isFloat(arg)) return true;
	return false;
}

Integer::Integer(string value) : PrimitiveMemory(Integer::isInteger(value) ? atoi(value.c_str()) : 0)
{
	setType("Integer");
}

bool Integer::isInteger(string arg)
{
	for(int i = 0; i < arg.size(); i++)
	{
		if(not isdigit(arg[i])) return false;
	}
	return true;
}

Float::Float(string value) : PrimitiveMemory(Float::isFloat(value) ? atof(value.c_str()) : 0)
{
	setType("Float");
}

bool Float::isFloat(string arg)
{
	int ptCounter = 0;
	for(int i = 0; i < arg.size(); i++)
	{
		if(not isdigit(arg[i]))
		{
			if(i != 0 and i != arg.size() -1 and ptCounter == 0 and arg[i] == '.')
			{
				ptCounter++;
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}


ObjectMemory::~ObjectMemory()
{
	del();
	cerr << "ObjectMemory destroyed" << endl;
}


String::String(const string& copy) : ObjectMemory(copy)
{
	setType("String");
}
