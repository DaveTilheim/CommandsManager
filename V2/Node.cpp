#include "Node.hpp"

int Node::idCounter = 0;

Node::Node(Node *super, string name) : super(super)
{
	if(name.size())
		this->name = name;
	else
		this->name = to_string(idCounter++);
	//cout << "[Node created " << this->name << endl;
}

Node::~Node()
{
	idCounter--;
	for(auto it : memory)
	{
		delete it.second;
	}
	if(child) delete child;
	memory.clear();
	//cout << "[Node destroyed " << this->name << endl;
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

string Node::addNode(string name)
{
	Node *depth = this;
	while(depth->child) depth = depth->child;
	depth->child = new Node(depth, name);
	return depth->child->name;
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

map<string, Memory *> Node::readAllMemory()
{
	map<string, Memory *> ret;
	const Node *curNode = this;
	string node = "";
	while(curNode)
	{
		node = curNode->name;
		for(auto it : curNode->memory)
		{
			ret[node+":"+it.first] = it.second;
		}
		curNode = curNode->child;
	}
	return ret;
}

string Node::removeNode(string name) noexcept(false)
{
	if(not child) return "";
	Node *depth = this;
	while(depth->child) depth = depth->child;
	depth = depth->super;
	if(name.size() == 0)
	{
		name = depth->child->name;
		delete depth->child;
	}
	else
	{
		while(depth)
		{
			if(name == depth->child->name)
			{
				delete depth->child;
				break;
			}
			depth = depth->super;
		}
	}
	if(not depth)
	{
		if(name.size())
		{
			throw CommandException(name + " block does not exists");
		}
		throw CommandException("block does not exists");
	}
	depth->child = nullptr;
	return name;
}

string Node::getCurrentName()
{
	Node *node = this;
	while(node->child) node = node->child;
	return node->name;
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
