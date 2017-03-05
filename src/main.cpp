#include <locale>
#include <iostream>
#include <fstream>
#include "wordpath.h"
bool checkFile(const char * filename){
    std::ifstream in(filename);
    if(!in){
        std::wcout << L"\033[31mФайл: " << filename << L" не найден!\033[0m\n";
        return false;
    }
    in.close();
    return true;
}

int main(int argc, char* argv[]){
    // set locale
    setlocale(LC_ALL, RUS_LOCALE);
    std::wcout.imbue(std::locale(RUS_LOCALE));
    std::wcin.imbue(std::locale(RUS_LOCALE));

    // checks
    if(argc != 3){
        std::wcout << L"\033[31mИспользование: " << argv[0] << L" <путь/до/файла/со/словами> <путь/до/файла/со/словарем>\033[0m\n";
        return 1;
    }
    const char* wordsFilename(argv[1]);
    const char* dictionaryFilename(argv[2]);
    if(!checkFile(wordsFilename) || !checkFile(dictionaryFilename)){
        return 1;
    }
    String first;
    String second;

    std::wifstream wordsFile(wordsFilename);
    wordsFile.imbue(std::locale(RUS_LOCALE));
    wordsFile >> first >> second;
    wordsFile.close();

    // run program
    WordPath wordPath(first, second, dictionaryFilename);

    if(WordPath::PATH_FOUND != wordPath.status()) {
        std::wcout << L"\033[32m" << wordPath.statusText() << L"\033[0m\n";
        return 0;
    }
    const StringList& words = wordPath.words();
    for(const auto& word: words){
        std::wcout << word << L"\n";
    }
    return 0;
}
