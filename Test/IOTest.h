#pragma once

#include "catch.hpp"

#include <string>
#include <fstream>

static std::string TestDirectory = "TestSamples/";

template<typename FileProcessor>
class IOTest {
public:
    virtual ~IOTest() = default;
    IOTest(const std::string& inputFilepath) : inputProcessor(TestDirectory + inputFilepath), goldStream(inputFilepath + ".gold") {}

    void Run() {
        std::string token1 = NextToken(inputProcessor), token2 = NextTokenFromGold(goldStream);
        while (!token1.empty() && !token2.empty()) {
            REQUIRE(token1 == token2);
        }
    }
protected:
    virtual std::string NextTokenFromGold(std::ifstream& ifs) {
        std::string token;
        while (token.empty() && !ifs.eof()) {
            std::getline(ifs, token);
        }

        return token;
    }
private:
    virtual std::string NextToken(FileProcessor& fileProcessor) = 0;

    FileProcessor inputProcessor;
    std::ifstream goldStream;
};

#define IO_TEST(TestType, fileInput) \
    TestType(fileInput).Run(); 