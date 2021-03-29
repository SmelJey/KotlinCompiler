#pragma once

#include "ILexer.h"
#include "../InputBuffer.h"

#include <string>
#include <functional>
#include <unordered_map>

using LexType = Lexeme::LexemeType;

template<typename InputType>
class IncrementalLexer : public ILexer {
public:
    IncrementalLexer(InputType& input) : myInputBuffer(input.rdbuf()) {}

    IncrementalLexer(const std::string& filepath) : myInputBuffer(filepath) {}
private:
    std::function<std::pair<Lexeme::LexemeType, std::string>(IncrementalLexer*, std::string&)> PatternMap[15] {
        &IncrementalLexer::ProcessNumber,
        &IncrementalLexer::ProcessIdentifier,
        &IncrementalLexer::ProcessOperation,
        &IncrementalLexer::ProcessBrace,
        &IncrementalLexer::ProcessEscapedIdentifier,
        &IncrementalLexer::ProcessChar,
        &IncrementalLexer::ProcessString,
        &IncrementalLexer::ProcessRawString,
        &IncrementalLexer::ProcessComment,
        &IncrementalLexer::ProcessMultilineComment,
        &IncrementalLexer::ProcessBinNumber,
        &IncrementalLexer::ProcessHexNumber,
        &IncrementalLexer::ProcessWhitespaces,
        &IncrementalLexer::ProcessUnknown,
        &IncrementalLexer::ProcessEof,
    };

    Lexeme GetLexemeFromInput() override {
        std::string curText;
        std::size_t startCol;
        std::size_t startRow;

        LexType lexemeType;
        std::string error;
        while (curText.empty()) {
            startCol = myCol;
            startRow = myRow;
            CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar(), myInputBuffer.LookAhead(1), myInputBuffer.LookAhead(2));
            std::tie(lexemeType, error) = PatternMap[(int)charGroup](this, curText);
            if (!error.empty() || lexemeType == LexType::EndOfFile) {
                return Lexeme(startCol, startRow, curText, lexemeType, error);
            }
        }

