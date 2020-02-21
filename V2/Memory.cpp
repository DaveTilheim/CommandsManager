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


ObjectMemory::~ObjectMemory()
{
	del();
	cerr << "ObjectMemory destroyed" << endl;
}

