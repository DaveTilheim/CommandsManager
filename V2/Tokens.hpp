#ifndef TOKENS_HPP
#define TOKENS_HPP
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include "CommandException.hpp"

using namespace std;

typedef string Token;

class Tokens
{
private:
	vector<Token> tokens;
	char separator;
	mutable int index = 0;
	string applyLock(string s_tokens);
public:
	Tokens(char separator=' ');
	Tokens(string s_tokens, char separator=' ');
	Tokens(const Tokens& cp);
	~Tokens();
	void setSeparator(char separator=' ');
	void freeLock();
	void setTokens(string s_tokens);
	void setIndex(int i);
	char getSeparator() const;
	string getToken() const;
	string getToken(int index) const;
	string getCurrent() const;
	vector<Token>& getTokens();
	int getIndex() const;
	int count() const;
	bool end() const;
	void resetIndex();
	void pop();
	bool surroundedBy(string sur="(") const;
	void removeSurrounded(string sur="(");
	Tokens partial() const;
	Tokens partial(int nb) const;
	Tokens& operator=(const Tokens& cp);
	Tokens& operator=(string s_tokens);
	Tokens& operator<<(string s_tokens);
	Tokens& operator<<(const Tokens& toks);
	bool operator!();
	operator string() const;
	operator int() const;
	operator float() const;
	Tokens& operator++();
	string operator[](int i) const;
	friend ostream& operator<<(ostream& out, const Tokens& tokens);
};



#endif
