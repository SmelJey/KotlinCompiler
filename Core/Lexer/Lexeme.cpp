#include "Lexeme.h"

#include <sstream>
#include <utility>
#include "../magic_enum.hpp"

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
Lexeme::Lexeme() : Lexeme(0, 0, "", LexemeType::Error, DEFAULT_LEXEME_ERROR, true) {}

Lexeme::Lexeme(int col, int row, std::string text, LexemeType lexemeType, const std::string& valueRepresentation, bool isError)
    : myColumn(col), myRow(row), myText(std::move(text)), myType(lexemeType), isError(isError) {

    switch (GetNumberType(lexemeType)) {
        case NumberType::Integer:
        case NumberType::UInteger: {
            if (isError) {
                myValue.emplace<std::pair<std::any, std::string>>(0ull, valueRepresentation);
            } else {
                myValue.emplace<std::uint64_t>(std::stoull(valueRepresentation));
            }
            
            break;
        }
            
        case NumberType::Real:
            if (isError) {
                myValue.emplace<std::pair<std::any, std::string>>(0.0, valueRepresentation);
            } else {
                myValue.emplace<double>(std::stod(valueRepresentation));
            }
            
            break;
        default:
            if (isError) {
                myValue.emplace<std::pair<std::any, std::string>>(text, valueRepresentation);
            } else {
                myValue = valueRepresentation;
            }
            
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

bool Lexeme::IsError() const {
    return isError;
}

Lexeme::LexemeType Lexeme::GetType() const {
    return myType;
}

std::string Lexeme::ToString() const {
    std::stringstream ss;

    std::string typeStr = GetStringType() + (isError ? "!" : "");
    ss << myRow << "\t" << myColumn << "\t" << typeStr << "\t";
    if (typeStr.size() < 8) {
        ss << "\t";
    }

    ss << myText << "\t";
    if (myText.size() < 8) {
        ss << "\t";
    }

    if (isError) {
        ss << GetValue<std::string>();
    } else {
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
    }
    
    return ss.str();
}

Lexeme Lexeme::CopyEmpty() const {
    return CopyEmptyOfType(myType);
}

Lexeme Lexeme::CopyEmptyOfType(LexemeType type) const {
    return Lexeme(myColumn, myRow, "", type, "");
}

std::string Lexeme::LexemeToStr[]{ "EOF", "Word", "Key",
    "Byte", "Short", "Int", "Long",
    "UByte", "UShort", "UInt", "ULong",
    "Float", "Double", "Op", "Br",
    "Str", "RStr", "StrRf", "StrExp", "Char",
    "Error", "Ignored" };

std::string Lexeme::GetStringType() const {
    auto tmp = magic_enum::enum_name(myType);
    return tmp.data();
}