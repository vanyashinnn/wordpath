#include "gtest/gtest.h"
#include <vector>
#include "wordpath.h"

TEST(someTest, TestWordPath){
    std::vector<std::wstring> stringlist;
    std::wstring first(L"кот");
    std::wstring second(L"сыр");
    WordPath wordPath(first, second);
    stringlist = wordPath.words();

    WordPath::Error error = wordPath.status();

    switch(error){
        case WordPath::PATH_NOT_FOUND:
            EXPECT_EQ(stringlist.empty(), true);
            break;
        case WordPath::PATH_FOUND:
            EXPECT_EQ(stringlist.empty(), false);
            EXPECT_EQ(first, stringlist.front());
            EXPECT_EQ(second, stringlist.back());
            break;
        default:
            EXPECT_TRUE(false);//Сюда мы попасть не должны
    }
}

TEST(someTest, OneWord){
    std::vector<std::wstring> stringlist;
    std::wstring first(L"кот");
    EXPECT_EQ(first.length(), 3);

    WordPath wordPath(first, first);
    stringlist = wordPath.words();

    EXPECT_EQ(wordPath.status(), WordPath::PATH_FOUND);
    EXPECT_EQ(stringlist.size(), 1);
    EXPECT_EQ(first, stringlist.front());
    EXPECT_EQ(first, stringlist.back());
}
