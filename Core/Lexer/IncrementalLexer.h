#pragma once

#include "ILexer.h"
#include "../InputBuffer.h"

#include <string>
#include <functional>
#include <queue>
#include <unordered_map>

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
        if (!myLexemeBuffer.empty()) {
            Lexeme res = myLexemeBuffer.front();
            myLexemeBuffer.pop_front();
            return res;
        }
        return ProcessNextLexeme();
    }

    Lexeme ProcessNextLexeme() {
        std::string curText;
        std::size_t startCol;
        std::size_t startRow;

        LexType lexemeType;
        std::string valueRepresentation;
        do {
            startCol = myCol;
            startRow = myRow;
            CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar(), myInputBuffer.LookAhead(1), myInputBuffer.LookAhead(2));
            std::tie(lexemeType, valueRepresentation) = PatternMap[(int)charGroup](this, curText);
        } while (lexemeType == Lexeme::LexemeType::Ignored);

        return Lexeme(startCol, startRow, curText, lexemeType, valueRepresentation);
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
                AddNextChars(2, out);
                out.clear();
                return std::make_pair(LexType::Ignored, "");
            }
            out.push_back(GetNextChar());
        }
        return std::make_pair(LexType::Error, "Unclosed comment");
    }

    std::pair<LexType, std::string> ProcessIdentifier(std::string& out) {
        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        while (charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Digit) {
            out.push_back(GetNextChar());
            charGroup = GetCharGroup(myInputBuffer.GetChar());
        }

        if (KeywordSet.count(out)) {
            return std::make_pair(LexType::Keyword, out);
        }
        return std::make_pair(LexType::Identifier, out);
    }

    std::pair<LexType, std::string> ProcessNumber(std::string& out) {
        if (myInputBuffer.GetChar() != '.') {
            if (!ProcessIntegerNumber(out, &ILexer::IsDecChar)) {
                return std::make_pair(LexType::Error, "Unparseable number");
            }
        }

        LexType numberType = LexType::Int;

        if (myInputBuffer.GetChar() == '.') {
            numberType = LexType::Double;
            out.push_back(GetNextChar());
            if (GetCharGroup(myInputBuffer.GetChar()) == CharGroup::Digit) {
                if (!ProcessIntegerNumber(out, &ILexer::IsDecChar)) {
                    return std::make_pair(LexType::Error, "Unparseable number");
                }
            }
        }

        if (myInputBuffer.GetChar() == 'e') {
            numberType = LexType::Double;
            out.push_back(GetNextChar());

            if (myInputBuffer.GetChar() == '+' || myInputBuffer.GetChar() == '-') {
                out.push_back(GetNextChar());
            }

            if (GetCharGroup(myInputBuffer.GetChar()) == CharGroup::Digit) {
                if (!ProcessIntegerNumber(out, &ILexer::IsDecChar)) {
                    return std::make_pair(LexType::Error, "Unparseable number");
                }
            } else {
                ConsumeLexeme(out);
                return std::make_pair(LexType::Error, "Unparseable number");
            }
        }

        if (myInputBuffer.GetChar() == 'f' || myInputBuffer.GetChar() == 'F') {
            out.push_back(GetNextChar());
            numberType = LexType::Float;
        }

        if ((myInputBuffer.GetChar() == 'u' || myInputBuffer.GetChar() == 'U') && numberType != LexType::Double && numberType != LexType::Float) {
            out.push_back(GetNextChar());
            numberType = LexType::UInt;
        }

        if (myInputBuffer.GetChar() == 'L' && numberType != LexType::Double && numberType != LexType::Float) {
            out.push_back(GetNextChar());
            numberType = (numberType == LexType::UInt ? Lexeme::LexemeType::ULong : Lexeme::LexemeType::Long);
        }

        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        if (charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Unknown || charGroup == CharGroup::Digit) {
            ConsumeLexeme(out);
            return std::make_pair(LexType::Error, "Unparseable number");
        }

        std::string number = out;
        number.erase(std::remove(number.begin(), number.end(), '_'), number.end());
        if (numberType == LexType::Double || numberType == LexType::Float) {
            try {
                std::stod(number);
            } catch (const std::invalid_argument&) {
                return std::make_pair(LexType::Error, "Unparseable number");
            } catch (const std::out_of_range&) {
                return std::make_pair(LexType::Error, "Number is out of range");
            }
            return std::make_pair(numberType, number);
        }

        return GetIntegerData(number, numberType);
    }

    std::pair<LexType, std::string> GetIntegerData(const std::string& number, LexType initialType, int base = 10) const {
        std::uint64_t val;
        std::string prefix = (base == 16 ? "0x" : "");
        try {
            // std::stoull can't parse hex literals without prefix (and can't parse binary literals with prefix)
            val = std::stoull(prefix + number, nullptr, base);
        } catch (const std::invalid_argument&) {
            return std::make_pair(LexType::Error, "Unparseable number");
        } catch (const std::out_of_range&) {
            return std::make_pair(LexType::Error, "Number is out of range");
        }

        initialType = GetLeastType(val, initialType);
        if (initialType == LexType::Error) {
            return std::make_pair(initialType, "Number is out of range");
        }
        return std::make_pair(initialType, prefix + number);
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
        return ProcessPrefixNumber(out, &ILexer::IsBinChar);
    }

    std::pair<LexType, std::string> ProcessHexNumber(std::string& out) {
        return ProcessPrefixNumber(out, &ILexer::IsHexChar);
    }

    std::pair<LexType, std::string> ProcessPrefixNumber(std::string& out, std::function<bool(int)> validator) {
        AddNextChars(2, out);
        if (!validator(myInputBuffer.GetChar())) {
            ConsumeLexeme(out);
            return std::make_pair(LexType::Error, "Unparseable number");
        }

        bool isGood = ProcessIntegerNumber(out, validator);
        LexType numberType = LexType::Int;

        if (myInputBuffer.GetChar() == 'u' || myInputBuffer.GetChar() == 'U') {
            numberType = LexType::UInt;
            out.push_back(GetNextChar());
        }

        if (myInputBuffer.GetChar() == 'L') {
            if (numberType == LexType::UInt) {
                numberType = LexType::ULong;
            } else {
                numberType = LexType::Long;
            }
            
            out.push_back(GetNextChar());
        }

        isGood &= ValidateNumberEnd(out);

        if (!isGood) {
            return std::make_pair(LexType::Error, "Unparseable number");
        }

        std::string valueRepresentation = out.substr(2);
        valueRepresentation.erase(std::remove(valueRepresentation.begin(), valueRepresentation.end(), '_'), valueRepresentation.end());
        return GetIntegerData(valueRepresentation, numberType, out[1] == 'b' ? 2 : 16);
    }

    std::pair<LexType, std::string> ProcessChar(std::string& out) {
        out.push_back(GetNextChar());
        bool isValidEscape = true;
        std::string escapedChar;

        if (myInputBuffer.GetChar() == '\\') {
            out.push_back(GetNextChar());
            if (!EscapeCharset.count(myInputBuffer.GetChar())) {
                isValidEscape = false;
            }
            escapedChar.push_back(EscapeToChar(myInputBuffer.GetChar()));
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

            return std::make_pair(LexType::CharLiteral, escapedChar.empty() ? out.substr(1, out.size() - 2) : escapedChar);
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
        bool needUnlock = false;
        if (!isInString) {
            isInString = true;
            needUnlock = true;
        }

        int startCol = myCol;
        int startRow = myRow;
        std::string valueRepresentation;

        out.push_back(GetNextChar());
        bool isValidEscape = true;
        while (myInputBuffer.GetChar() != BUFFER_EOF && !NewlineCharset.count(myInputBuffer.GetChar()) && myInputBuffer.GetChar() != '\"') {
            if (myInputBuffer.GetChar() == '$') {
                ProcessStringTemplate(out, startCol, startRow, Lexeme::LexemeType::String, valueRepresentation);
                continue;
            }

            if (myInputBuffer.GetChar() == '\\') {
                if (myInputBuffer.LookAhead(1) == BUFFER_EOF || !EscapeCharset.count(myInputBuffer.LookAhead(1))) {
                    isValidEscape = false;
                }
                out.push_back(GetNextChar());
                valueRepresentation.push_back(EscapeToChar(myInputBuffer.GetChar()));
                out.push_back(GetNextChar());
                continue;
            }

            out.push_back(myInputBuffer.GetChar());
            valueRepresentation.push_back(GetNextChar());
        }

        LexType lexType = Lexeme::LexemeType::Error;

        if (myInputBuffer.GetChar() == '\"') {
            out.push_back(myInputBuffer.GetChar());
            GetNextChar();
            if (!isValidEscape) {
                valueRepresentation = "Invalid escape char";
            } else {
                lexType = Lexeme::LexemeType::String;
            }
        } else {
            valueRepresentation = "Unclosed string";
        }

        return ReturnCurrentStringLexeme(startCol, startRow, needUnlock, out, lexType, valueRepresentation);
    }

    std::pair<LexType, std::string> ProcessRawString(std::string& out) {
        bool needUnlock = false;
        if (!isInString) {
            isInString = true;
            needUnlock = true;
        }

        std::string valueRepresentation;
        int startRow = myRow;
        int startCol = myCol;

        AddNextChars(3, out);

        while (!(myInputBuffer.GetChar() == BUFFER_EOF
                 || GetCharGroup(myInputBuffer.GetChar(), myInputBuffer.LookAhead(1), myInputBuffer.LookAhead(2)) == CharGroup::TripleQuote
                 && myInputBuffer.LookAhead(3) != '\"')) {
            if (myInputBuffer.GetChar() == '$') {
                ProcessStringTemplate(out, startCol, startRow, Lexeme::LexemeType::RawString, valueRepresentation);
                continue;
            }
            out.push_back(myInputBuffer.GetChar());
            valueRepresentation.push_back(GetNextChar());
        }

        LexType lexType = Lexeme::LexemeType::RawString;
        

        if (myInputBuffer.GetChar() == BUFFER_EOF) {
            lexType = Lexeme::LexemeType::Error;
            valueRepresentation = "Unclosed string";
        } else {
            AddNextChars(3, out);
        }

        return ReturnCurrentStringLexeme(startCol, startRow, needUnlock, out, lexType, valueRepresentation);
    }

    void ProcessStringTemplate(std::string& out, int& startCol, int& startRow, LexType stringType, std::string& valueRepresentation) {
        if (myInputBuffer.LookAhead(1) == '{') {
            myLexemeBuffer.emplace_back(startCol, startRow, out, stringType, valueRepresentation);
            out.clear();
            valueRepresentation.clear();
            AddNextChars(2, out);
            myLexemeBuffer.emplace_back(myCol - 2, myRow, out, LexType::StringExpr, out);
            ProcessStrExpression();

            out.clear();
            startCol = myCol;
            startRow = myRow;
        } else if (GetCharGroup(myInputBuffer.LookAhead(1)) == CharGroup::Alphabetic) {
            myLexemeBuffer.emplace_back(startCol, startRow, out, stringType, valueRepresentation);
            out.clear();
            valueRepresentation.clear();
            startCol = myCol;
            startRow = myRow;

            out.push_back(GetNextChar());
            auto [lexType, idValue] = ProcessIdentifier(out);
            myLexemeBuffer.emplace_back(startCol, startRow, out, LexType::StringRef, idValue.substr(1));
            out.clear();
            startCol = myCol;
            startRow = myRow;
        }
    }

    std::pair<LexType, std::string> ReturnCurrentStringLexeme(int col, int row, bool unlockString, std::string& out, LexType lexType, const std::string& valueRepresentation) {
        myLexemeBuffer.emplace_back(col, row, out, lexType, valueRepresentation);
        if (unlockString) {
            isInString = false;
        }
        if (isInString) {
            out.clear();
            return std::make_pair(LexType::Ignored, "");
        }

        Lexeme frontLexeme = myLexemeBuffer.front();
        myLexemeBuffer.pop_front();
        out = frontLexeme.GetText();
        return std::make_pair(frontLexeme.GetType(), frontLexeme.GetValue<std::string>());
    }

    void ProcessStrExpression() {
        Lexeme lexeme = ProcessNextLexeme();
        while (lexeme.GetType() != Lexeme::LexemeType::EndOfFile) {
            myLexemeBuffer.push_back(lexeme);
            if (lexeme.GetText() == "{") {
                ProcessStrExpression();
            } else if (lexeme.GetText() == "}") {
                return;
            }
            lexeme = ProcessNextLexeme();
        }
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
                return std::make_pair(LexType::Operation, out);
            }

            operation.pop_back();
        }

        out.push_back(GetNextChar());
        return std::make_pair(LexType::Operation, "Unknown operator");
    }

    std::pair<LexType, std::string> ProcessBrace(std::string& out) {
        out.push_back(GetNextChar());
        return std::make_pair(LexType::Brace, out);
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
        return std::make_pair(LexType::Identifier, out.substr(1, out.size() - 2));
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

    void AddNextChars(int cnt, std::string& out) {
        for (int i = 0; i < cnt; i++) {
            out.push_back(GetNextChar());
        }
    }
         
    std::size_t myCol = 0;
    std::size_t myRow = 0;
    InputBuffer<InputType> myInputBuffer;
    std::deque<Lexeme> myLexemeBuffer;

    bool isInString = false;
};
