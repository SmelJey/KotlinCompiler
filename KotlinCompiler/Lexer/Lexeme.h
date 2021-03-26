#pragma once

#include <string>
#include <fstream>
#include <unordered_set>

class LexerState;

class Lexeme {
public:
    enum class LexemeType {
        EndOfFile,
        Identifier,
        Digit,
        Operation,
        Separator,
        String,
        Incomplete,
        Ignored,
        Error,
    };

    static std::unordered_set<std::string> LongOperatorsSet;

    Lexeme();
    Lexeme(int col, int row, std::string text, LexemeType type);

    friend std::ostream& operator<<(std::ostream& out, const Lexeme& lexeme);

    int GetColumn() const;
    int GetRow() const;
    const std::string& GetText() const;
    LexemeType GetType() const;

    LexerState& TryToMerge(const Lexeme& src);
private:
    std::string GetStringType() const;

    int myColumn;
    int myRow;
    std::string myText;
    LexemeType myType;
};
