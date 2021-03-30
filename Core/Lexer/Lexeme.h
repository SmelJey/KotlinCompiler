#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <any>

class Lexeme {
public:
    enum class LexemeType : int {
        EndOfFile = 0,
        Identifier = 1,
        Keyword = 2,

        Byte = 3,
        Short = 4,
        Int = 5,
        Long = 6,

        UByte = 7,
        UShort = 8,
        UInt = 9,
        ULong = 10,

        Float = 11,
        Double = 12,

        Operation = 13,
        Brace = 14,

        String = 15,
        RawString = 16,
        StringRef = 17,
        StringExpr = 18,
        CharLiteral = 19,

        Error = 20,
        Ignored = 21,
    };

    enum class NumberType {
        NotANumber,
        UInteger,
        Integer,
        Real
    };

    const static std::string DEFAULT_LEXEME_ERROR;

    static std::string LexemeToStr[];

    static NumberType GetNumberType(LexemeType lexemeType);

    Lexeme();
    Lexeme(int col, int row, std::string text, LexemeType type, const std::string& valueRepresentation);

    int GetColumn() const;
    int GetRow() const;
    const std::string& GetText() const;

    template<typename T>
    T GetValue() const {
        return std::any_cast<T>(myValue);
    }

    LexemeType GetType() const;

    std::string ToString() const;
private:
    std::string GetStringType() const;

    int myColumn;
    int myRow;
    std::string myText;
    std::any myValue;
    LexemeType myType;
};

std::ostream& operator<<(std::ostream& out, const Lexeme& lexeme);
