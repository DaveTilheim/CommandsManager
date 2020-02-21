#include "Node.hpp"


Node::Node() {}

Node::~Node()
{
	for(auto it : memory)
	{
		delete it.second;
	}
	for(auto it : memoryBlocks)
	{
		delete it.second;
	}
	memory.clear();
	memoryBlocks.clear();
	cerr << "Node destroyed" << endl;
}

void Node::addMemory(string id, Memory *mem)
{
	if(memory.find(id) != memory.end())
	{
		delete memory[id];
	}
	memory[id] = mem;
}

Memory& Node::readMemory(string id)
{
	if(memory.find(id) != memory.end())
		return *memory[id];
	throw CommandException(id + " undefined");
}

bool Node::contains(string id) const
{
	for(auto mem : memory)
	{
		if(id == mem.first) return true;
	}
	return false;
}
