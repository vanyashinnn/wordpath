#include <iostream>
int main(int argc, char* argv[]){
    if(argc != 3){
        std::cout << "\033[32mUse: " << argv[0] << " <path/to/file/with/words> <path/to/file/with/dictionary>\033[0m\n";
        return 1;
    }
    return 0;
}
