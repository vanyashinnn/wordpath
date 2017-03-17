#include "gtest/gtest.h"
#include <vector>
#include "wordpath.h"

TEST(someTest, TestWordPath){
    std::wstring first(L"муха");
    std::wstring second(L"слон");
    WordPath wordPath(first, second);
    const StringList& stringlist = wordPath.words();

    WordPath::Error error = wordPath.status();

    switch(error){
        case WordPath::PATH_FOUND:
            EXPECT_EQ(stringlist.empty(), false);
            EXPECT_EQ(first, stringlist.front());
            EXPECT_EQ(second, stringlist.back());
            for(int i=0; i<stringlist.size()-1; ++i){
                EXPECT_EQ(WordPath::difference(stringlist.at(i), stringlist.at(i+1)), 1);
            }
            break;
        case WordPath::PATH_NOT_FOUND:
        case WordPath::DICTIONARY_NOT_FOUND:
        case WordPath::WORDS_LENGTH_NOT_EQUAL:
        case WordPath::EMPTY_WORDS:
            EXPECT_EQ(stringlist.empty(), true);
            break;
        default:
            EXPECT_TRUE(false);//Сюда мы попасть не должны
    }
}

TEST(someTest, OneWord){
    std::wstring first(L"кот");
    EXPECT_EQ(first.length(), 3);

    WordPath wordPath(first, first);
    const StringList& stringlist = wordPath.words();

    EXPECT_EQ(wordPath.status(), WordPath::PATH_FOUND);
    EXPECT_EQ(stringlist.size(), 1);
    EXPECT_EQ(first, stringlist.front());
    EXPECT_EQ(first, stringlist.back());
}

TEST(someTest, TestLength){
    WordPath wordPath(L"кот", L"слон");
    EXPECT_EQ(wordPath.status(), WordPath::WORDS_LENGTH_NOT_EQUAL);
}
TEST(someTest, TestDistance){
    EXPECT_EQ(WordPath::difference(L"ййй", L"ццц"), 3);
    EXPECT_EQ(WordPath::difference(L"ййй", L"йцц"), 2);
    EXPECT_EQ(WordPath::difference(L"ййй", L"цйц"), 2);
    EXPECT_EQ(WordPath::difference(L"ййй", L"ццй"), 2);
    EXPECT_EQ(WordPath::difference(L"ййй", L"цйй"), 1);
    EXPECT_EQ(WordPath::difference(L"ййй", L"йцй"), 1);
    EXPECT_EQ(WordPath::difference(L"ййй", L"ййц"), 1);
    EXPECT_EQ(WordPath::difference(L"ййй", L"ййй"), 0);
    EXPECT_EQ(WordPath::difference(L"йцу", L"йцк"), 1);
    EXPECT_EQ(WordPath::difference(L"йцу", L"йцк"), 1);
    EXPECT_EQ(WordPath::difference(L"кот", L"слон"), -1);
}
