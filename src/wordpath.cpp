#include "wordpath.h"

class WordPath::Impl{
public:
    explicit Impl(const std::wstring& first, const std::wstring& second, const char * wordsFilename):
    status(WordPath::PATH_NOT_FOUND){
        if(first == second){
            words.push_back(first);
            status = PATH_FOUND;
        }
        //std::cout << "WordPath::Impl()\n";
    }
    ~Impl(){
        //std::cout << "WordPath::~Impl()\n";
    }
    StringList words;
    WordPath::Error status;
};

WordPath::WordPath(const std::wstring& first, const std::wstring& second, const char * wordsFilename):
    pimpl(new Impl(first, second, wordsFilename))
{
    //std::cout << "WordPath(...)\n";
}

WordPath::~WordPath()
{
    delete pimpl;
    pimpl = 0;
    //std::cout << "~WordPath()\n";
}

WordPath::Error WordPath::status() const
{
    return pimpl->status;
}

StringList WordPath::words() const
{
    return pimpl->words;
}
