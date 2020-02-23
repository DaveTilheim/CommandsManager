#ifndef MEMORY_HPP
#define MEMORY_HPP
#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>

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
	string reads();
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
	static bool equal(string other, string ther);
	static void addType(string type);
	static string type(string arg);
	static Memory *create(string value);
};

class VectorMemory : public Memory
{
public:
	VectorMemory();
	VectorMemory(VectorMemory&);
	~VectorMemory();
	int size() const;
	void clear();
	Memory& get(int i);
	void add(Memory *);
};

class PrimitiveMemory : public Memory
{
public:
	using Memory::Memory;
	~PrimitiveMemory();
	static bool isPrimitive(string arg);
};

class Integer : public PrimitiveMemory
{
public:
	Integer(string value);
	using PrimitiveMemory::PrimitiveMemory;
	static bool isInteger(string arg);
};

class Float : public PrimitiveMemory
{
public:
	Float(string value);
	using PrimitiveMemory::PrimitiveMemory;
	static bool isFloat(string arg);
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

class String : public ObjectMemory
{
public:
	String(const string& copy);
};


#endif
