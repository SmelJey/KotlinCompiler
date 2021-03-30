#pragma once

#include "catch.hpp"

#include <string>
#include <fstream>
#include <iostream>

const static std::string TestDirectory = "TestSamples/";

template<typename FileProcessor>
class IOTest {
public:
    virtual ~IOTest() = default;
    IOTest(const std::string& inputFilepath) : myFilepath(TestDirectory + inputFilepath) {
        std::cout << myFilepath;
    }

    void Run() {
        FileProcessor fileProcessor(myFilepath);
        fileProcessor.Init();
        std::ifstream goldStream(myFilepath + ".gold");
        std::ofstream outputStream(myFilepath + ".gold.last");

        REQUIRE(!goldStream.eof());

        std::string token1 = NextToken(fileProcessor), token2 = NextTokenFromGold(goldStream);
        while (!token1.empty() && !token2.empty()) {
            CHECK(token1 == token2);
            outputStream << "@@@" << token1 << std::endl;
            token1 = NextToken(fileProcessor), token2 = NextTokenFromGold(goldStream);
        }
        CHECK(token1 == token2);
        while (!token1.empty()) {
            outputStream << "@@@" << token1 << std::endl;
            token1 = NextToken(fileProcessor);
        }

        outputStream.close();
    }
protected:
    virtual std::string NextTokenFromGold(std::ifstream& ifs) {
        std::string token = myLastToken;
        if (token.empty()) {
            std::getline(ifs, token);
        }
        myLastToken = "";

        while (std::getline(ifs, myLastToken)) {
            if (myLastToken.size() >= 3 && myLastToken.substr(0, 3) == "@@@") {
                break;
            }

            token.append("\n");
            token.append(myLastToken);
        }

        if (token.empty()) {
            return token;
        }

        return token.substr(3);
    }
private:
    virtual std::string NextToken(FileProcessor& fileProcessor) = 0;

    std::string myFilepath;
    std::string myLastToken;
};

#define IO_TEST(TestType, fileInput) \
    SECTION(fileInput) \
    TestType(fileInput).Run(); 