#ifndef NODE_HPP
#define NODE_HPP
#include "CommandException.hpp"
#include "Memory.hpp"

class Node
{
private:
	map<string, Memory *> memory;
	map<string, Node *> memoryBlocks;
public:
	Node();
	~Node();
	void addMemory(string id, Memory *mem);
	Memory& readMemory(string id);
	bool contains(string id) const;
	friend class StdCommand;
};

#endif
