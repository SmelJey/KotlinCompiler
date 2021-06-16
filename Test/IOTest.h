#pragma once

#include <filesystem>
#include <string>

#include "catch.hpp"

const static std::string TestDirectory = "TestSamples/";

class IOTest {
public:
    static std::string CreateTestPath(const std::string& path);

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
    TestType(fileInput).Run()

template<class T>
void RunTests(std::string path) {
    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(T::CreateTestPath(path))) {
        if (dirEntry.is_regular_file() && (!dirEntry.path().has_extension() || dirEntry.path().extension() == ".kt")) {
            IO_TEST(T, path + dirEntry.path().filename().generic_string());
        }
    }
}