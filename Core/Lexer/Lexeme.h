#pragma once

#include <string>
#include <fstream>
#include <unordered_map>

class Lexeme {
public:
    enum class LexemeType : int {
        EndOfFile = 0,
        Identifier = 1,
        Number = 2,
        Operation = 3,
        Separator = 4,
        String = 5,
        RawString = 6,
        Error = 7,
    };

    static std::string LexemeToStr[];
    /*static std::unordered_map<std::string, LexemeType> StrToLexeme;*/

    Lexeme();
    Lexeme(int col, int row, std::string text, LexemeType type, bool isComplete = true);

    int GetColumn() const;
    int GetRow() const;
    const std::string& GetText() const;
    LexemeType GetType() const;
    bool IsComplete() const;

    std::string ToString() const;
private:
    std::string GetStringType() const;

    int myColumn;
    int myRow;
    std::string myText;
    LexemeType myType;
    bool isComplete;
};

std::ostream& operator<<(std::ostream& out, const Lexeme& lexeme);
