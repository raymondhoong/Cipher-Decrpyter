# CS32-Project4

## Cipher-Decrypter

Decrypts a cipher and returns all possible solutions. I made this project as an assignment for my class. The only files provided from the instructor are provided.h and the referenced dictionary word list text file.

## Usage

On main.cpp, one can change the constant DECRYPT_THIS to be a string of their choice. If there is a matching decryption for this string based on the given list of dictionary words, it will return the translation. Otherwise it will notify you that there is no translation. The three tests all show the success of the decrypter on different ciphers. Apostrophes and question marks are not supported in the ciphers. The following are supported and will kept exactly as is in the translation: 1234567890 ,;:.!()[]{}-"#$%^&

## Class Descriptions

## MyHash

MyHash is an implementation of a hash table. The rules for this project made it so that using the STL unordered_map or map are not allowed. Therefore, in order to reach the desired efficiency, an implementation of a hash table was necessary.

## Decrypter

The decrypter decodes the cipher by recursively checking which mapping works for all characters in the cipher. This is the class where the decoded words are returned.

## Tokenizer

The Tokenizer splits the starting cipher string into separate words and stores them into a vector.

## Translator

The translator uses a linked list that acts as a stack to store current mappings. Mappings are relating each letter in the alphabet to another letter. This is what is used to see how many matches there are in a mapping and to ultimately decode the cipher.

## WordList

The WordList is the class that stores the allowed words. The allowed words are loaded from the enigmix.txt file that has plenty of English dictionary words.

## Credit

The word list text file is found from http://www.gwicks.net/dictionaries.htm
