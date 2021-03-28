#include "Lexeme.h"
#include <utility>
#include <sstream>

Lexeme::Lexeme() : Lexeme(0, 0, "", LexemeType::Error, false) {};

Lexeme::Lexeme(int col, int row, std::string text, LexemeType lexemeType, bool isComplete)
    : myColumn(col), myRow(row), myText(std::move(text)), myType(lexemeType), isComplete(isComplete) {}

std::ostream& operator<<(std::ostream& out, const Lexeme& lexeme) {
    out << lexeme.ToString();
    return out;
}

int Lexeme::GetColumn() const {
    return myColumn;
}

int Lexeme::GetRow() const {
    return myRow;
}

const std::string& Lexeme::GetText() const {
    return myText;
}

Lexeme::LexemeType Lexeme::GetType() const {
    return myType;
}

std::string Lexeme::ToString() const {
    std::stringstream ss;
    ss << myRow << "\t" << myColumn << "\t" << GetStringType() + (isComplete ? "" : "!") << "\t" << myText;
    return ss.str();
}

std::string Lexeme::LexemeToStr[]{ "EOF", "Word", "Num", "Op", "Sep", "Str", "Error" };

//std::unordered_map<std::string, Lexeme::LexemeType> Lexeme::StrToLexeme {
//    {"EOF", LexemeType::EndOfFile},
//    {"Word", LexemeType::Identifier },
//    {"" }
//};

std::string Lexeme::GetStringType() const {
    return LexemeToStr[(int)myType];
}
