#include <fstream>
#include <locale>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "wordpath.h"
//#define DEBUG
typedef std::multimap<int, String> WordsMap;
typedef std::pair<WordsMap::iterator, WordsMap::iterator> WordsMapEqualRange;

static int random(int min, int max){
    return (rand()%(max - min + 1)) + min;
}

class WordPath::Impl{
public:
    explicit Impl(const String& first, const String& second, const char * wordsFilename):
        _first(first),
        _second(second),
        _status(WordPath::PATH_NOT_FOUND)
    {
        srand(time(0));
        _words.push_back(first);
        _status = readWords(wordsFilename);
        if(WordPath::PATH_NOT_FOUND != _status){
            return;
        }
        searchPath();
        postprocessing();
    }
    void postprocessing(){

        StringList::iterator first = _words.begin();
        StringList::iterator last;
        while(true){
#ifdef DEBUG
            //
            std::wcout << L"size: " << _words.size() << L"\n";
            for(const auto & word: _words){
                std::wcout << word << "\n";
            }
            //
#endif
            ++first;
            if(first == _words.end()){
                break;
            }
            StringList::iterator from = first+1;
            last = std::find(from, _words.end(), (*first));
            if(last == _words.end()){
                continue;
            }
            StringList::iterator to = last+1;
            if(from != _words.end() && to != _words.end()){
#ifdef DEBUG
                std::wcout << (*first) << L": " << (*last) << L"\n";
#endif
                _words.erase(from, to);
                first = _words.begin();
            }
        }
    }

    void searchPath(){
#ifdef DEBUG
        std::wcout << L"searchPath for " << _words.back() << L"\n";
#endif
        _nextWords.clear();
        for(const auto& word: _dictionary){
            if(WordPath::difference(word, _words.back()) == 1){
                int weight = WordPath::difference(word, _second);
                _nextWords.insert(std::pair<int, String>(weight, word));
            }
        }
#ifdef DEBUG
        std::wcout << L"Map:\n";
        for(WordsMap::iterator it = _nextWords.begin(); it != _nextWords.end(); ++it){
            std::wcout << it->first << L": " << it->second << L"\n";
        }
#endif
        if(_nextWords.empty()){
            _status = WordPath::PATH_NOT_FOUND;
            return;
        }
        _equalRange = _nextWords.equal_range(_nextWords.begin()->first);
        _equalRangeList.clear();
        for(WordsMap::iterator it = _equalRange.first; it != _equalRange.second; ++it){
            _equalRangeList.push_back(it->second);
        }
        _words.push_back(_equalRangeList.at(random(0, _equalRangeList.size() - 1)));
        if(_words.back() == _second){
            _status = WordPath::PATH_FOUND;
            return;
        }
        searchPath();
    }
    WordPath::Error readWords(const char * wordsFilename){
        if(_first.empty()){
            return WordPath::EMPTY_WORDS;
        }
        if(_first.length() != _second.length()){
            return WordPath::WORDS_LENGTH_NOT_EQUAL;
        }
        if(_first == _second){
            return WordPath::PATH_FOUND;
        }
        std::wifstream in(wordsFilename);
        if(!in){
            return WordPath::DICTIONARY_NOT_FOUND;
        }
        String tmp;
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
    WordsMapEqualRange _equalRange;
    StringList _equalRangeList;
    String _first;
    String _second;
    StringList _words;
    StringList _dictionary;
    WordsMap _nextWords;
    WordPath::Error _status;
};

WordPath::WordPath(const String& first, const String& second, const char * wordsFilename):
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

String WordPath::statusText() const
{
    return WordPath::getErrorText(pimpl->_status);
}


StringList& WordPath::words() const
{
    return pimpl->_words;
}

int WordPath::difference(const String& first, const String& second)
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

String WordPath::getErrorText(WordPath::Error error)
{
    switch(error){
    case PATH_FOUND:
        return L"Путь найден.";
    case PATH_NOT_FOUND:
        return L"Путь не найден.";
    case DICTIONARY_NOT_FOUND:
        return L"Словарь не найден.";
    case WORDS_LENGTH_NOT_EQUAL:
        return L"Слова разной длины.";
    case EMPTY_WORDS:
        return L"Слова нулевой длины.";
    }
    return L"Не известная ошибка.";
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
