#pragma once

#include "IOTest.h"
#include "Lexer/Lexer.h"

const static std::string LexerDirectory = "LexerTests/";

class LexerTest : public IOTest {
public:
    static std::string CreateTestPath(const std::string& path);

    explicit LexerTest(const std::string& filepath);
private:
    std::string NextToken() override;

    std::unique_ptr<Lexer> myLexer;
};

#define LEXER_TEST(input) \
    IO_TEST(LexerTest, input)

const static std::string ParserDirectory = "ParserTests/";

class ParserTest : public IOTest {
public:
    static std::string CreateTestPath(const std::string& path);

    explicit ParserTest(const std::string& filepath);
private:
    std::string NextToken() override;

    std::vector<std::string> myTokens;
    size_t myIdx = 0;
};

#define PARSER_TEST(input) \
    IO_TEST(ParserTest, input)