#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
	TokenizerImpl(string separators);
	vector<string> tokenize(const std::string& s) const;
private:
	vector<char> m_separators;
	bool isSeparator(char c) const;
};

TokenizerImpl::TokenizerImpl(string separators)
{
	for (unsigned int k = 0; k < separators.size(); k++)
		m_separators.push_back(separators[k]);
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
	string temp;
	vector<string> Tokens;

	// CHECK TIME COMPLEXITY S*(P + 5)
	for (unsigned int k = 0; k < s.size(); k++)
	{
		if (!isSeparator(s[k]))
		{
			temp += s[k];
			if (k == s.size() - 1)
				Tokens.push_back(temp);
		}
		else if (temp != "")
		{
			Tokens.push_back(temp);
			temp = "";
		}
	}
	return Tokens;
}

bool TokenizerImpl::isSeparator(char c) const
{
	vector<char>::const_iterator it;
	for (it = m_separators.begin(); it != m_separators.end(); it++)
	{
		if (c == *it)
			return true;
	}
	return false;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
	m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
	delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
	return m_impl->tokenize(s);
}
