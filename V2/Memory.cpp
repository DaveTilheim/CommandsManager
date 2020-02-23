#include "Memory.hpp"

map<string, int> Memory::types = map<string, int>();
int Memory::counter = 0;

Memory::Memory() : pdata(nullptr)
{

}

Memory::~Memory()
{
	//cout << "Memory destroyed" << endl;
}

bool Memory::equal(string v1, string v2)
{
	return v1 == v2;
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

string Memory::reads()
{
	if(getType() == "Integer") return to_string(*(int *)pdata);
	if(getType() == "Float") return to_string(*(double *)pdata);
	if(getType() == "String") return *(string *)pdata;
	if(getType() == "Vector")
	{
		string buf;
		VectorMemory *v = dynamic_cast<VectorMemory *>(this);
		for(int i = 0; i < v->size(); i++)
		{
			buf += v->get(i).reads();
			if(i != v->size()-1) buf += " ";
		}
		return buf;
	}
	return "null";
}

Memory *Memory::create(string value)
{
	string type = Memory::type(value);
	if(type == "Integer")
	{
		return new Integer(value);
	}
	else if(type == "Float")
	{
		return new Float(value);
	}
	else
	{
		return new String(value);
	}
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


VectorMemory::VectorMemory()
{
	pdata = new vector<Memory *>();
	setType("Vector");
}

VectorMemory::VectorMemory(VectorMemory& cp) : VectorMemory()
{
	for(int i = 0; i < cp.size(); i++)
	{
		add(Memory::create(cp.get(i).reads()));
	}
}

void VectorMemory::clear()
{
	((vector<Memory *> *)pdata)->clear();
}

VectorMemory::~VectorMemory()
{
	for(int i = 0; i < ((vector<Memory *> *)pdata)->size(); i++)
	{
		delete (Memory *)(*((vector<Memory *> *)pdata))[i];
	}
	delete (vector<Memory *> *)pdata;
}

int VectorMemory::size() const
{
	return ((vector<Memory *> *)pdata)->size();
}

Memory& VectorMemory::get(int i)
{
	return *(Memory *)(*((vector<Memory *> *)pdata))[i];
}

void VectorMemory::add(Memory *m)
{
	((vector<Memory *> *)pdata)->push_back(m);
}



PrimitiveMemory::~PrimitiveMemory()
{
	if(not isNull())
	{
		free(pdata);
	}
	//cerr << "PrimitiveMemory destroyed" << endl;
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
		if(not i and arg[i] == '-') continue;
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
		if(not i and arg[i] == '-') continue;
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
	//cerr << "ObjectMemory destroyed" << endl;
}


String::String(const string& copy) : ObjectMemory(copy)
{
	setType("String");
}