        return Lexeme(startCol, startRow, curText, lexemeType);
    }

    std::pair<LexType, std::string> ProcessWhitespaces(std::string& out) {
        while (myInputBuffer.GetChar() != BUFFER_EOF && SpacingCharset.count(myInputBuffer.GetChar())) {
            GetNextChar();
        }
        return std::make_pair(LexType::Ignored, "");
    }

    std::pair<LexType, std::string> ProcessComment(std::string& out) {
        while (myInputBuffer.GetChar() != BUFFER_EOF && !NewlineCharset.count(myInputBuffer.GetChar())) {
            GetNextChar();
        }
        return std::make_pair(LexType::Ignored, "");
    }

    std::pair<LexType, std::string> ProcessMultilineComment(std::string& out) {
        while (myInputBuffer.GetChar() != BUFFER_EOF) {
            if (myInputBuffer.GetChar() == '*' && myInputBuffer.LookAhead(1) == '/') {
                GetNextChar();
                GetNextChar();
                out.clear();
                return std::make_pair(LexType::Ignored, "");
            }
            out.push_back(GetNextChar());
        }
        return std::make_pair(LexType::Error, "Unclosed comment");
    }

    std::pair<LexType, std::string> ProcessIdentifier(std::string& out) {
        out.push_back(GetNextChar());
        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        while (charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Digit) {
            out.push_back(GetNextChar());
            charGroup = GetCharGroup(myInputBuffer.GetChar());
        }

        if (KeywordSet.count(out)) {
            return std::make_pair(LexType::Keyword, "");
        }
        return std::make_pair(LexType::Identifier, "");
    }

    std::pair<LexType, std::string> ProcessNumber(std::string& out) {
        if (myInputBuffer.GetChar() != '.') {
            if (!ProcessIntegerNumber(out, &ILexer::IsDecChar)) {
                return std::make_pair(LexType::Error, "Unparseable number");
            }
        }

        bool isFloat = false;

        if (myInputBuffer.GetChar() == '.') {
            isFloat = true;
            out.push_back(GetNextChar());
            if (GetCharGroup(myInputBuffer.GetChar()) == CharGroup::Digit) {
                if (!ProcessIntegerNumber(out, &ILexer::IsDecChar)) {
                    return std::make_pair(LexType::Error, "Unparseable number");
                }
            }
        }

        if (myInputBuffer.GetChar() == 'e') {
            isFloat = true;
            out.push_back(GetNextChar());
            if (myInputBuffer.GetChar() == '+' || myInputBuffer.GetChar() == '-') {
                out.push_back(GetNextChar());
            }
            CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
            if (charGroup == CharGroup::Digit) {
                if (!ProcessIntegerNumber(out, &ILexer::IsDecChar)) {
                    return std::make_pair(LexType::Error, "Unparseable number");
                }
            } else {
                ConsumeLexeme(out);
                return std::make_pair(LexType::Error, "Unparseable number");
            }
        }

        if (myInputBuffer.GetChar() == 'f' || myInputBuffer.GetChar() == 'F' || myInputBuffer.GetChar() == 'L' && !isFloat) {
            out.push_back(GetNextChar());
        }

        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        if (charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Unknown || charGroup == CharGroup::Digit) {
            ConsumeLexeme(out);
            return std::make_pair(LexType::Error, "Unparseable number");
        }

        return std::make_pair(LexType::Number, "");
    }

    bool ProcessIntegerNumber(std::string& out, std::function<bool(int)> validator) {
        out.push_back(GetNextChar());
        int internalLen = ProcessNumberInternals(out, validator);

        if (!validator(myInputBuffer.GetChar())) {
            if (internalLen == 0) {
                return true;
            }

            ConsumeLexeme(out);
            return false;
        }

        out.push_back(GetNextChar());
        return true;
    }

    int ProcessNumberInternals(std::string& out, std::function<bool(int)> validator) {
        int len = 0;
        while ((validator(myInputBuffer.GetChar()) || myInputBuffer.GetChar() == '_')
               && (validator(myInputBuffer.LookAhead(1)) || myInputBuffer.LookAhead(1) == '_')) {
            len++;
            out.push_back(GetNextChar());
        }
        return len;
    }

    bool ValidateNumberEnd(std::string& out) {
        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        if (charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Unknown || charGroup == CharGroup::Digit) {
            ConsumeLexeme(out);
            return false;
        }
        return true;
    }

    std::pair<LexType, std::string> ProcessBinNumber(std::string& out) {
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        if (!IsBinChar(myInputBuffer.GetChar())) {
            ConsumeLexeme(out);
            return std::make_pair(LexType::Error, "Unparseable number");
        }

        bool isGood = ProcessIntegerNumber(out, &ILexer::IsBinChar);
        if (myInputBuffer.GetChar() == 'L') {
            out.push_back(GetNextChar());
        }

        isGood &= ValidateNumberEnd(out);

        if (!isGood) {
            return std::make_pair(LexType::Error, "Unparseable number");
        }
        return std::make_pair(LexType::Number, "");
    }

    std::pair<LexType, std::string> ProcessHexNumber(std::string& out) {
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        if (!IsHexChar(myInputBuffer.GetChar())) {
            ConsumeLexeme(out);
            return std::make_pair(LexType::Error, "Unparseable number");
        }

        bool isGood = ProcessIntegerNumber(out, &ILexer::IsHexChar);
        if (myInputBuffer.GetChar() == 'L') {
            out.push_back(GetNextChar());
        }

        isGood &= ValidateNumberEnd(out);

        if (!isGood) {
            return std::make_pair(LexType::Error, "Unparseable number");
        }
        return std::make_pair(LexType::Number, "");
    }

    std::pair<LexType, std::string> ProcessChar(std::string& out) {
        out.push_back(GetNextChar());
        bool isValidEscape = true;
        if (myInputBuffer.GetChar() == '\\') {
            out.push_back(GetNextChar());
            if (!EscapeCharset.count(myInputBuffer.GetChar())) {
                isValidEscape = false;
            }
        }
        if (myInputBuffer.GetChar() == BUFFER_EOF || NewlineCharset.count(myInputBuffer.GetChar())) {
            return std::make_pair(LexType::Error, "Unclosed char literal");
        }

        out.push_back(GetNextChar());
        if (myInputBuffer.GetChar() == '\'') {
            out.push_back(GetNextChar());
            if (!isValidEscape) {
                return std::make_pair(LexType::Error, "Invalid escape char");
            }

            return std::make_pair(LexType::CharLiteral, "");
        }

        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        while (charGroup != CharGroup::EndOfFile && charGroup != CharGroup::Spacing && charGroup != CharGroup::SingleQuote) {
            out.push_back(GetNextChar());
            charGroup = GetCharGroup(myInputBuffer.GetChar());
        }
        if (charGroup == CharGroup::SingleQuote) {
            out.push_back(GetNextChar());
            return std::make_pair(LexType::Error, "Invalid char literal");
        }

        return std::make_pair(LexType::Error, "Unclosed char literal");
    }

    std::pair<LexType, std::string> ProcessString(std::string& out) {
        out.push_back(myInputBuffer.GetChar());
        GetNextChar();
        bool isValidEscape = true;
        while (myInputBuffer.GetChar() != BUFFER_EOF && !NewlineCharset.count(myInputBuffer.GetChar()) && myInputBuffer.GetChar() != '\"') {
            if (myInputBuffer.GetChar() == '\\') {
                if (myInputBuffer.LookAhead(1) == BUFFER_EOF || !EscapeCharset.count(myInputBuffer.LookAhead(1))) {
                    isValidEscape = false;
                }
                out.push_back(myInputBuffer.GetChar());
                GetNextChar();
            }
            out.push_back(myInputBuffer.GetChar());
            GetNextChar();
        }

        if (myInputBuffer.GetChar() == '\"') {
            out.push_back(myInputBuffer.GetChar());
            GetNextChar();
            if (!isValidEscape) {
                return std::make_pair(LexType::Error, "Invalid escape char");
            }
            return std::make_pair(LexType::String, "");
        }

        return std::make_pair(LexType::Error, "Unclosed string");
    }

    std::pair<LexType, std::string> ProcessRawString(std::string& out) {
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());

        while (!(myInputBuffer.GetChar() == BUFFER_EOF
                 || GetCharGroup(myInputBuffer.GetChar(), myInputBuffer.LookAhead(1), myInputBuffer.LookAhead(2)) == CharGroup::TripleQuote
                 && myInputBuffer.LookAhead(3) != '\"')) {
            out.push_back(GetNextChar());
        }
        if (myInputBuffer.GetChar() == BUFFER_EOF) {
            return std::make_pair(LexType::RawString, "Unclosed string");
        }

        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        return std::make_pair(LexType::RawString, "");
    }

    std::pair<LexType, std::string> ProcessOperation(std::string& out) {
        std::string operation;
        for (int i = 0; i < 3; i++) {
            int curChar = myInputBuffer.LookAhead(i);
            if (curChar == BUFFER_EOF) {
                break;
            }
            operation.push_back(myInputBuffer.LookAhead(i));
        }
        while (!operation.empty()) {
            if (OperationsSets[operation.size() - 1].count(operation)) {
                for (auto c : operation) {
                    out.push_back(GetNextChar());
                }
                return std::make_pair(LexType::Operation, "");
            }

            operation.pop_back();
        }

        out.push_back(GetNextChar());
        return std::make_pair(LexType::Operation, "Unknown operator");
    }

    std::pair<LexType, std::string> ProcessBrace(std::string& out) {
        out.push_back(GetNextChar());
        return std::make_pair(LexType::Brace, "");
    }

    std::pair<LexType, std::string> ProcessEscapedIdentifier(std::string& out) {
        out.push_back(GetNextChar());
        while (myInputBuffer.GetChar() != BUFFER_EOF && !NewlineCharset.count(myInputBuffer.GetChar()) && myInputBuffer.GetChar() != '`') {
            out.push_back(GetNextChar());
        }

        if (myInputBuffer.GetChar() != '`') {
            return std::make_pair(LexType::Error, "Unclosed escaped identifier");
        }
        out.push_back(GetNextChar());
        return std::make_pair(LexType::Identifier, "");
    }

    std::pair<LexType, std::string> ProcessUnknown(std::string& out) {
        ConsumeLexeme(out);
        return std::make_pair(LexType::Error, "Unknown lexeme");
    }

    std::pair<LexType, std::string> ProcessEof(std::string& out) {
        return std::make_pair(LexType::EndOfFile, "");
    }

    void ConsumeLexeme(std::string& out) {
        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        while (charGroup == CharGroup::Digit || charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Unknown) {
            out.push_back(GetNextChar());
            charGroup = GetCharGroup(myInputBuffer.GetChar());
        }
    }

    int GetNextChar() {
        int character = myInputBuffer.NextChar();
        if (character != BUFFER_EOF) {
            myCol++;
            if (NewlineCharset.count(character)) {
                myRow++;
                myCol = 0;
            }
        }

        return character;
    }
         
    std::size_t myCol = 0;
    std::size_t myRow = 0;
    InputBuffer<InputType> myInputBuffer;
    std::size_t myStringNesting = 0;
};
