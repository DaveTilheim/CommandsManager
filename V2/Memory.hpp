#ifndef MEMORY_HPP
#define MEMORY_HPP
#include <iostream>
#include <map>

using namespace std;


class Memory
{
private:
	static map<string, int> types;
	static int counter;
protected:
	int typeId = 0;
	void *pdata;
public:
	Memory();
	template <class T> Memory(const T& tocopy)
	{
		pdata = new T(tocopy);
	}
	virtual ~Memory();
	void setType(string type);
	string getType() const;
	template <class T> const T& read()
	{
		return *(T *)pdata;
	}
	template <class T> Memory& operator=(const T& tocopy)
	{
		*(T *)pdata = tocopy;
		return *this;
	}
	bool isNull() const;
	static void addType(string type);
};

class PrimitiveMemory : public Memory
{
public:
	using Memory::Memory;
	~PrimitiveMemory();
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
	~ObjectMemory();
};


#endif
