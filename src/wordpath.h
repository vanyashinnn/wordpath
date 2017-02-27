#ifndef WORDPATH_H
#define WORDPATH_H
#include <iostream>
#include <vector>
typedef std::vector<std::string> StringList;
class WordPath
{
public:
    enum Error{
        PATH_FOUND = 0,
        PATH_NOT_FOUND
    };
    explicit WordPath(const std::string & first, const std::string & second, const std::string & wordsFilename = "");
    ~WordPath();
    Error status() const;
    StringList words() const;
private:
    WordPath(const WordPath&);
    class Impl;
    Impl * pimpl;
};

#endif // WORDPATH_H
