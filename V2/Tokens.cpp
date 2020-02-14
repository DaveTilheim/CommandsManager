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
	index = cp.index;
}

Tokens::~Tokens()
{

}

void Tokens::setSeparator(char s_separator)
{
	separator = s_separator;
}

string Tokens::applyLock(string s_tokens)
{
	bool lock = false;
	for(int i = 0; i < s_tokens.size(); i++)
	{
		if(s_tokens[i] == '\"')
		{
			s_tokens[i] = separator;
			lock = not lock;
			continue;
		}
		if(lock)
		{
			if(s_tokens[i] == separator)
			{
				s_tokens[i] = '\e';
			}
		}
	}
	return s_tokens;
}

void Tokens::freeLock()
{
	int o=0;
	for(int i = index; i < tokens.size(); i++)
	{
		for(int j = 0; j < tokens[i].size(); j++)
		{
			if(tokens[i][j] == '\e')
			{
				tokens[i][j] = separator;
			}
		}
	}
}

void Tokens::setTokens(string s_tokens)
{
	s_tokens = applyLock(s_tokens);
	stringstream ss(s_tokens);
	string token;
	while (getline(ss, token, separator))
	{
		if(token.size())
		{
			tokens.push_back(token);
		}
	}
}

void Tokens::setIndex(int i)
{
	index = i;
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

string Tokens::getCurrent() const
{
	return tokens[index];
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

bool Tokens::surroundedBy(string sur) const
{
	if(tokens[index].size() <= sur.size() * 2) return false;
	string sub1 = tokens[index].substr(0, sur.size());
	string sub2 = tokens[index].substr(tokens[index].size() - sur.size());
	if(sub1 == sur)
	{
		if(sur == "(")
		{
			return sub2 == ")";
		}
		if(sur == "{")
		{
			return sub2 == "}";
		}
		if(sur == "[")
		{
			return sub2 == "]";
		}
		if(sur == "<")
		{
			return sub2 == ">";
		}
		return sub2 == sur;
	}

	return false;
}

void Tokens::removeSurrounded(string sur)
{
	if(surroundedBy(sur))
	{
		tokens[index].erase(0, sur.size());
		tokens[index].erase(tokens[index].size() - sur.size());
	}
}

Tokens Tokens::partial() const
{
	Tokens tok;
	tok.setSeparator(getSeparator());
	for(int i = getIndex(); i < count(); i++)
	{
		tok.tokens.push_back(tokens[i]);
	}
	return tok;
}

Tokens& Tokens::operator=(const Tokens& cp)
{
	separator = cp.separator;
	tokens = cp.tokens;
	index = cp.index;
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
	string tok = getToken();
	int ret = atoi(tok.c_str());
	if(not ret)
	{
		for(int i = 0; i < tok.size(); i++)
		{
			if(tok[i] != '0')
			{
				if(tok[i] == '-' and i == 0) continue;
				throw CommandException("Integer value expected(" + tok + ")"); //create Exception class
			}
		}
	}
	return ret;
}

Tokens::operator float() const
{
	string tok = getToken();
	double ret = atof(tok.c_str());
	if(not ret)
	{
		for(int i = 0; i < tok.size(); i++)
		{
			if(tok[i] != '0')
			{
				if(tok[i] != '.')
				{
					if(tok[i] == '-' and i == 0) continue;
					throw CommandException("Double value expected(" + tok + ")");
				}
			}
		}
	}
	return ret;
}

string Tokens::operator[](int i) const
{
	return getToken(i);
}

ostream& operator<<(ostream& out, const Tokens& tokens)
{
	string buff;
	for(Token token : tokens.tokens)
	{
		buff += token + ", ";
	}
	if(buff.size())
		buff.erase(buff.size() - 2);
	return out << buff;
}
