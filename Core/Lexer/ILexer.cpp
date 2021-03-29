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

ILexer::ILexer() : myCurrentLexeme(0, 0, "", Lexeme::LexemeType::EndOfFile) {}

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

