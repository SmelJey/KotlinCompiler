#include "Lexeme.h"
#include "LexerStates.h"

#include <utility>

Lexeme::Lexeme() : Lexeme(0, 0, "", LexemeType::Ignored) {};

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
    ">=", "<=", "==", "!=", "===", "!==",
    "->", "=>",
    "//", "/*", "*/"};

LexerState& Lexeme::TryToMerge(const Lexeme& src) {
    StartState::Instance();

    if (myType == LexemeType::Ignored) {
        std::string prefix = myText.substr(0, 2);
        if (prefix == "//") {
            if (myRow != src.myRow) {
                return BadState::Instance();
            }

            myText += src.myText;

            return InCommentState::Instance();
        }

        if (prefix == "/*") {
            if (myText.substr(myText.size() - 2) == "*/") {
                return BadState::Instance();
            }

            myText += src.myText;
            if (myText.substr(myText.size() - 2) == "*/") {
                return StartState::Instance();
            }
            return InOperationState::Instance();
        }
    }

    if (myType == LexemeType::Operation) {
        if (myRow != src.myRow || myColumn + myText.size() != src.myColumn) {
            return BadState::Instance();
        }

        std::string mergedText = myText + src.myText;
        if (LongOperatorsSet.count(mergedText)) {
            myText = mergedText;
            if (myText == "//") {
                myType = LexemeType::Ignored;
                return InCommentState::Instance();
            }
            if (myText == "/*") {
                myType = LexemeType::Ignored;
                return InMultilineCommentState::Instance();
            }
            return InOperationState::Instance();
        }
    }

    return BadState::Instance();;
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



