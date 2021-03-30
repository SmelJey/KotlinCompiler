#include "ILexer.h"

#include <unordered_set>

#include "Lexeme.h"
#include "../InputBuffer.h"

const std::unordered_set<char> ILexer::BraceCharset { '(', ')', '[', ']', '{', '}', '\"', '\'' };
const std::unordered_set<char> ILexer::OperationsCharset { '+', '-', '*', '/', '=', '>', '<', '.',
        ',', ';', ':', '@', '$', '?', '!', '#', '|', '&', '%' };
const std::unordered_set<char> ILexer::SpacingCharset { ' ', '\t', '\r', '\n' };
const std::unordered_set<char> ILexer::NewlineCharset { '\n', '\r' };

const std::unordered_set<std::string> ILexer::OperationsSets[3] {
    { "+", "-", "/", "*", "%", "!", ",", ".", ":", ";", "=", "#", "@", "?", "<", ">" },
    { "++", "--", "&&", "||", "+=", "-=", "*=", "/=", "%=", "->", "=>", "..", "::", ";;", "<=", ">=", "!=", "==" },
    { "...", "!==", "===" }
} ;

const std::unordered_set<std::string> ILexer::KeywordSet {
    "return", "continue", "break", "this", "super", "file", "field", "property", "get", "set", "receiver",
    "param", "setparam", "delegate", "package", "import", "class", "interface", "fun", "object", "val", "var",
    "typealias", "constructor", "by", "companion", "init", "typeof", "where", "if", "else", "when", "try",
    "catch", "finally", "for", "do", "while", "throw", "as", "is", "in", "out", "dynamic", "public", "private",
    "protected", "internal", "enum", "sealed", "annotation", "data", "inner", "tailrec", "operator", "inline",
    "infix", "external", "suspend", "override", "abstract", "final", "open", "const", "lateinit", "vararg",
    "noinline", "crossinline", "reified", "expect", "actual", "true", "false", "null"
};

const std::unordered_set<char> ILexer::EscapeCharset { 't', 'b', 'r', 'n', '\'', '\"', '\\', '$' };

#define MAX_I(Type) std::numeric_limits<Type>::max()
const std::uint64_t ILexer::IntegersLimits[] {
    MAX_I(std::int8_t), MAX_I(std::int16_t), MAX_I(std::int32_t), MAX_I(std::int64_t),
    MAX_I(std::uint8_t), MAX_I(std::uint16_t), MAX_I(std::uint32_t), MAX_I(std::uint64_t)
};

ILexer::CharGroup ILexer::GetCharGroup(int character) {
    if (character == BUFFER_EOF) {
        return CharGroup::EndOfFile;
    }
    if (character >= 'a' && character <= 'z'
        || character >= 'A' && character <= 'Z' || character == '_') {
        return CharGroup::Alphabetic;
    }
    if (IsDecChar(character)) {
        return CharGroup::Digit;
    }
    if (character == '`') {
        return CharGroup::IdQuote;
    }
    if (character == '\"') {
        return CharGroup::Quote;
    }
    if (character == '\'') {
        return CharGroup::SingleQuote;
    }
    if (OperationsCharset.count(character)) {
        return CharGroup::Operation;
    }
    if (BraceCharset.count(character)) {
        return CharGroup::Brace;
    }
    if (SpacingCharset.count(character)) {
        return CharGroup::Spacing;
    }
    return CharGroup::Unknown;
}

ILexer::CharGroup ILexer::GetCharGroup(int character, int lookAhead1, int lookAhead2) {
    if (character == '\"') {
        if (lookAhead1 == '\"' && lookAhead2 == '\"') {
            return CharGroup::TripleQuote;
        }
    }

    if (character == '/') {
        if (lookAhead1 == '/') {
            return CharGroup::CommentStart;
        }
        if (lookAhead1 == '*') {
            return CharGroup::MultilineCommentStart;
        }
    }
    if (character == '#' && lookAhead1 == '!') {
        return CharGroup::CommentStart;
    }
    if (character == '.' && GetCharGroup(lookAhead1) == CharGroup::Digit) {
        return CharGroup::Digit;
    }
    if (character == '0') {
        if (lookAhead1 == 'x') {
            return CharGroup::HexPrefix;
        }
        if (lookAhead1 == 'b') {
            return CharGroup::BinPrefix;
        }
    }

    return GetCharGroup(character);
}

bool ILexer::IsDecChar(int character) {
    return character >= '0' && character <= '9';
}

bool ILexer::IsBinChar(int character) {
    return character == '0' || character == '1';
}

bool ILexer::IsHexChar(int character) {
    return IsDecChar(character) || (character >= 'a' && character <= 'f') || (character >= 'A' && character <= 'F');
}

LexType ILexer::GetLeastType(uint64_t value, LexType initialType) {
    if (initialType == LexType::ULong || initialType == LexType::Long) {
        if (initialType == LexType::Long && value > IntegersLimits[(int)LexType::Long - (int)LexType::Byte]) {
            return LexType::Error;
        }
        return initialType;
    }

    Lexeme::NumberType numType = Lexeme::GetNumberType(initialType);
    LexType lowestType;
    switch (numType) {
        case Lexeme::NumberType::Integer:
            initialType = LexType::Long;
            lowestType = LexType::Byte;
            if (IntegersLimits[(int)initialType - (int)LexType::Byte] < value) {
                return LexType::Error;
            }
            break;
        case Lexeme::NumberType::UInteger:
            initialType = LexType::ULong;
            lowestType = LexType::UByte;
            break;
        default:
            throw std::invalid_argument("Not an integer lexeme type");
    }

    while (initialType > lowestType) {
        if (IntegersLimits[(int)initialType - (int)LexType::Byte - 1] < value) {
            break;
        }
        initialType = static_cast<LexType>((int)initialType - 1);
    }

    return initialType;
}

char ILexer::EscapeToChar(char escapedChar) {
    switch (escapedChar) {
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 'b':
            return '\b';
        case 't':
            return '\t';
        default:
            return escapedChar;
    }
}

ILexer::ILexer() : myCurrentLexeme(0, 0, "", Lexeme::LexemeType::EndOfFile, Lexeme::DEFAULT_LEXEME_ERROR) {}

void ILexer::Init() {
    NextLexeme();
}

//TODO: change get / next lexeme (pre init somehow)
Lexeme ILexer::GetLexeme() const {
    return myCurrentLexeme;
}

Lexeme ILexer::NextLexeme() {
    Lexeme lastLexeme = myCurrentLexeme;
    myCurrentLexeme = GetLexemeFromInput();
    return lastLexeme;
}

