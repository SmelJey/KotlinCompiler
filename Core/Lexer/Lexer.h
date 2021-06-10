#pragma once

#include "LexerUtils.h"
#include "../InputBuffer.h"

#include <functional>
#include <queue>
#include <string>

using LexType = Lexeme::LexemeType;

class Lexer {
public:
    explicit Lexer(std::ifstream& input);
    explicit Lexer(const std::string& filepath);

    Lexeme GetLexeme() const;

    Lexeme NextLexeme();
    Lexeme PrevLexeme();

private:
    Lexeme NextFromInput();

    void ProcessNextLexeme();

    void ProcessWhitespaces();

    void ProcessComment();
    void ProcessMultilineComment();

    void ProcessIdentifier();
    void ProcessEscapedIdentifier();

    void ProcessNumber();
    bool ProcessIntegerNumber(std::function<bool(int)> isNumber);
    void ProcessPrefixNumber(std::function<bool(int)> isNumber);

    void ProcessChar();

    void ProcessString();
    void ProcessRawString();
    bool ProcessStringTemplate(LexType stringType);
    void ReturnCurrentStringLexeme(bool unlockString);
    void ProcessStrExpression();

    void ProcessOperation();

    void ProcessUnknown();
    void ConsumeLexeme();

    int GetNextChar();
    void AddNextChar(int cnt = 1);

    void ResetLexeme();

    void MakeError(const std::string& errorMessage);
         
    std::size_t myCol = 0;
    std::size_t myRow = 0;

    std::size_t myStartCol = 0;
    std::size_t myStartRow = 0;

    InputBuffer<std::ifstream> myInputBuffer;
    std::deque<Lexeme> myLexemeBuffer;

    Lexeme myCurrentLexeme;
    Lexeme myPreviousLexeme = Lexeme(-1, -1, "", Lexeme::LexemeType::EndOfFile, "", true);

    std::string myLexemeText;
    std::string myLexemeValue;

    LexType myLexemeType = LexType::Error;
    bool isError = false;
    bool isInString = false;
};
