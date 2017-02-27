#include "wordpath.h"

class WordPath::Impl{
public:
    explicit Impl(const std::string& first, const std::string& second, const std::string& wordsFilename){
        std::cout << "WordPath::Impl()\n";
    }
    ~Impl(){
        std::cout << "WordPath::~Impl()\n";
    }
    StringList words;
};

WordPath::WordPath(const std::string& first, const std::string& second, const std::string& wordsFilename):
    pimpl(new Impl(first, second, wordsFilename))
{
    std::cout << "WordPath(...)\n";
}

WordPath::~WordPath()
{
    delete pimpl;
    pimpl = 0;
    std::cout << "~WordPath()\n";
}

StringList WordPath::words() const
{
    return pimpl->words;
}
