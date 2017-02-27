#include "gtest/gtest.h"
#include <vector>
#include "wordpath.h"

TEST(someTest, TestWordPath){
  std::vector<std::string> stringlist;
  std::string first("кот");
  std::string second("сыр");
  WordPath wordPath(first, second);
  stringlist = wordPath.words();
  if(stringlist.empty()){
    return;
  }
  EXPECT_EQ(first, stringlist.front());
  EXPECT_EQ(second, stringlist.back());
}