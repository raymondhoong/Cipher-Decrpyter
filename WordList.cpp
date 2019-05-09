#include "provided.h"
#include <string>
#include <vector>
using namespace std;

#include "MyHash.h"
#include <iostream>
#include <fstream>
#include <list>
#include <cctype>

class WordListImpl
{
public:
	WordListImpl();
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
	MyHash<string, list<string>> wordTable;
	string getPattern(const string& word) const;
	string convertLower(const string& word) const;
};

WordListImpl::WordListImpl()
{
}

bool WordListImpl::loadWordList(string filename)
{
	wordTable.reset();

		// Load text file into program
	ifstream infile(filename);
	if (!infile)
	{
		cerr << "Error: Cannot open wordlist.txt!" << endl;
		return false;
	}

	string s;
	bool validWord;

			// Iterate through every line in the list
	while (getline(infile, s))
	{
			// Ignore invalid lines with characters other than letters or apostrophes
		validWord = true;
		for (unsigned int k = 0; k < s.size(); k++)
		{
			if (!isalpha(s[k]) && s[k] != '\'')
			{
				validWord = false;
				break;
			}
		}
		if (validWord)
		{
				// Convert word to lowercase letters
			string uncapWord = convertLower(s);

				// Get the letter sequence for the word
			string key = getPattern(uncapWord);
			list<string>* vptr = wordTable.find(key);

				// Add word to the list according to its sequence
			if (vptr == nullptr)
			{
				list<string> wordList;
				wordList.push_back(uncapWord);
				wordTable.associate(key, wordList);
			}
			else
				vptr->push_back(uncapWord);
		}
	}
	return true;
}

bool WordListImpl::contains(string word) const
{
	string uncapWord = convertLower(word);
	string key = getPattern(uncapWord);

	const list<string>* vptr = wordTable.find(key);

	if (vptr == nullptr)
		return false;

	for (list<string>::const_iterator it = vptr->begin(); it != vptr->end(); it++)
	{
		if ((*it) == uncapWord)
			return true;
	}
	return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
		// Check if inputs are the same length
	if (cipherWord.size() != currTranslation.size())
		return vector<string>();
	
		// Check if inputs contain the correct characters and match each other
	for (unsigned int k = 0; k < cipherWord.size(); k++)
	{
			// If trans contain any other characters, it is invalid
		if (!isalpha(currTranslation[k]) && currTranslation[k] != '?' && currTranslation[k] != '\'')
			return vector<string>();
		else
		{
				// If trans[k] is letter or apostrophe, cipher[k] must be letter
			if ((isalpha(currTranslation[k]) || currTranslation[k] == '?') && !isalpha(cipherWord[k]))
				return vector<string>();

				// If trans[k] is apostrophe, cipher[k] must be apostrophe
			else if (currTranslation[k] == '\'' && cipherWord[k] != '\'')
				return vector<string>();
		}
	}
	
		// Retrieve the correct list for the letter pattern
	string uncapCipher = convertLower(cipherWord);
	string key = getPattern(uncapCipher);
	const list<string>* vptr = wordTable.find(key);

		// If no such list of candidates exist, return empty vector
	if (vptr == nullptr)
		return vector<string>();

		// Only add words that match currTranslation to the candidateList
	vector<string> candidateList;
	string uncapTrans = convertLower(currTranslation);

	for (list<string>::const_iterator w = vptr->begin(); w != vptr->end(); w++)
	{
			// If the lengths don't match, continue to the next word in the list
		if (w->size() != uncapTrans.size())
			continue;
			
			// Each index of currTrans must match with the index of the word
		bool match = true;
		for (unsigned int j = 0; j < w->size(); j++)
		{
				// If trans[j] is unknown, word[j] must be a letter
			if (uncapTrans[j] == '?' && !isalpha((*w)[j]))
				match = false;

				// Then trans[j] must be a letter or an apostrophe, and word[j] MUST also too
			else if (uncapTrans[j] != '?' && (*w)[j] != uncapTrans[j])
				match = false;
		}

			// If the word passes all the qualifications, add it to the candidate vector
		if (match)
			candidateList.push_back(*w);
	}
	return candidateList;
}

string WordListImpl::getPattern(const string& word) const
{
	MyHash<char, char> ptable;
	string s;
	char symb = 65;
	for (unsigned int k = 0; k < word.size(); k++)
	{
		char* ptr = ptable.find(word[k]);
		if (ptr == nullptr)
		{
			ptable.associate(word[k], symb);
			s += symb;
			symb++;
		}
		else
			s += *ptr;
	}
	return s;
}

string WordListImpl::convertLower(const string& word) const
{
	string finalWord;
	for (unsigned int k = 0; k < word.size(); k++)
		finalWord += tolower(word[k]);
	return finalWord;
}

//***** hash functions for string, int, and char *****

unsigned int hasher(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hasher(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hasher(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}
