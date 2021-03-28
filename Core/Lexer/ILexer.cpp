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

ILexer::CharGroup ILexer::GetCharGroup(int character) {
    if (character == BUFFER_EOF) {
        return CharGroup::EndOfFile;
    }
    if (character >= 'a' && character <= 'z'
        || character >= 'A' && character <= 'Z' || character == '_') {
        return CharGroup::Alphabetic;
    }
    if (character >= '0' && character <= '9') {
        return CharGroup::Digit;
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

ILexer::ILexer() : myCurrentLexeme(0, 0, "", Lexeme::LexemeType::EndOfFile) {}

Lexeme ILexer::GetLexeme() {
    if (myCurrentLexeme.GetType() == Lexeme::LexemeType::EndOfFile) {
        NextLexeme();
    }

    return myCurrentLexeme;
}

Lexeme ILexer::NextLexeme() {
    Lexeme lastLexeme = myCurrentLexeme;
    myCurrentLexeme = GetLexemeFromInput();
    return lastLexeme;
}

