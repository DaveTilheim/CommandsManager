#include "Tokens.hpp"


Tokens::Tokens(char separator) : separator(separator)
{

}

Tokens::Tokens(string s_tokens, char separator) : separator(separator)
{
	setTokens(s_tokens);
}

Tokens::Tokens(const Tokens& cp)
{
	tokens = cp.tokens;
	separator = cp.separator;
}

Tokens::~Tokens()
{

}

void Tokens::setSeparator(char s_separator)
{
	separator = s_separator;
}

void Tokens::setTokens(string s_tokens)
{
	stringstream ss(s_tokens);
	string token;
	while (getline(ss, token, separator))
	{
		tokens.push_back(token);
	}
}

void Tokens::setIndex(int i)
{
	if(i < tokens.size())
	{
		index = i;
	}
}

char Tokens::getSeparator() const
{
	return separator;
}

string Tokens::getToken() const
{
	if(not end())
	{
		return tokens[index++];
	}
	return "";
}

string Tokens::getToken(int index) const
{
	if(index < tokens.size())
	{
		return tokens[index];
	}
	return "";
}

const vector<Token>& Tokens::getTokens() const
{
	return tokens;
}

int Tokens::getIndex() const
{
	return index;
}

int Tokens::count() const
{
	return tokens.size();
}

bool Tokens::end() const
{
	return index >= tokens.size();
}

void Tokens::resetIndex()
{
	setIndex(0);
}

void Tokens::pop()
{
	if(index)
	{
		index--;
	}
	tokens.erase(tokens.begin());
}

Tokens& Tokens::operator=(const Tokens& cp)
{
	separator = cp.separator;
	tokens = cp.tokens;
	index = 0;
	return *this;
}

Tokens& Tokens::operator=(string s_tokens)
{
	tokens.clear();
	setTokens(s_tokens);
	index = 0;
	return *this;
}

Tokens& Tokens::operator<<(string s_tokens)
{
	setTokens(s_tokens);
	return *this;
}

bool Tokens::operator!()
{
	return not end();
}

Tokens::operator string() const
{
	return getToken();
}

Tokens::operator int() const
{
	return atoi(getToken().c_str());
}

Tokens::operator float() const
{
	return atof(getToken().c_str());
}

Tokens::operator vector<Token>() const
{
	return vector<Token>(tokens.begin()+1, tokens.end());
}

string Tokens::operator[](int i) const
{
	return tokens[i];
}

ostream& operator<<(ostream& out, const Tokens& tokens)
{
	for(Token token : tokens.tokens)
	{
		out << token << endl;
	}
	return out;
}
