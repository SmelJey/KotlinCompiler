#include "Lexeme.h"
#include <utility>
#include <sstream>
#include <regex>

const std::string Lexeme::DEFAULT_LEXEME_ERROR = "Uninitialized lexeme";

Lexeme::NumberType Lexeme::GetNumberType(LexemeType lexemeType) {
    if (lexemeType >= LexemeType::Byte && lexemeType <= LexemeType::Long) {
        return NumberType::Integer;
    }
    if (lexemeType >= LexemeType::UByte && lexemeType <= LexemeType::ULong) {
        return NumberType::UInteger;
    }
    if (lexemeType == LexemeType::Double || lexemeType == LexemeType::Float) {
        return NumberType::Real;
    }
    return NumberType::NotANumber;
}
Lexeme::Lexeme() : Lexeme(0, 0, "", LexemeType::Error, DEFAULT_LEXEME_ERROR) {}

Lexeme::Lexeme(int col, int row, std::string text, LexemeType lexemeType, const std::string& valueRepresentation)
    : myColumn(col), myRow(row), myText(std::move(text)), myType(lexemeType) {

    switch (GetNumberType(lexemeType)) {
        case NumberType::Integer:
        case NumberType::UInteger: {
            int base = 10;
            if (myText.size() > 1) {
                std::string prefix = myText.substr(0, 2);
                if (prefix == "0b") {
                    base = 2;
                } else if (prefix == "0x") {
                    base = 16;
                }
            }

            myValue.emplace<std::uint64_t>(std::stoull(valueRepresentation, 0, base));
            break;
        }
            
        case NumberType::Real:
            myValue = std::stod(valueRepresentation);
            break;
        default:
            myValue = valueRepresentation;
            break;
    }
}

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

    ss << myRow << "\t" << myColumn << "\t" << GetStringType() << "\t" << myText << "\t";
    if (myText.size() < 8) {
        ss << "\t";
    }

    switch (GetNumberType(myType)) {
        case NumberType::Real:
            ss << GetValue<double>();
            break;
        case NumberType::UInteger:
        case NumberType::Integer:
            ss << GetValue<uint64_t>();
            break;
        default:
            ss << GetValue<std::string>();
            break;
    }
    return ss.str();
}

std::string Lexeme::LexemeToStr[]{ "EOF", "Word", "Key",
    "Byte", "Short", "Int", "Long",
    "UByte", "UShort", "UInt", "ULong",
    "Float", "Double", "Op", "Br",
    "Str", "RStr", "StrRf", "StrExp", "Char",
    "Error", "Ignored" };

std::string Lexeme::GetStringType() const {
    return LexemeToStr[(int)myType];
}
