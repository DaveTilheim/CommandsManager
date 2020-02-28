#ifndef NODE_HPP
#define NODE_HPP
#include "CommandException.hpp"
#include "Memory.hpp"

class Node
{
private:
	static int idCounter;
	string name;
	map<string, Memory *> memory;
	Node *child = nullptr;
	Node *super;
public:
	Node(Node *super=nullptr, string name="m");
	~Node();
	void addMemory(string id, Memory *mem);
	string getCurrentName();
	string addNode(string name="");
	string removeNode(string name="") noexcept(false);
	Memory& readMemory(string id);
	map<string, Memory *> readAllMemory();
	bool contains(string id) const;
	friend class StdCommand;
};

#endif
