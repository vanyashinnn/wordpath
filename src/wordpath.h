#ifndef WORDPATH_H
#define WORDPATH_H
#include <iostream>
#include <vector>
#define RUS_LOCALE "ru_RU.UTF-8"
typedef std::vector<std::wstring> StringList;
class WordPath
{
public:
    enum Error{
        PATH_FOUND = 0,
        PATH_NOT_FOUND,
        DICTIONARY_NOT_FOUND,
        WORDS_LENGTH_NOT_EQUAL,
        EMPTY_WORDS
    };
    explicit WordPath(const std::wstring & first, const std::wstring & second, const char * wordsFilename = "res/word_rus.txt");
    ~WordPath();
    Error status() const;
    StringList words() const;
    void test() const;
private:
    WordPath(const WordPath&);
    class Impl;
    Impl * pimpl;
};

#endif // WORDPATH_H
