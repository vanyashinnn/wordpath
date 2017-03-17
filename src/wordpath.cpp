#include <fstream>
#include <locale>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#ifdef STATUSBAR
#include <iomanip>
#endif

#include "wordpath.h"
typedef std::multimap<int, String> WordsMap;
typedef std::pair<WordsMap::iterator, WordsMap::iterator> WordsMapEqualRange;

static int randomMinMax(int min, int max){
    return (rand()%(max - min + 1)) + min;
}

class WordPath::Impl{
public:
    explicit Impl(const String& first, const String& second, const char * wordsFilename):
        _first(first),
        _second(second)
    {
        srand(time(0));
        _dictionary.insert(first);
        _dictionary.insert(second);
        _words.push_back(first);
        _status = readWords(wordsFilename);
        if(WordPath::PATH_NOT_FOUND != _status){
            return;
        }
        _difference = first.length();
        searchPath();
    }
    void postprocessing(){
        StringList::iterator current = _words.begin();
        while(true){
            if(current == _words.end()){
                break;
            }
            StringList::iterator from = current+1;
            StringList::iterator to = _words.end();
            StringList::iterator it = from;
            for(it = from; it != _words.end(); ++it){
                if(WordPath::difference(*it, *current) == 1){
                    to = it;
                }
            }
            if(from != _words.end() && to != _words.end()){
                _words.erase(from, to);
            }
            ++current;
        }
    }
    bool chance(const int diff){
        int x = 100*(diff-1)/(_first.size()-1);
        bool ch = (randomMinMax(0, 100) <= x);
        return ch;
    }

    void searchPath(){
        int maxPath = 1000;
        int maxAttempts = 100;
#ifdef STATUSBAR
        int statusBarSize = 35;
#endif
        for(int i=0; i<maxAttempts; ++i){
#ifdef STATUSBAR
            int percent = (i*100)/maxAttempts;
            int currentBar = (i*statusBarSize)/maxAttempts;
            std::wcout << L"\033[32mПоиск: (" << std::setfill(L' ') << std::setw(3) << percent <<"%)[";
            for(int p=0; p<statusBarSize; ++p){
                if(p <= currentBar){
                    std::wcout << L"#";
                }else{
                    std::wcout << L" ";
                }
            }
            std::wcout << L"]\033[0m\n";
#endif
            _words.clear();
            _words.push_back(_first);
            for(int count=0; count<maxPath; ++count){
                _nextWords.clear();
                int diff = WordPath::difference(_words.back(), _second);
                bool useRandom = chance(diff);
                for(const auto& word: _dictionary){
                    if(WordPath::difference(word, _words.back()) == 1){
                        int w = 1;
                        if(!useRandom){
                            w = weight(word);
                        }
                        _nextWords.insert(std::pair<int, String>(w , word));
                    }
                }
                if(_nextWords.empty()){
                    break;
                }
                _equalRange = _nextWords.equal_range(_nextWords.rbegin()->first);
                _equalRangeList.clear();
                for(WordsMap::iterator it = _equalRange.first; it != _equalRange.second; ++it){
                    _equalRangeList.push_back(it->second);
                }
                _words.push_back(_equalRangeList.at(randomMinMax(0, _equalRangeList.size() - 1)));
                if(_words.back() == _second){
                    _status = WordPath::PATH_FOUND;
                    break;
                }
            }
            postprocessing();
#ifdef DEBUG
            std::wcout << L"------------(" << (i+1) << L")------------\n";
            for(const auto& word: _words){
                std::wcout << word << L" (" << WordPath::difference(word, _second) << L")\n";
            }
#endif
#ifdef STATUSBAR
            std::wcout << L"\033[1A";
            for(int p=0; p<(statusBarSize+20); ++p){
                std::wcout << L" ";
            }
            std::wcout << L"\n\033[1A";
#endif
            if(WordPath::PATH_FOUND == _status){
                break;
            }
        }
    }
    bool isVowel(const wchar_t wc) const{
        static const std::wstring vowels = L"аяоёуюыиэе";
        return vowels.find(wc) != std::wstring::npos;
    }
    bool isConsonant(const wchar_t wc) const{
        static const std::wstring vowels = L"йцкнгшщзхфвпрлджчстб";
        return vowels.find(wc) != std::wstring::npos;
    }

    int weight(const String& word) const{
#ifdef DEBUG
        std::wcout << _words.back() << L" => " << word << L" => " << _second << L"\n";
#endif
        int w(_first.length()-WordPath::difference(word, _second));
        for(int i=0; i<word.length(); ++i){
            if(word.at(i) == _second.at(i)){
                w += 3;
            }
            if(isVowel(word.at(i)) && isVowel(_second.at(i))){
                w += 2;
            }
            if(isConsonant(word.at(i)) && isConsonant(_second.at(i))){
                w += 2;
            }
            if(isConsonant(_words.back().at(i)) && isVowel(word.at(i)) && isVowel(_second.at(i))){
                w += 2;
            }
            if(isVowel(word.at(i))){
                ++w;
            }
            if(isVowel(_words.back().at(i)) && isConsonant(word.at(i)) && isConsonant(_second.at(i))){
                w += 2;
            }
        }
        return w;
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
        std::wstring tmp;

        in.imbue(std::locale(RUS_LOCALE));
        while(true){
            in >> tmp;
            if(tmp.length() == _first.length()){
                _dictionary.insert(tmp);
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
    int _difference;
    String _first;
    String _second;
    StringList _words;
    StringSet _dictionary;
    WordsMap _nextWords;
    WordPath::Error _status;
};

WordPath::WordPath(const String& first, const String& second, const char * wordsFilename):
    pimpl(new Impl(first, second, wordsFilename))
{
}

WordPath::~WordPath()
{
    delete pimpl;
    pimpl = 0;
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
