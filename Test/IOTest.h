#pragma once

#include <string>

#include "catch.hpp"

const static std::string TestDirectory = "TestSamples/";

class IOTest {
public:
    virtual ~IOTest();

    explicit IOTest(const std::string& inputFilepath);

    void Run();

protected:
    virtual std::string NextTokenFromGold(std::ifstream& ifs);

    std::string GetFilepath() const;
   
private:
    virtual std::string NextToken() = 0;

    std::string myFilepath;
    std::string myLastToken;
};

#define IO_TEST(TestType, fileInput) \
    SECTION(fileInput) \
    TestType(fileInput).Run(); 