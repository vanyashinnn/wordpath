#include "gtest/gtest.h"
#include <vector>
#include "wordpath.h"

TEST(someTest, TestWordPath){
  std::vector<std::string> stringlist;
  std::string first("кот");
  std::string second("сыр");
  WordPath wordPath(first, second);
  stringlist = wordPath.words();

  WordPath::Error error = wordPath.status();

  if(stringlist.empty()){
    EXPECT_EQ(error, WordPath::PATH_NOT_FOUND);
    return;
  }
  EXPECT_EQ(first, stringlist.front());
  EXPECT_EQ(second, stringlist.back());
}
