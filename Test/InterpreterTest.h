#pragma once
#include <string>

const static std::string InterpreterTestDirectory = "TestSamples/InterpreterTests/";
const static std::string InterpreterPath = "../Release/KotlinCompiler.exe";

class InterpreterTest {
public:
    static void RunTests(const std::string& directory);
    static void Run(const std::string& fileName);

private:
    static std::string RunFromShell(const std::string& command);
    static std::string WrapString(const std::string& src);
};