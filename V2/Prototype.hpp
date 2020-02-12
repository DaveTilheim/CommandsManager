#ifndef PROTOTYPE_HPP
#define PROTOTYPE_HPP
#include "Tokens.hpp"
#include <functional>

typedef Tokens Args;

class Command;

class Prototype final
{
private:
	string description;
	function<void *(Args)> func;
public:
	Prototype() = delete;
	Prototype(const Prototype&) = delete;
	Prototype(function<void *(Args)> func, string description="<no description>");
	~Prototype();
	string getDescription() const;
	function<void *(Args)> getFunction() const;
	void setFunction(function<void *(Args)> func);
	void *call(Args args) const;
	void *operator()(Args args) const;
	Prototype& operator=(const Prototype&) = delete;
	friend ostream& operator<<(ostream& out, const Prototype& pr);
};


#endif
