#include "Prototype.hpp"


Prototype::Prototype(function<void *(Args)> func, string description) : func(func), description(description)
{

}

Prototype::~Prototype()
{

}

string Prototype::getDescription() const
{
	return description;
}

function<void *(Args)> Prototype::getFunction() const
{
	return func;
}

void Prototype::setFunction(function<void *(Args)> func)
{
	this->func = func;
}

void *Prototype::call(Args args) const
{
	return func(args);
}

void *Prototype::operator()(Args args) const
{
	return call(args);
}

ostream& operator<<(ostream& out, const Prototype& pr)
{
	return out << "Description: " << pr.description;
}
