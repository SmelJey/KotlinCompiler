#pragma once

#include <string>
#include <fstream>
#include <unordered_map>

class Lexeme {
public:
    enum class LexemeType : int {
        EndOfFile = 0,
        Identifier = 1,
        Keyword = 2,
        Number = 3,
        Operation = 4,
        Brace = 5,
        String = 6,
        RawString = 7,
        StringRef = 8,
        RawStringRef = 9,
        StringExpr = 10,
        RawStringExpr = 11,
        CharLiteral = 12,
        Error = 13,
        Ignored = 14,
    };

    static std::string LexemeToStr[];
    /*static std::unordered_map<std::string, LexemeType> StrToLexeme;*/

    Lexeme();
    Lexeme(int col, int row, std::string text, LexemeType type, std::string errorDescription = "");

    int GetColumn() const;
    int GetRow() const;
    const std::string& GetText() const;
    const std::string& GetError() const;
    LexemeType GetType() const;

    std::string ToString() const;
private:
    std::string GetStringType() const;

    int myColumn;
    int myRow;
    std::string myText;
    std::string myError;
    LexemeType myType;
};

std::ostream& operator<<(std::ostream& out, const Lexeme& lexeme);
