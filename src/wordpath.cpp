#include <fstream>
#include <locale>
#include "wordpath.h"

class WordPath::Impl{
public:
    explicit Impl(const std::wstring& first, const std::wstring& second, const char * wordsFilename):
        _first(first),
        _second(second),
        _status(WordPath::PATH_NOT_FOUND)
    {
        _status = readWords(wordsFilename);
    }
    WordPath::Error readWords(const char * wordsFilename){
        if(_first.empty()){
            return WordPath::EMPTY_WORDS;
        }
        if(_first.length() != _second.length()){
            return WordPath::WORDS_LENGTH_NOT_EQUAL;
        }
        if(_first == _second){
            _words.push_back(_first);
            return WordPath::PATH_FOUND;
        }
        std::wifstream in(wordsFilename);
        if(!in){
            return WordPath::DICTIONARY_NOT_FOUND;
        }
        std::wstring tmp;
        in.imbue(std::locale(RUS_LOCALE));
        while(true){
            in >> tmp;
            if(tmp.length() == _first.length()){
                _dictionary.push_back(tmp);
            }
            if(!in.good()){
                break;
            }
        }
        in.close();
        return WordPath::PATH_NOT_FOUND;
    }
    std::wstring _first;
    std::wstring _second;
    StringList _words;
    StringList _dictionary;
    WordPath::Error _status;
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
    return pimpl->_status;
}

StringList WordPath::words() const
{
    return pimpl->_words;
}

int WordPath::distance(const std::wstring& first, const std::wstring& second)
{
    if(first.length() != second.length()){
        return -1;
    }
    int distance(0);
    for(int i=0; i<first.length(); ++i){
        if(first.at(i) != second.at(i)){
            ++distance;
        }
    }
    return distance;
}

void WordPath::test() const
{
    std::wofstream out("out.txt");
    out.imbue(std::locale(RUS_LOCALE));
    out << "size: " << pimpl->_dictionary.size() << L"\n";
    for(const auto & word: pimpl->_dictionary){
        out << word << "\n";
        std::wcout << word << "\n";
    }
    out.close();
}
