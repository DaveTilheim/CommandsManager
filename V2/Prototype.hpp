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
	function<string (Args)> func;
public:
	Prototype() = delete;
	Prototype(const Prototype&) = delete;
	Prototype(function<string (Args)> func, string description="<no description>");
	~Prototype();
	string getDescription() const;
	function<string (Args)> getFunction() const;
	void setFunction(function<string (Args)> func);
	string call(Args args) const;
	string operator()(Args args) const;
	Prototype& operator=(const Prototype&) = delete;
	friend ostream& operator<<(ostream& out, const Prototype& pr);
};


#endif
