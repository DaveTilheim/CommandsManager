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
	return *memory[id];
}
