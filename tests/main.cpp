#include "gtest/gtest.h"
#include "wordpath.h"

int main(int argc, char** argv) {
    setlocale(LC_ALL, RUS_LOCALE);
    std::wcout.imbue(std::locale(RUS_LOCALE));
    std::wcin.imbue(std::locale(RUS_LOCALE));
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
 
