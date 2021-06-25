#pragma once

#include <any>
#include <fstream>
#include <string>

class Lexeme {
public:
    friend class Lexer;

    enum class LexemeType : int {
        EndOfFile,
        Identifier,
        Keyword,

        Byte,
        Short,
        Int,
        Long,

        UByte,
        UShort,
        UInt,
        ULong,

        Float,
        Double,

        OpAdd,
        OpSub,
        OpMult,
        OpDiv,
        OpMod,

        OpAssign,
        OpExclMark,
        OpComma,
        OpDot,
        OpColon,
        OpSemicolon,
        OpHash,
        OpAt,
        OpQuestMark,
        OpInc,
        OpDec,

        OpAnd,
        OpOr,

        OpPlusAssign,
        OpMinusAssign,
        OpMultAssign,
        OpDivAssign,
        OpModAssign,
        OpArrow,
        OpDArrow,
        OpDDot,
        OpDColon,
        OpDSemicolon,
        OpTripleDot,

        OpLess,
        OpGreater,
        OpLessOrEq,
        OpGreaterOrEq,
        OpInequal,
        OpEqual,
        OpStrictIneq,
        OpStrictEq,

        OpIn,
        OpNotIn,

        LParen,
        RParen,
        LSquare,
        RSquare,
        LCurl,
        RCurl,

        Quote,
        SingleQuote,

        String,
        RawString,
        StringRef,
        StringExpr,
        CharLiteral,

        Error,
        Ignored,
    };

    enum class NumberType {
        NotANumber,
        UInteger,
        Integer,
        Real
    };

    static const std::string DEFAULT_LEXEME_ERROR;

    static std::string LexemeToStr[];

    static NumberType GetNumberType(LexemeType lexemeType);

    Lexeme();
    Lexeme(int col, int row, std::string text, LexemeType type, const std::string& valueRepresentation, bool isError = false);

    int GetColumn() const;
    int GetRow() const;
    const std::string& GetText() const;

    bool IsError() const;

    template<typename T>
    T GetValue() const {
        if (isError) {
            return std::any_cast<T>(std::any_cast<std::pair<std::any, std::string>>(myValue).first);
        }
        return std::any_cast<T>(myValue);
    }

    LexemeType GetType() const;

    std::string ToString() const;

    Lexeme CopyEmpty() const;
    Lexeme CopyEmptyOfType(LexemeType type) const;
private:
    std::string GetStringType() const;

    int myColumn;
    int myRow;
    std::string myText;
    std::any myValue;
    LexemeType myType;
    bool isError;
};

template<>
inline std::string Lexeme::GetValue() const {
    if (isError) {
        return std::any_cast<std::pair<std::any, std::string>>(myValue).second;
    }
    return std::any_cast<std::string>(myValue);
}

std::ostream& operator<<(std::ostream& out, const Lexeme& lexeme);
