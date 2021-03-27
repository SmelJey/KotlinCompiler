#pragma once

#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>

class LexerState;

class Lexeme {
public:
    enum class LexemeType : int {
        EndOfFile = 0,
        Identifier = 1,
        Number = 2,
        Operation = 3,
        Separator = 4,
        String = 5,
        Incomplete = 6,
        Ignored = 7,
        Error = 8,
    };

    static std::string LexemeToStr[];
    /*static std::unordered_map<std::string, LexemeType> StrToLexeme;*/

    static std::unordered_set<std::string> LongOperatorsSet;

    Lexeme();
    Lexeme(int col, int row, std::string text, LexemeType type);

    int GetColumn() const;
    int GetRow() const;
    const std::string& GetText() const;
    LexemeType GetType() const;

    LexerState& TryToMerge(const Lexeme& src);

    std::string ToString() const;
private:
    std::string GetStringType() const;

    int myColumn;
    int myRow;
    std::string myText;
    LexemeType myType;
};

std::ostream& operator<<(std::ostream& out, const Lexeme& lexeme);
