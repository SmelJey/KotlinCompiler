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
    std::function<std::pair<bool, Lexeme::LexemeType>(IncrementalLexer*, std::string&)> PatternMap[11] {
        &IncrementalLexer::ProcessNumber,
        &IncrementalLexer::ProcessIdentifier,
        &IncrementalLexer::ProcessOperation,
        &IncrementalLexer::ProcessBrace,
        &IncrementalLexer::ProcessString,
        &IncrementalLexer::ProcessRawString,
        &IncrementalLexer::ProcessComment,
        &IncrementalLexer::ProcessMultilineComment,
        &IncrementalLexer::ProcessWhitespaces,
        &IncrementalLexer::ProcessUnknown,
        &IncrementalLexer::ProcessEof,
    };

    Lexeme GetLexemeFromInput() override {
        std::string curText;
        std::size_t startCol;
        std::size_t startRow;

        LexType lexemeType;
        bool isGood;
        while (curText.empty()) {
            startCol = myCol;
            startRow = myRow;
            CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar(), myInputBuffer.LookAhead(1), myInputBuffer.LookAhead(2));
            std::tie(isGood, lexemeType) = PatternMap[(int)charGroup](this, curText);
            if (!isGood || lexemeType == LexType::EndOfFile) {
                return Lexeme(startCol, startRow, curText, lexemeType, isGood);
            }
        }

        return Lexeme(startCol, startRow, curText, lexemeType);
    }

    std::pair<bool, LexType> ProcessWhitespaces(std::string& out) {
        while (myInputBuffer.GetChar() != BUFFER_EOF && SpacingCharset.count(myInputBuffer.GetChar())) {
            GetNextChar();
        }
        return std::make_pair(true, LexType::Ignored);
    }

    std::pair<bool, LexType> ProcessComment(std::string& out) {
        while (myInputBuffer.GetChar() != BUFFER_EOF && !NewlineCharset.count(myInputBuffer.GetChar())) {
            GetNextChar();
        }
        return std::make_pair(true, LexType::Ignored);
    }

    std::pair<bool, LexType> ProcessMultilineComment(std::string& out) {
        while (myInputBuffer.GetChar() != BUFFER_EOF) {
            if (myInputBuffer.GetChar() == '*' && myInputBuffer.LookAhead(1) == '/') {
                GetNextChar();
                GetNextChar();
                return std::make_pair(true, LexType::Ignored);
            }
            GetNextChar();
        }
        return std::make_pair(false, LexType::Ignored);;
    }

    std::pair<bool, LexType> ProcessIdentifier(std::string& out) {
        out.push_back(GetNextChar());
        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        while (charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Digit) {
            out.push_back(GetNextChar());
            charGroup = GetCharGroup(myInputBuffer.GetChar());
        }

        if (KeywordSet.count(out)) {
            return std::make_pair(true, LexType::Keyword);
        }
        return std::make_pair(true, LexType::Identifier);
    }

    std::pair<bool, LexType> ProcessNumber(std::string& out) {
        if (myInputBuffer.GetChar() != '.') {
            bool isGood = ProcessIntegerNumber(out);
            if (!isGood) {
                return std::make_pair(isGood, LexType::Number);
            }
        }

        bool isFloat = false;

        if (myInputBuffer.GetChar() == '.') {
            isFloat = true;
            out.push_back(GetNextChar());
            if (GetCharGroup(myInputBuffer.GetChar()) == CharGroup::Digit) {
                bool isGood = ProcessIntegerNumber(out);
                if (!isGood) {
                    return std::make_pair(isGood, LexType::Number);
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
                bool isGood = ProcessIntegerNumber(out);
                if (!isGood) {
                    return std::make_pair(isGood, LexType::Number);
                }
            } else {
                ConsumeLexeme(out);
                return std::make_pair(false, LexType::Number);
            }
        }

        if (myInputBuffer.GetChar() == 'f' || myInputBuffer.GetChar() == 'F' || myInputBuffer.GetChar() == 'L' && !isFloat) {
            out.push_back(GetNextChar());
        }

        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        if (charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Unknown) {
            ConsumeLexeme(out);
            return std::make_pair(false, LexType::Number);
        }

        return std::make_pair(true, LexType::Number);
    }

    bool ProcessIntegerNumber(std::string& out) {
        out.push_back(GetNextChar());
        int internalLen = ProcessNumberInternals(out);

        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());

        if (charGroup != CharGroup::Digit) {
            if (internalLen == 0) {
                return true;
            }

            ConsumeLexeme(out);
            return false;
        }

        out.push_back(GetNextChar());
        return true;
    }

    int ProcessNumberInternals(std::string& out) {
        int len = 0;
        while ((GetCharGroup(myInputBuffer.GetChar()) == CharGroup::Digit || myInputBuffer.GetChar() == '_')
               && (GetCharGroup(myInputBuffer.LookAhead(1)) == CharGroup::Digit || myInputBuffer.LookAhead(1) == '_')) {
            len++;
            out.push_back(GetNextChar());
        }
        return len;
    }

    std::pair<bool, LexType> ProcessString(std::string& out) {
        out.push_back(myInputBuffer.GetChar());
        GetNextChar();
        while (myInputBuffer.GetChar() != BUFFER_EOF && !NewlineCharset.count(myInputBuffer.GetChar()) && myInputBuffer.GetChar() != '\"') {
            if (myInputBuffer.GetChar() == '\\' && myInputBuffer.LookAhead(1) == '\"') {
                out.push_back(myInputBuffer.GetChar());
                GetNextChar();
            }
            out.push_back(myInputBuffer.GetChar());
            GetNextChar();
        }

        bool isComplete = false;
        if (myInputBuffer.GetChar() == '\"') {
            isComplete = true;
            out.push_back(myInputBuffer.GetChar());
            GetNextChar();
        }

        return std::make_pair(isComplete, LexType::String);
    }

    std::pair<bool, LexType> ProcessOperation(std::string& out) {
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
                return std::make_pair(true, LexType::Operation);
            }

            operation.pop_back();
        }

        return std::make_pair(false, LexType::Operation);
    }

    std::pair<bool, LexType> ProcessBrace(std::string& out) {
        out.push_back(GetNextChar());
        return std::make_pair(true, LexType::Brace);
    }

    std::pair<bool, LexType> ProcessRawString(std::string& out) {
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());

        while (!(myInputBuffer.GetChar() == BUFFER_EOF
                 || GetCharGroup(myInputBuffer.GetChar(), myInputBuffer.LookAhead(1), myInputBuffer.LookAhead(2)) == CharGroup::TripleQuote
                 && myInputBuffer.LookAhead(3) != '\"')) {
            out.push_back(GetNextChar());
        }
        if (myInputBuffer.GetChar() == BUFFER_EOF) {
            return std::make_pair(false, LexType::RawString);
        }

        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        return std::make_pair(true, LexType::RawString);
    }

    std::pair<bool, LexType> ProcessUnknown(std::string& out) {
        ConsumeLexeme(out);
        return std::make_pair(false, LexType::Error);
    }

    std::pair<bool, LexType> ProcessEof(std::string& out) {
        return std::make_pair(true, LexType::EndOfFile);
    }

    void ConsumeLexeme(std::string& out) {
        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        while (charGroup == CharGroup::Digit || charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Unknown) {
            out.push_back(myInputBuffer.GetChar());
            charGroup = GetCharGroup(GetNextChar());
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
};
