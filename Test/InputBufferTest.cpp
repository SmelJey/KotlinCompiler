#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sstream>

#include "Lexer/InputBuffer.h"

TEST_CASE("InputBufferTest", "[InputBuffer]") {
    std::stringstream ss;

    SECTION("Simple Test") {
        std::string testString = "test string blabla";
        ss << testString;
        InputBuffer<std::stringstream> inputBuffer(testString);

        std::string newString;

        int curChar = inputBuffer.NextChar();
        while (curChar != InputBuffer<std::stringstream>::EOF_VAL) {
            newString.push_back(curChar);
            curChar = inputBuffer.NextChar();
        }

        REQUIRE(testString == newString);
    }
    SECTION("Short Buffer Test") {
        std::string testString = "test string blabla 123456 325345 23 536 4 645 634 643 6347 87 sfdgsdgsdfgs wr4t326 3425235";
        ss << testString;
        InputBuffer<std::stringstream> inputBuffer(testString, 20);

        std::string newString;

        int curChar = inputBuffer.NextChar();
        while (curChar != InputBuffer<std::stringstream>::EOF_VAL) {
            newString.push_back(curChar);
            curChar = inputBuffer.NextChar();
        }

        REQUIRE(testString == newString);
    }
    SECTION("Character Return Test") {
        std::string testString = "test string blabla 123456 325345 23 536 4 645 634 643 6347 87 sfdgsdgsdfgs wr4t326 3425235";
        ss << testString;
        InputBuffer<std::stringstream> inputBuffer(testString, 10);

        std::string newString;

        int curChar = inputBuffer.NextChar();

        for (int i = 0; i < 25; i++) {
            newString.push_back(curChar);
            curChar = inputBuffer.NextChar();
        }

        std::string tmp = testString.substr(0, 25);
        REQUIRE(newString == testString.substr(0, 25));
        inputBuffer.ReturnChar(curChar);

        for (int i = 0; i < 20; i++) {
            inputBuffer.ReturnChar(*newString.rbegin());
            newString.pop_back();
        }

        REQUIRE(newString == testString.substr(0, 5));

        curChar = inputBuffer.NextChar();

        while (curChar != InputBuffer<std::stringstream>::EOF_VAL) {
            newString.push_back(curChar);
            curChar = inputBuffer.NextChar();
        }

        REQUIRE(testString == newString);
    }
}
