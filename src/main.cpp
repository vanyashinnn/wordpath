#include <locale>
#include <iostream>
#include <fstream>
#include "wordpath.h"
int main(int argc, char* argv[]){
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
    std::wcin.imbue(std::locale("ru_RU.UTF-8"));

    if(argc != 3){
        std::cout << "\033[32mUse: " << argv[0] << " <path/to/file/with/words> <path/to/file/with/dictionary>\033[0m\n";
        return 1;
    }

    WordPath wordPath(L"муха", L"слон");
    wordPath.test();
    return 0;
}
