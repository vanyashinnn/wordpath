#include <fstream>
#include <locale>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "wordpath.h"
typedef std::multimap<int, String> WordsMap;
typedef std::pair<WordsMap::iterator, WordsMap::iterator> WordsMapEqualRange;

static int randomMinMax(int min, int max){
    return (rand()%(max - min + 1)) + min;
}

namespace {
    struct OzhoegovDict{
        void read(std::wifstream& in){
            std::getline(in, VOCAB, L'|');
            std::getline(in, BASEFORM, L'|');
            std::getline(in, PHONGL, L'|');
            std::getline(in, GRCLASSGL, L'|');
            std::getline(in, STYLGL, L'|');
            std::getline(in, DEF, L'|');
            std::getline(in, ANTI, L'|');
            std::getline(in, LEGLEXAM, L'\n');
        }
        std::wstring VOCAB;
        std::wstring BASEFORM;
        std::wstring PHONGL;
        std::wstring GRCLASSGL;
        std::wstring STYLGL;
        std::wstring DEF;
        std::wstring ANTI;
        std::wstring LEGLEXAM;
    };
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
        searchPath();
        postprocessing();
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
            while(true){
                StringList::iterator found = std::find(it, _words.end(), (*current));
                if(found != _words.end()){
                    to = found+1;
                    it = to;
                }else{
                    break;
                }
            }
            if(from != _words.end() && to != _words.end()){
                _words.erase(from, to);
            }
            ++current;
        }
    }
    bool chance(const int diff){
        int x = 100*(diff-2)/(_first.size()-1);
        bool ch = (randomMinMax(0, 100) <= x);
#ifdef DEBUG
        std::wcout << L"difference: " << diff << L"\n";
        std::wcout << L"_first.size(): " << _first.size() << L"\n";
        std::wcout << L"percent: " << x << L"\n";
        std::wcout << L"chance: " << ch << L"\n";
#endif
        return ch;
    }

    void searchPath(){
        int counter(0);
        while(true){
#ifdef DEBUG
            std::wcout << L"--------------search--------------\n";
            std::wcout << L"searchPath for " << _words.back() << L"\n";
#endif
            _nextWords.clear();
            int diff = WordPath::difference(_words.back(), _second);
            bool useRandom = chance(diff);
            for(const auto& word: _dictionary){
                if(WordPath::difference(word, _words.back()) == 1){
                    int pos = getFirstDiff(word, _words.back());
                    int w = 1;
                    if(!useRandom){
                        w = weight(word, pos);
                    }
                    _nextWords.insert(std::pair<int, String>(w, word));
                }
            }
#ifdef DEBUG
            std::wcout << L"Map:\n";
            for(WordsMap::iterator it = _nextWords.begin(); it != _nextWords.end(); ++it){
                std::wcout << it->first << L": " << it->second << L"(" <<WordPath::difference(it->second, _second)<< L")\n";
            }
#endif
            if(_nextWords.empty()){
                _status = WordPath::PATH_NOT_FOUND;
                return;
            }
            _equalRange = _nextWords.equal_range(_nextWords.rbegin()->first);
            _equalRangeList.clear();
            for(WordsMap::iterator it = _equalRange.first; it != _equalRange.second; ++it){
                _equalRangeList.push_back(it->second);
            }
            _words.push_back(_equalRangeList.at(randomMinMax(0, _equalRangeList.size() - 1)));
            if(_words.back() == _second){
                _status = WordPath::PATH_FOUND;
                return;
            }
            counter = (counter+1)%10;
            if(0 == counter){
                postprocessing();
            }
#ifdef DEBUG
            std::wcout << L"List:\n";
            for(const auto& word: _words){
                std::wcout << word << L"\n";
            }
#endif
        }
    }
    int getFirstDiff(const std::wstring& first, const std::wstring& second){
        for(int i=0; i<first.length(); ++i){
            if(first.at(i) != second.at(i)){
                return i;
            }
        }
        return -1;
    }

    bool isVowel(const wchar_t wc) const{
        static const std::wstring vowels = L"аяоёуюыиэе";
        return vowels.find(wc) != std::wstring::npos;
    }
    bool isConsonant(const wchar_t wc) const{
        static const std::wstring vowels = L"йцкнгшщзхфвпрлджчстб";
        return vowels.find(wc) != std::wstring::npos;
    }

    int weight(const String& word, const int pos) const{
#ifdef DEBUG
        std::wcout << _words.back() << L" => " << word << L" => " << _second;
#endif
        int w(0);
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
            if(isVowel(_words.back().at(i)) && isConsonant(word.at(i)) && isConsonant(_second.at(i))){
                w += 2;
            }
        }
        return w;
    }
    WordPath::Error readWords(const char * wordsFilename){
#ifdef DEBUG
            std::wcout << L"Чтение слов.\n";
#endif
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
        OzhoegovDict tmp;
        in.imbue(std::locale(RUS_LOCALE));
        while(true){
            tmp.read(in);
#ifdef DEBUG
            std::wcout << L"VOCAB: " << tmp.VOCAB << L"\n";
            std::wcout << L"DEF: " << tmp.DEF << L"\n";
#endif
            if(tmp.VOCAB.length() == _first.length()){
                _dictionary.insert(tmp.VOCAB);
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
    StringSet _dictionary;
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
