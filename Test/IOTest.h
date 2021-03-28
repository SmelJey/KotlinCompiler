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

        std::string token1 = NextToken(fileProcessor), token2 = NextTokenFromGold(goldStream);
        while (!token1.empty() && !token2.empty()) {
            CHECK(token1 == token2);
            outputStream << token1 << std::endl;
            token1 = NextToken(fileProcessor), token2 = NextTokenFromGold(goldStream);
        }
        CHECK(token1 == token2);
        while (!token1.empty()) {
            outputStream << token1 << std::endl;
            token1 = NextToken(fileProcessor);
        }

        outputStream.close();
    }
protected:
    virtual std::string NextTokenFromGold(std::ifstream& ifs) {
        std::string token;
        std::getline(ifs, token);
        return token;
    }
private:
    virtual std::string NextToken(FileProcessor& fileProcessor) = 0;

    std::string myFilepath;
};

#define IO_TEST(TestType, fileInput) \
    SECTION(fileInput) \
    TestType(fileInput).Run(); 