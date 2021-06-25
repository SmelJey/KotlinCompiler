#pragma once

#include "Lexeme.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

using LexemeType = Lexeme::LexemeType;

class LexerUtils {
public:
    static const std::unordered_set<char> OperationsCharset;
    static const std::unordered_set<char> SpacingCharset;
    static const std::unordered_set<char> NewlineCharset;

    static const std::unordered_set<char> EscapeCharset;

    static const std::unordered_set<std::string> KeywordSet;
    static const std::unordered_map<std::string, LexemeType> OperationsSets[3];

    static const std::uint64_t IntegersLimits[];

    static bool IsDigit(int character);

    static bool IsBinChar(int character);

    static bool IsHexChar(int character);

    static bool IsAlphabetic(int character);

    static bool TryGetReal(std::string& floatStr);
    static bool TryGetInteger(std::string& intStr, LexemeType& initialType, int base = 10);

    static bool GetLeastType(uint64_t value, LexemeType& initialType);

    static char EscapeToChar(char escapedChar);

    static bool IsIntegerType(LexemeType type);

    static bool IsRealType(LexemeType type);

    static bool IsArithmAssignOperation(LexemeType type);

    static bool IsEqualityOperation(LexemeType type);
    static bool IsBoolOperation(LexemeType type);
    static bool IsArithmeticOperation(LexemeType type);
};
