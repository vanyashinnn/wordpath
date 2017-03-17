#ifndef WORDPATH_H
#define WORDPATH_H
#include <iostream>
#include <vector>
#include <set>

#define RUS_LOCALE "ru_RU.UTF-8"
typedef std::wstring String;
typedef std::vector<String> StringList;
typedef std::set<String> StringSet;

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
    explicit WordPath(const String & first, const String & second, const char * wordsFilename = "res/dictionary");
    ~WordPath();
    Error status() const;
    String statusText() const;
    StringList& words() const;
    static int difference(const String& first, const String& second);
    static String getErrorText(Error error);
private:
    WordPath(const WordPath&);
    class Impl;
    Impl * pimpl;
};

#endif // WORDPATH_H
