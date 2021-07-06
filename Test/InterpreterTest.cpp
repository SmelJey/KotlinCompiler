#include "InterpreterTest.h"

#include <array>

#include <thread>
#include <filesystem>

#include "catch.hpp"


void InterpreterTest::RunTests(const std::string& directory) {
    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(InterpreterTestDirectory + directory)) {
        if (dirEntry.is_regular_file() && (!dirEntry.path().has_extension() || dirEntry.path().extension() == ".kt")) {
            SECTION(directory + dirEntry.path().filename().generic_string())
            Run(InterpreterTestDirectory + directory + dirEntry.path().filename().generic_string());
        }
    }
}

void InterpreterTest::Run(const std::string& fileName) {
    std::string fullpath = fileName;
    std::string outputExe = fullpath + ".exe";

    if (!std::filesystem::is_regular_file(outputExe)) {
        std::string output = RunFromShell("kotlinc -o " + WrapString(outputExe) + " " + WrapString(fullpath));
    }

    std::string goldRes = RunFromShell(std::filesystem::absolute(outputExe).generic_string());
    std::string res = RunFromShell(std::filesystem::absolute(InterpreterPath).generic_string() + " " + WrapString(fullpath));

    REQUIRE(!goldRes.empty());
    CHECK(goldRes == res);
}

std::string InterpreterTest::RunFromShell(const std::string& command) {
    std::array<char, 4096> buffer{};
    std::string output;
    auto handle = _popen(command.c_str(), "r");
    if (handle == nullptr) {
        throw;
    }
    std::size_t bytesRead;
    while ((bytesRead = fread(buffer.data(), sizeof(buffer[0]), sizeof(buffer), handle)) != 0) {
        output += std::string(buffer.data(), bytesRead);
    }

    _pclose(handle);
    return output;
}

std::string InterpreterTest::WrapString(const std::string& src) {
    return "\"" + src + "\"";
}
