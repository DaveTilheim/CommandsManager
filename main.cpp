#include "Command.hpp"
#include <fstream>
#include <vector>


struct Object
{
	virtual ~Object() {};
};

template <class T> struct Data : public Object
{
	string id = "(null)";
	T *addr = nullptr;
	Data(string id, T *addr) : id(id), addr(addr) {}
	~Data() { cout << id << " delete" << endl; delete addr; }
};

map<string, void *> datav;

int main(int argc, char const *argv[])
{

	//datav.push_back(new Data("var", new float(3.14)));

	

	Command::terminal("term$ ");

	for(auto d : datav)
	{
		delete (Object *)d.second;
	}

	return 0;
}





