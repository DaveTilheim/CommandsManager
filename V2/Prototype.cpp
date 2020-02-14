#include "Prototype.hpp"


Prototype::Prototype(function<string (Args)> func, string description) : func(func), description(description)
{

}

Prototype::~Prototype()
{

}

string Prototype::getDescription() const
{
	return description;
}

function<string (Args)> Prototype::getFunction() const
{
	return func;
}

void Prototype::setFunction(function<string (Args)> func)
{
	this->func = func;
}

string Prototype::call(Args args) const
{
	return func(args);
}

string Prototype::operator()(Args args) const
{
	return call(args);
}

ostream& operator<<(ostream& out, const Prototype& pr)
{
	return out << "Description: " << pr.description;
}
