#pragma once

#include "ILexer.h"
#include "../InputBuffer.h"

#include <string>

template<typename InputType>
class IncrementalLexer : public ILexer {
public:
    IncrementalLexer(InputType& input) : myInputBuffer(input.rdbuf()) {
        
    }

    IncrementalLexer(const std::string& filepath) : myInputBuffer(filepath) {
        
    }
private:
    Lexeme GetLexemeFromInput() override {
        ProcessWhitespaces();
        CommentStatus commentStatus = IsComment();
        while (commentStatus != CommentStatus::NotComment) {
            ProcessComments(commentStatus);
            ProcessWhitespaces();
            commentStatus = IsComment();
        }

        std::string curText;
        std::size_t startCol = myCol;
        std::size_t startRow = myRow;

        if (myInputBuffer.GetChar() == '\"') {
            if (myInputBuffer.LookAhead(1) == '\"' && myInputBuffer.LookAhead(2) == '\"') {
                bool isComplete = ProcessRawString(curText);
                // TODO: make error 
                return Lexeme(startCol, startRow, curText, Lexeme::LexemeType::RawString, isComplete);
            }
            // TODO: make error
            bool isComplete = ProcessString(curText);
            return Lexeme(startCol, startRow, curText, Lexeme::LexemeType::String, isComplete);
        }

        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        switch (charGroup) {
            case CharGroup::Operation: {
                if (myInputBuffer.GetChar() == '.' && GetCharGroup(myInputBuffer.LookAhead(1)) == CharGroup::Digit) {
                    bool isComplete = ProcessNumber(curText);
                    return Lexeme(startCol, startRow, curText, Lexeme::LexemeType::Number, isComplete);
                }

                bool isComplete = ProcessOperation(curText);

                return Lexeme(startCol, startRow, curText, Lexeme::LexemeType::Operation, isComplete);
            }
            case CharGroup::Digit: {
                // TODO: make error;
                bool isComplete = ProcessNumber(curText);
                return Lexeme(startCol, startRow, curText, Lexeme::LexemeType::Number, isComplete);
            }
            case CharGroup::Alphabetic: {
                ProcessIdentifier(curText);
                return Lexeme(startCol, startRow, curText, Lexeme::LexemeType::Identifier);
            }
            case CharGroup::Brace: {
                curText.push_back(GetNextChar());
                return Lexeme(startCol, startRow, curText, Lexeme::LexemeType::Separator);
            }
            case CharGroup::EndOfFile: {
                return Lexeme(startCol, startRow, "", Lexeme::LexemeType::EndOfFile);
            }
        }

        while (GetCharGroup(myInputBuffer.GetChar()) == CharGroup::Unknown) {
            curText.push_back(GetNextChar());
        }
        return Lexeme(startCol, startRow, curText, Lexeme::LexemeType::Error, false);
    }

    enum class CommentStatus {
        NotComment,
        ShebangLine,
        Comment,
        MultilineComment
    };

    CommentStatus IsComment() {
        if (myInputBuffer.GetChar() == '/') {
            if (myInputBuffer.LookAhead(1) == '/') {
                return CommentStatus::Comment;
            }
            if (myInputBuffer.LookAhead(1) == '*') {
                return CommentStatus::MultilineComment;
            }
        } else if (myInputBuffer.GetChar() == '#' && myInputBuffer.LookAhead(1) == '!') {
            return CommentStatus::ShebangLine;
        }
        return CommentStatus::NotComment;
    }

    void ProcessWhitespaces() {
        while (myInputBuffer.GetChar() != BUFFER_EOF && SpacingCharset.count(myInputBuffer.GetChar())) {
            GetNextChar();
        }
    }

    void ProcessComments(CommentStatus commentStatus) {
        GetNextChar();
        GetNextChar();

        switch (commentStatus) {
            case CommentStatus::ShebangLine:
            case CommentStatus::Comment:
                while (myInputBuffer.GetChar() != BUFFER_EOF && !NewlineCharset.count(GetNextChar())) {}
                return;
            case CommentStatus::MultilineComment:
                while (myInputBuffer.GetChar() != BUFFER_EOF) {
                    if (myInputBuffer.GetChar() == '*' && myInputBuffer.LookAhead(1) == '/') {
                        GetNextChar();
                        GetNextChar();
                        return;
                    }
                    GetNextChar();
                }
        }
    }

    void ProcessIdentifier(std::string& out) {
        out.push_back(GetNextChar());
        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        while (charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Digit) {
            out.push_back(GetNextChar());
            charGroup = GetCharGroup(myInputBuffer.GetChar());
        }
    }

    bool ProcessNumber(std::string& out) {
        if (myInputBuffer.GetChar() != '.') {
            bool isGood = ProcessIntegerNumber(out);
            if (!isGood) {
                return isGood;
            }
        }

        if (myInputBuffer.GetChar() == '.') {
            out.push_back(GetNextChar());
            if (GetCharGroup(myInputBuffer.GetChar()) == CharGroup::Digit) {
                bool isGood = ProcessIntegerNumber(out);
                if (!isGood) {
                    return isGood;
                }
            }
        }

        if (myInputBuffer.GetChar() == 'e') {
            out.push_back(GetNextChar());
            if (myInputBuffer.GetChar() == '+' || myInputBuffer.GetChar() == '-') {
                out.push_back(GetNextChar());
            }
            if (GetCharGroup(myInputBuffer.GetChar()) == CharGroup::Digit) {
                bool isGood = ProcessIntegerNumber(out);
                if (!isGood) {
                    return isGood;
                }
            } else {
                CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
                while (charGroup == CharGroup::Digit || charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Unknown) {
                    out.push_back(myInputBuffer.GetChar());
                    charGroup = GetCharGroup(GetNextChar());
                }
                return false;
            }
        }
        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());
        if (charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Unknown) {
            while (charGroup == CharGroup::Digit || charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Unknown) {
                out.push_back(myInputBuffer.GetChar());
                charGroup = GetCharGroup(GetNextChar());
            }
            return false;
        }

        return true;
    }

    bool ProcessIntegerNumber(std::string& out) {
        out.push_back(GetNextChar());
        int internalLen = ProcessNumberInternals(out);

        if (internalLen == 0) {
            return true;
        }

        CharGroup charGroup = GetCharGroup(myInputBuffer.GetChar());

        if (charGroup != CharGroup::Digit) {
            while (charGroup == CharGroup::Digit || charGroup == CharGroup::Alphabetic || charGroup == CharGroup::Unknown) {
                out.push_back(myInputBuffer.GetChar());
                GetCharGroup(GetNextChar());
            }

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

    bool ProcessString(std::string& out) {
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

        return isComplete; 
    }

    bool ProcessOperation(std::string& out) {
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
                return true;
            }

            operation.pop_back();
        }

        return false;
    }

    bool ProcessRawString(std::string& out) {
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());

        while (!(myInputBuffer.GetChar() == BUFFER_EOF || myInputBuffer.GetChar() == '\"' && myInputBuffer.LookAhead(1) == '\"' && myInputBuffer.LookAhead(2) == '\"')) {
            out.push_back(GetNextChar());
        }
        if (myInputBuffer.GetChar() == BUFFER_EOF) {
            return false;
        }

        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        out.push_back(GetNextChar());
        return true;
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
