#include <iostream>
#include "Tokens.hpp"



int main(int argc, char const *argv[])
{
	Tokens tokens("Hello world I am Arthur", ' ');
	cout << tokens << endl;
	return 0;
}
