#include "Lexeme.h"
#include "LexerStates.h"

#include <utility>
#include <sstream>

Lexeme::Lexeme() : Lexeme(0, 0, "", LexemeType::Ignored) {};

Lexeme::Lexeme(int col, int row, std::string text, LexemeType lexemeType) : myColumn(col), myRow(row), myText(std::move(text)), myType(lexemeType) {}

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
            if (myText == "//" || myText == "!#") {
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

    return BadState::Instance();
}

std::string Lexeme::ToString() const {
    std::stringstream ss;
    ss << myRow << "\t" << myColumn << "\t" << GetStringType() << "\t" << myText;
    return ss.str();
}

std::string Lexeme::LexemeToStr[] { "EOF", "Word", "Num", "Op", "Sep", "Str", "Incomplete", "Ignored", "Error" };

//std::unordered_map<std::string, Lexeme::LexemeType> Lexeme::StrToLexeme {
//    {"EOF", LexemeType::EndOfFile},
//    {"Word", LexemeType::Identifier },
//    {"" }
//};

std::string Lexeme::GetStringType() const {
    return LexemeToStr[(int)myType];
}
