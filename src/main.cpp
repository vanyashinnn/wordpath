#include <locale>
#include <iostream>
#include <fstream>
#include "wordpath.h"
int main(int argc, char* argv[]){
    setlocale(LC_ALL, RUS_LOCALE);
    std::wcout.imbue(std::locale(RUS_LOCALE));
    std::wcin.imbue(std::locale(RUS_LOCALE));

    if(argc != 3){
        std::cout << "\033[32mUse: " << argv[0] << " <path/to/file/with/words> <path/to/file/with/dictionary>\033[0m\n";
        return 1;
    }

    WordPath wordPath(L"муха", L"слон");
    wordPath.test();
    return 0;
}
