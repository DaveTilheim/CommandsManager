#include "Node.hpp"


Node::Node(Node *super) : super(super) {}

Node::~Node()
{
	for(auto it : memory)
	{
		delete it.second;
	}
	if(child) delete child;
	memory.clear();
	cerr << "Node destroyed" << endl;
}

void Node::addMemory(string id, Memory *mem)
{
	Node *depth = this;
	while(depth->child) depth = depth->child;
	if(depth->memory.find(id) != depth->memory.end())
	{
		delete depth->memory[id];
	}
	depth->memory[id] = mem;
}

void Node::addNode()
{
	Node *depth = this;
	while(depth->child) depth = depth->child;
	depth->child = new Node(depth);
}

Memory& Node::readMemory(string id)
{
	Node *depth = this;
	while(depth->child) depth = depth->child;
	while(depth)
	{
		if(depth->memory.find(id) != depth->memory.end())
			return *depth->memory[id];
		depth = depth->super;
	}
	throw CommandException(id + " undefined");
}

bool Node::removeNode()
{
	if(not child) return false;
	Node *depth = this;
	while(depth->child) depth = depth->child;
	depth = depth->super;
	delete depth->child;
	depth->child = nullptr;
	return true;
}

bool Node::contains(string id) const
{
	for(auto mem : memory)
	{
		if(id == mem.first) return true;
	}
	if(child) return child->contains(id);
	return false;
}
