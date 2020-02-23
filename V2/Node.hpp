#ifndef NODE_HPP
#define NODE_HPP
#include "CommandException.hpp"
#include "Memory.hpp"

class Node
{
private:
	map<string, Memory *> memory;
	Node *child = nullptr;
	Node *super;
public:
	Node(Node *super=nullptr);
	~Node();
	void addMemory(string id, Memory *mem);
	void addNode();
	bool removeNode();
	Memory& readMemory(string id);
	bool contains(string id) const;
	friend class StdCommand;
};

#endif
