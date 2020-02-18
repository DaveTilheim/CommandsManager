#include <iostream>
#include "Prompt.hpp"
#include <math.h>
#include <cstdlib>


class Memory
{
private:
	static map<string, int> types;
	static int counter;
protected:
	int typeId = 0;
	void *pdata;
public:

	Memory() : pdata(nullptr)
	{

	}
	template <class T> Memory(const T& tocopy)
	{
		pdata = new T(tocopy);
	}

	virtual ~Memory()
	{

	}

	void setType(string type)
	{
		typeId = Memory::types[type];
	}

	string getType() const
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

	template <class T> const T& read()
	{
		return *(T *)pdata;
	}

	template <class T> Memory& operator=(const T& tocopy)
	{
		*(T *)pdata = tocopy;
		return *this;
	}
	

	bool isNull() const
	{
		return pdata == nullptr;
	}

	static void addType(string type)
	{
		if(types.find(type) != types.end())
		{
			types[type] = ++counter;
		}
	}
};

map<string, int> Memory::types = map<string, int>();
int Memory::counter = 0;

class PrimitiveMemory : public Memory
{
protected:
	void * pdata;
public:

	using Memory::Memory;

	~PrimitiveMemory()
	{
		if(not isNull())
		{
			free(pdata);
		}
		cerr << "PrimitiveMemory destroyed" << endl;
	}

	
};


class ObjectMemory : public Memory
{
private:
	function<void()> del = [](){};
public:
	template <class T> ObjectMemory(const T& tocopy) : Memory(tocopy)
	{
		del = [this]()
		{
			if(pdata)
			{
				delete (T *)pdata;
				pdata = nullptr;
			}
		};
	}

	~ObjectMemory()
	{
		del();
		cerr << "ObjectMemory destroyed" << endl;
	}
};

class TypedMemory : public Memory
{
private:
	string type = "none";
public:
	using Memory::Memory;
	void setType(string types)
	{
		type = types;
	}
	string getType() const
	{
		return type;
	}
};


class Node
{
private:
	map<string, Memory *> memory;
	map<string, Node *> memoryBlocks;
public:
	Node() {}
	~Node()
	{
		for(auto it : memory)
		{
			cerr << it.first << " ";
			delete it.second;
		}
		for(auto it : memoryBlocks)
		{
			cerr << it.first << " ";
			delete it.second;
		}
		memory.clear();
		memoryBlocks.clear();
		cerr << "Node destroyed" << endl;
	}

	void addMemory(string id, Memory *mem)
	{
		if(memory.find(id) != memory.end())
		{
			delete memory[id];
		}
		memory[id] = mem;
	}

	Memory& readMemory(string id)
	{
		return *memory[id];
	}
};


int main(int argc, char const *argv[])
{
	Memory::addType("Integer");
	Memory::addType("String");
	Node root;
	Command create("create");
	create.proto([&root](Args args)
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

	Command read("read");
	read.proto([&root](Args args)
	{
		string var = args;
		if(root.readMemory(var).getType() == "Integer")
		{
			return to_string(root.readMemory(var).read<int>());
		}
		return root.readMemory(var).read<string>();
	}, 1);

	read.arm();
	create.arm();

	Prompt::run(">>> ");
	


	
	

	return 0;
}
