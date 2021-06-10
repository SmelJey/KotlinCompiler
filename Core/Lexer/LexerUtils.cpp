#include "LexerUtils.h"

#include <iomanip>
#include <sstream>

using LexType = Lexeme::LexemeType;

const std::unordered_set<char> LexerUtils::OperationsCharset { '+', '-', '*', '/', '=', '>', '<', '.',
        ',', ';', ':', '@', '$', '?', '!', '#', '|', '&', '%', '(', ')', '[', ']', '{', '}', '\"', '\'' };

const std::unordered_set<char> LexerUtils::SpacingCharset { ' ', '\t', '\r', '\n' };

const std::unordered_set<char> LexerUtils::NewlineCharset { '\n', '\r' };

const std::unordered_set<char> LexerUtils::EscapeCharset{ 't', 'b', 'r', 'n', '\'', '\"', '\\', '$' };

const std::unordered_set<std::string> LexerUtils::KeywordSet {
        "return", "continue", "break", "this", "super", "file", "field", "property", "get", "set", "receiver",
        "param", "setparam", "delegate", "package", "import", "class", "interface", "fun", "object", "val", "var",
        "typealias", "constructor", "by", "companion", "init", "typeof", "where", "if", "else", "when", "try",
        "catch", "finally", "for", "do", "while", "throw", "as", "is", "in", "out", "dynamic", "public", "private",
        "protected", "internal", "enum", "sealed", "annotation", "data", "inner", "tailrec", "operator", "inline",
        "infix", "external", "suspend", "override", "abstract", "final", "open", "const", "lateinit", "vararg",
        "noinline", "crossinline", "reified", "expect", "actual", "true", "false", "null"
};

const std::unordered_map<std::string, Lexeme::LexemeType> LexerUtils::OperationsSets[3]{
    {
        {"+", LexType::OpAdd}, {"-", LexType::OpSub}, {"/", LexType::OpDiv}, {"*", LexType::OpMult},
        {"%", LexType::OpMod}, {"!", LexType::OpExclMark}, {",", LexType::OpComma}, {".", LexType::OpDot},
        {":", LexType::OpColon}, {";", LexType::OpSemicolon}, {"=", LexType::OpAssign}, {"#", LexType::OpHash},
        {"@", LexType::OpAt}, {"?", LexType::OpQuestMark}, {"<", LexType::OpLess}, {">", LexType::OpGreater},
        {"(", LexType::LParen}, {")", LexType::RParen}, {"[", LexType::LSquare}, {"]", LexType::RSquare},
        {"{", LexType::LCurl}, {"}", LexType::RCurl}, {"\"", LexType::Quote}, {"\'", LexType::SingleQuote}
    }, {
        {"++", LexType::OpInc}, {"--", LexType::OpDec}, {"&&", LexType::OpAnd}, {"||", LexType::OpOr},
        {"+=", LexType::OpPlusAssign}, {"-=", LexType::OpMinusAssign}, {"*=", LexType::OpMultAssign},
        {"/=", LexType::OpDivAssign}, {"%=", LexType::OpModAssign}, {"->", LexType::OpArrow}, {"=>", LexType::OpDArrow},
        {"..", LexType::OpDDot}, {"::", LexType::OpDColon}, {";;", LexType::OpDColon},
        {"<=", LexType::OpLessOrEq}, {">=", LexType::OpGreaterOrEq}, {"!=", LexType::OpInequal}, {"==", LexType::OpEqual},
        {"in", LexType::OpIn}
    }, {
        {"...", LexType::OpTripleDot}, {"!==", LexType::OpStrictIneq}, {"===", LexType::OpStrictEq}, {"!in", LexType::OpNotIn}
    }
};

#define MAX_I(Type) std::numeric_limits<Type>::max()

const std::uint64_t LexerUtils::IntegersLimits[]{
    MAX_I(std::int8_t), MAX_I(std::int16_t), MAX_I(std::int32_t), MAX_I(std::int64_t),
    MAX_I(std::uint8_t), MAX_I(std::uint16_t), MAX_I(std::uint32_t), MAX_I(std::uint64_t)
};

bool LexerUtils::IsDigit(int character) {
    return character >= '0' && character <= '9';
}

bool LexerUtils::IsBinChar(int character) {
    return character == '0' || character == '1';
}

bool LexerUtils::IsHexChar(int character) {
    return IsDigit(character) || (character >= 'a' && character <= 'f') || (character >= 'A' && character <= 'F');
}

bool LexerUtils::IsAlphabetic(int character) {
    return character >= 'a' && character <= 'z' || character >= 'A' && character <= 'Z';
}

bool LexerUtils::TryGetReal(std::string& floatStr) {
    double val;
    try {
        val = std::stod(floatStr);
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }

    std::stringstream stm;
    stm << std::setprecision(std::numeric_limits<double>::digits10) << val;
    floatStr = stm.str();
    return true;
}

bool LexerUtils::TryGetInteger(std::string& intStr, LexType& initialType, int base) {
    std::uint64_t val;
    std::string prefix = (base == 16 ? "0x" : "");
    try {
        // std::stoull can't parse hex literals without prefix (and can't parse binary literals with prefix)
        val = std::stoull(prefix + intStr, nullptr, base);
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }

    intStr = std::to_string(val);
    return LexerUtils::GetLeastType(val, initialType);
}

bool LexerUtils::GetLeastType(uint64_t value, LexType& initialType) {
    if (initialType == LexType::Long) {
        return value <= IntegersLimits[(int)LexType::Long - (int)LexType::Byte];
    }
    if (initialType == LexType::ULong) {
        return true;
    }

    Lexeme::NumberType numType = Lexeme::GetNumberType(initialType);
    LexType lowestType;
    switch (numType) {
        case Lexeme::NumberType::Integer:
            initialType = LexType::Long;
            lowestType = LexType::Byte;
            if (IntegersLimits[(int)initialType - (int)LexType::Byte] < value) {
                return false;
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

    return true;
}

char LexerUtils::EscapeToChar(char escapedChar) {
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

bool LexerUtils::IsIntegerType(Lexeme::LexemeType type) {
    return type >= Lexeme::LexemeType::Byte && type <= Lexeme::LexemeType::ULong;
}

bool LexerUtils::IsRealType(Lexeme::LexemeType type) {
    return type == Lexeme::LexemeType::Float || type == Lexeme::LexemeType::Double;
}
