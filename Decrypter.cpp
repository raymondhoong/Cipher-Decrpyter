#include "provided.h"
#include <string>
#include <vector>
using namespace std;

#include <set>
#include <stack>
#include <algorithm>

class DecrypterImpl
{
public:
	DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
	WordList m_wordList;
	Tokenizer m_tokenizer;
	Translator m_translator;

	string crackRecursion(string ciphertext, vector<string>& output);
	string pickWord(const vector<string>& tokens) const;
	vector<string> getCompletedWords(const string& decryptedMessage) const;
};

DecrypterImpl::DecrypterImpl()
	: m_tokenizer(",;:.!()[]{}-\"#$%^& 0123456789")
{
}

bool DecrypterImpl::load(string filename)
{
	if (m_wordList.loadWordList(filename))
		return true;
	else
		return false;  
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{

	vector<string> output;

	crackRecursion(ciphertext, output);

	sort(output.begin(), output.end());

	return output;
}

string DecrypterImpl::crackRecursion(string ciphertext, vector<string>& output)
{
		// Tokenize the ciphertext message
	vector<string> tokens = m_tokenizer.tokenize(ciphertext);

		// Pick the ciphertext word with the most unknown letters
	string word = pickWord(tokens);

		// Translate the chosen word using the current mapping
	string wordDecrypted = m_translator.getTranslation(word);

		// Final all possible candidates for the chosen word
	vector<string> collection = m_wordList.findCandidates(word, wordDecrypted);

		// If collection, discard current mapping and return
	if (collection.empty())
	{
		m_translator.popMapping();
		return string();
	}

		// Go through each possible candidate
	for (vector<string>::iterator it = collection.begin(); it != collection.end(); it++)
	{
			// If mapping unsuccessful, continue on to next candidate
		if (!m_translator.pushMapping(word, *it))
			continue;
		
			// Translate entire ciphertext message with current mapping
		string decryptedMessage = m_translator.getTranslation(ciphertext);

			// Get the fully-translated words
		vector<string> completedWords = getCompletedWords(decryptedMessage);

			// If any completed words don't exist, discard the current mapping and continue to next candidate
		bool skip = false;
		for (vector<string>::iterator q = completedWords.begin(); q != completedWords.end(); q++)
		{
			if (!m_wordList.contains(*q))
			{
				m_translator.popMapping();
				skip = true;
				break;
			}
		}
		if (skip == true)
			continue;

			// If number of completed words equal number of ciphertext words, solution found
		if (completedWords.size() == tokens.size())
		{
			output.push_back(decryptedMessage);
			m_translator.popMapping();
		}
			// Otherwise, continue to next ciphertext token
		else
			crackRecursion(ciphertext, output);
	}
	
	m_translator.popMapping();
	return string();
}

string DecrypterImpl::pickWord(const vector<string>& tokens) const
{
	int topCount = 0;
	string finalWord;
	string currWord;
	for (vector<string>::const_iterator it = tokens.begin(); it != tokens.end(); it++)
	{
		int currCount = 0;
		currWord = m_translator.getTranslation(*it);
		for (unsigned int k = 0; k < currWord.size(); k++)
		{
			if (currWord[k] == '?')
				currCount++;
		}
		if (currCount > topCount)
		{
			finalWord = *it;
			topCount = currCount;
		}
	}
	return finalWord;
}

vector<string> DecrypterImpl::getCompletedWords(const string& decryptedMessage) const
{
	vector<string> tokens = m_tokenizer.tokenize(decryptedMessage);

	vector<string> completedWords;
	for (vector<string>::const_iterator p = tokens.begin(); p != tokens.end(); p++)
	{
		bool isCompleted = true;
		for (unsigned int k = 0; k < p->size(); k++)
		{
			if ((*p)[k] == '?')
			{
				isCompleted = false;
				break;
			}
		}
		if (isCompleted)
			completedWords.push_back(*p);
	}
	return completedWords;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
