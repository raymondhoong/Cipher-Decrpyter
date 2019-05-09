#include "provided.h"
#include <string>
using namespace std;

#include <iostream>
#include <list>
#include <map>
#include <cctype>

class TranslatorImpl
{
public:
	TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
	list<map<char, char>> m_mapStack;
	map<char, char> m_map;
	list<map<char, char>> m_plainStack;
	map<char, char> m_plainMap;

	int m_pushCount;
	int m_popCount;
};

TranslatorImpl::TranslatorImpl()
	: m_pushCount(0), m_popCount(0)
{
	for (int k = 65; k < 91; k++)
	{
		m_map[k] = '?';
		m_plainMap[k] = '?';
	}
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
		// Return false if input lengths don't match
	if (ciphertext.size() != plaintext.size())
		return false;

		// Check for inconsistencies with current mapping
	for (unsigned int k = 0; k < ciphertext.size(); k++)
	{
			// If ciphertext or plaintext contain non-letters
		if (!isalpha(ciphertext[k]) && !isalpha(plaintext[k]))
			return false;

			// Check if a cipher letter maps to 2 OR MORE plain letters
		map<char, char>::iterator p;
		p = m_map.find(toupper(ciphertext[k]));
		if (p->second != tolower(plaintext[k]) && p->second != '?')
			return false;

			// Check if a plain letter is mapped to 2 OR MORE cipher letters
		map<char, char>::iterator q;
		q = m_plainMap.find(toupper(plaintext[k]));
		if (q != m_plainMap.end() && q->second != tolower(ciphertext[k]) && q->second != '?')
			return false;
	}

		// Add current maps to stacks
	m_mapStack.push_back(m_map);
	m_plainStack.push_back(m_plainMap);

		// Update maps
	for (unsigned int j = 0; j < ciphertext.size(); j++)
	{
		char upperCipher = toupper(ciphertext[j]);
		m_map[upperCipher] = tolower(plaintext[j]);

		char upperPlain = toupper(plaintext[j]);
		m_plainMap[upperPlain] = tolower(ciphertext[j]);
	}

	m_pushCount++;
	return true;
}

bool TranslatorImpl::popMapping()
{
		// Return false is no more maps to pop off
	if (m_pushCount == m_popCount)
		return false;

		// Set current maps to most-recently pushed maps
	m_map = m_mapStack.back();
	m_plainMap = m_plainStack.back();

		// Pop most-recent maps
	m_mapStack.pop_back();
	m_plainStack.pop_back();

	m_popCount++;
	return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
	string s;

	for (unsigned int k = 0; k < ciphertext.size(); k++)
	{
			// If character of ciphertext not a letter, leave unchanged
		if (!isalpha(ciphertext[k]))
		{
			s += ciphertext[k];
			continue;
		}

			// Find mapping for ciphertext character
		map<char, char>::const_iterator it;
		it = m_map.find(toupper(ciphertext[k]));

			// If mapping found, convert ciphertext character with respective letter case
		if (isalpha(it->second))
		{
			if (isupper(ciphertext[k]))
				s += toupper(it->second);
			else
				s += it->second;
		}

			// If no mapping found, convert character to ?
		else
			s += it->second;
	}

    return s; // This compiles, but may not be correct
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
