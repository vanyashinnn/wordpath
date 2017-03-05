#ifndef WORDPATH_H
#define WORDPATH_H
#include <iostream>
#include <vector>

#define RUS_LOCALE "ru_RU.UTF-8"
typedef std::wstring String;
typedef std::vector<String> StringList;

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
    explicit WordPath(const String & first, const String & second, const char * wordsFilename = "res/word_rus.txt");
    ~WordPath();
    Error status() const;
    String statusText() const;
    StringList words() const;
    static int distance(const String& first, const String& second);
    static String getErrorText(Error error);
    void test() const;
private:
    WordPath(const WordPath&);
    class Impl;
    Impl * pimpl;
};

#endif // WORDPATH_H
