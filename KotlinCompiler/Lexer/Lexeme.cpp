#include "Lexeme.h"

#include <utility>

Lexeme::Lexeme(int col, int row, std::string text, LexemeType lexemeType) : myColumn(col), myRow(row), myText(std::move(text)), myType(lexemeType) {}

std::ostream& operator<<(std::ostream& out, const Lexeme& lexeme) {
    out << lexeme.myRow << "\t" << lexeme.myColumn << "\t" << lexeme.GetStringType() << "\t" << lexeme.myText;
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

std::unordered_set<std::string> Lexeme::LongOperatorsSet { "++", "--",
    "&&", "||",
    "+=", "-=", "*=", "%=", "/=",
    "..", "...", "::", ";;",
    ">=", "<=", "==", "!=", "===", "!=="};

bool Lexeme::TryToMerge(const Lexeme& src) {
    if (myRow != src.myRow || myColumn + myText.size() != src.myColumn) {
        return false;
    }

    if (myType == LexemeType::Operation && src.myType == LexemeType::Operation) {
        if (src.myText.size() > 1) {
            return false;
        }

        std::string mergedText = myText + src.myText;
        if (LongOperatorsSet.count(mergedText)) {
            myText = mergedText;
            return true;
        }
    }

    return false;
}

std::string Lexeme::GetStringType() const {
    switch (myType) {
        case LexemeType::Identifier:
            return "Word";
        case LexemeType::Digit:
            return "Num";
        case LexemeType::EndOfFile:
            return "EOF";
        case LexemeType::Error:
            return "Err";
        case LexemeType::Ignored:
            return "Ignored";
        case LexemeType::Incomplete:
            return "Incomplete";
        case LexemeType::Operation:
            return "Op";
        case LexemeType::Separator:
            return "Sep";
        case LexemeType::String:
            return "Str";
        default:
            return "Unknown";
    }
}



