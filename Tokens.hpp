#ifndef TOKENS_HPP
#define TOKENS_HPP
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

typedef string Token;

class Tokens
{
private:
	vector<Token> tokens;
	char separator;
	mutable int index = 0;
public:
	Tokens(char separator=' ');
	Tokens(string s_tokens, char separator=' ');
	Tokens(const Tokens& cp);
	~Tokens();
	void setSeparator(char separator=' ');
	void setTokens(string s_tokens);
	void setIndex(int i);
	char getSeparator() const;
	string getToken() const;
	string getToken(int index) const;
	const vector<Token>& getTokens() const;
	int getIndex() const;
	int count() const;
	bool end() const;
	void resetIndex();
	void pop();
	Tokens& operator=(const Tokens& cp);
	Tokens& operator=(string s_tokens);
	Tokens& operator<<(string s_tokens);
	bool operator!();
	operator string() const;
	operator int() const;
	operator float() const;
	operator vector<Token>() const;
	string operator[](int i) const;
	friend ostream& operator<<(ostream& out, const Tokens& tokens);
};


#endif
