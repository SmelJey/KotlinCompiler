#include "LexerUtils.h"

#include <iomanip>
#include <sstream>

using LexemeType = LexemeType;

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

const std::unordered_map<std::string, LexemeType> LexerUtils::OperationsSets[3]{
    {
        {"+", LexemeType::OpAdd}, {"-", LexemeType::OpSub}, {"/", LexemeType::OpDiv}, {"*", LexemeType::OpMult},
        {"%", LexemeType::OpMod}, {"!", LexemeType::OpExclMark}, {",", LexemeType::OpComma}, {".", LexemeType::OpDot},
        {":", LexemeType::OpColon}, {";", LexemeType::OpSemicolon}, {"=", LexemeType::OpAssign}, {"#", LexemeType::OpHash},
        {"@", LexemeType::OpAt}, {"?", LexemeType::OpQuestMark}, {"<", LexemeType::OpLess}, {">", LexemeType::OpGreater},
        {"(", LexemeType::LParen}, {")", LexemeType::RParen}, {"[", LexemeType::LSquare}, {"]", LexemeType::RSquare},
        {"{", LexemeType::LCurl}, {"}", LexemeType::RCurl}, {"\"", LexemeType::Quote}, {"\'", LexemeType::SingleQuote}
    }, {
        {"++", LexemeType::OpInc}, {"--", LexemeType::OpDec}, {"&&", LexemeType::OpAnd}, {"||", LexemeType::OpOr},
        {"+=", LexemeType::OpPlusAssign}, {"-=", LexemeType::OpMinusAssign}, {"*=", LexemeType::OpMultAssign},
        {"/=", LexemeType::OpDivAssign}, {"%=", LexemeType::OpModAssign}, {"->", LexemeType::OpArrow}, {"=>", LexemeType::OpDArrow},
        {"..", LexemeType::OpDDot}, {"::", LexemeType::OpDColon}, {";;", LexemeType::OpDColon},
        {"<=", LexemeType::OpLessOrEq}, {">=", LexemeType::OpGreaterOrEq}, {"!=", LexemeType::OpInequal}, {"==", LexemeType::OpEqual},
        {"in", LexemeType::OpIn}
    }, {
        {"...", LexemeType::OpTripleDot}, {"!==", LexemeType::OpStrictIneq}, {"===", LexemeType::OpStrictEq}, {"!in", LexemeType::OpNotIn}
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

bool LexerUtils::TryGetInteger(std::string& intStr, LexemeType& initialType, int base) {
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

bool LexerUtils::GetLeastType(uint64_t value, LexemeType& initialType) {
    if (initialType == LexemeType::Long) {
        return value <= IntegersLimits[(int)LexemeType::Long - (int)LexemeType::Byte];
    }
    if (initialType == LexemeType::ULong) {
        return true;
    }

    Lexeme::NumberType numType = Lexeme::GetNumberType(initialType);
    LexemeType lowestType;
    switch (numType) {
        case Lexeme::NumberType::Integer:
            initialType = LexemeType::Long;
            lowestType = LexemeType::Byte;
            if (IntegersLimits[(int)initialType - (int)LexemeType::Byte] < value) {
                return false;
            }
            break;
        case Lexeme::NumberType::UInteger:
            initialType = LexemeType::ULong;
            lowestType = LexemeType::UByte;
            break;
        default:
            throw std::invalid_argument("Not an integer lexeme type");
    }

    while (initialType > lowestType) {
        if (IntegersLimits[(int)initialType - (int)LexemeType::Byte - 1] < value) {
            break;
        }
        initialType = static_cast<LexemeType>((int)initialType - 1);
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

bool LexerUtils::IsIntegerType(LexemeType type) {
    return type >= LexemeType::Byte && type <= LexemeType::ULong;
}

bool LexerUtils::IsRealType(LexemeType type) {
    return type == LexemeType::Float || type == LexemeType::Double;
}

bool LexerUtils::IsEqualityOperation(LexemeType type) {
    return type == LexemeType::OpEqual || type == LexemeType::OpInequal || type == LexemeType::OpStrictEq || type == LexemeType::OpStrictIneq;
}

bool LexerUtils::IsBoolOperation(LexemeType type) {
    return type >= LexemeType::OpLess && type <= LexemeType::OpStrictEq || type == LexemeType::OpAnd || type == LexemeType::OpOr;
}

bool LexerUtils::IsArithmeticOperation(LexemeType type) {
    return type >= LexemeType::OpAdd && type <= LexemeType::OpMod;
}
