#include "Lexer.h"

Lexer::Lexer(std::ifstream& input) : myInputBuffer(input.rdbuf()),
    myCurrentLexeme(0, 0, "", LexemeType::EndOfFile, Lexeme::DEFAULT_LEXEME_ERROR) {}

Lexer::Lexer(const std::string& filepath) : myInputBuffer(filepath),
    myCurrentLexeme(0, 0, "", LexemeType::EndOfFile, Lexeme::DEFAULT_LEXEME_ERROR) {}

Lexeme Lexer::GetLexeme() const {
    return myCurrentLexeme;
}

Lexeme Lexer::NextLexeme() {
    myPreviousLexeme = myCurrentLexeme;
    if (!myLexemeBuffer.empty()) {
        myCurrentLexeme = myLexemeBuffer.front();
        myLexemeBuffer.pop_front();
    } else {
        myCurrentLexeme = NextFromInput();
    }

    return myPreviousLexeme;
}

Lexeme Lexer::PrevLexeme() {
    return myPreviousLexeme;
}

Lexeme Lexer::NextFromInput() {
    do {
        ResetLexeme();
        ProcessNextLexeme();
    } while (myLexemeType == LexemeType::Ignored);

    return Lexeme(myStartCol, myStartRow, myLexemeText, myLexemeType, myLexemeValue, isError);
}

void Lexer::ProcessNextLexeme() {
    int curChars[]{ myInputBuffer.GetChar(), myInputBuffer.LookAhead(1), myInputBuffer.LookAhead(2) };

    if (LexerUtils::IsDigit(curChars[0])) {
        if (curChars[0] == '0') {
            if (curChars[1] == 'x' || curChars[1] == 'X') {
                ProcessPrefixNumber(&LexerUtils::IsHexChar);
                return;
            }
            if (curChars[1] == 'b' || curChars[1] == 'B') {
                ProcessPrefixNumber(&LexerUtils::IsBinChar);
                return;
            }
        }

        ProcessNumber();
        return;
    }

    if (curChars[0] == '\"') {
        if (curChars[1] == '\"' && curChars[2] == '\"') {
            ProcessRawString();
            return;
        }

        ProcessString();
        return;
    }

    if (curChars[0] == '\'') {
        ProcessChar();
        return;
    }

    if (curChars[0] == '`') {
        ProcessEscapedIdentifier();
        return;
    }

    if (curChars[0] == '/') {
        if (curChars[1] == '/') {
            ProcessComment();
            return;
        }
        if (curChars[1] == '*') {
            ProcessMultilineComment();
            return;
        }
    }

    if (curChars[0] == '#' && curChars[1] == '!') {
        ProcessComment();
        return;
    }

    if (curChars[0] == '.' && LexerUtils::IsDigit(curChars[1])) {
        ProcessNumber();
        return;
    }
    
    if (curChars[0] == 'i' && curChars[1] == 'n') {
        ProcessOperation();
        return;
    }

    if (LexerUtils::IsAlphabetic(curChars[0]) || curChars[0] == '_') {
        ProcessIdentifier();
        return;
    }

    if (curChars[0] == BUFFER_EOF) {
        myLexemeType = LexemeType::EndOfFile;
        return;
    }

    if (LexerUtils::OperationsCharset.count(curChars[0])) {
        ProcessOperation();
        return;
    }
    if (LexerUtils::SpacingCharset.count(curChars[0])) {
        ProcessWhitespaces();
        return;
    }

    ProcessUnknown();
}

void Lexer::ProcessWhitespaces() {
    myLexemeType = LexemeType::Ignored;
    while (myInputBuffer.GetChar() != BUFFER_EOF && LexerUtils::SpacingCharset.count(myInputBuffer.GetChar())) {
        GetNextChar();
    }
}

void Lexer::ProcessComment() {
    myLexemeType = LexemeType::Ignored;
    while (myInputBuffer.GetChar() != BUFFER_EOF && !LexerUtils::NewlineCharset.count(myInputBuffer.GetChar())) {
        AddNextChar();
    }
}

void Lexer::ProcessMultilineComment() {
    myLexemeType = LexemeType::Ignored;
    int nestedCnt = 0;

    while (myInputBuffer.GetChar() != BUFFER_EOF) {
        if (myInputBuffer.GetChar() == '/' && myInputBuffer.LookAhead(1) == '*') {
            nestedCnt++;
            AddNextChar(2);
            continue;
        }

        if (myInputBuffer.GetChar() == '*' && myInputBuffer.LookAhead(1) == '/') {
            nestedCnt--;
            AddNextChar(2);
            if (nestedCnt == 0) {
                myLexemeText.clear();
                return;
            }
        }

        AddNextChar();
    }

    myLexemeType = LexemeType::Error;
    MakeError("Unclosed comment");
}

void Lexer::ProcessIdentifier() {
    while (LexerUtils::IsAlphabetic(myInputBuffer.GetChar()) || LexerUtils::IsDigit(myInputBuffer.GetChar()) || myInputBuffer.GetChar() == '_') {
        AddNextChar();
    }

    myLexemeValue = myLexemeText;
    myLexemeType = LexerUtils::KeywordSet.count(myLexemeText) ? LexemeType::Keyword : LexemeType::Identifier;
}

void Lexer::ProcessEscapedIdentifier() {
    myLexemeType = LexemeType::Identifier;
    AddNextChar();
    while (myInputBuffer.GetChar() != BUFFER_EOF && !LexerUtils::NewlineCharset.count(myInputBuffer.GetChar()) && myInputBuffer.GetChar() != '`') {
        AddNextChar();
    }

    if (myInputBuffer.GetChar() != '`') {
        MakeError("Unclosed escaped identifier");
        return;
    }

    AddNextChar();
    myLexemeValue = myLexemeText.substr(1, myLexemeText.size() - 2);
}

void Lexer::ProcessNumber() {
    myLexemeType = LexemeType::Int;

    if (myInputBuffer.GetChar() != '.' && !ProcessIntegerNumber(&LexerUtils::IsDigit)) {
        MakeError("Illegal underscore");
        AddNextChar();
    }

    if (myInputBuffer.GetChar() == '.' && myInputBuffer.LookAhead(1) != '.') {
        myLexemeType = LexemeType::Double;

        if (!LexerUtils::IsDigit(myInputBuffer.LookAhead(1))) {
            myLexemeValue = myLexemeText;
            myLexemeValue.erase(std::remove(myLexemeValue.begin(), myLexemeValue.end(), '_'), myLexemeValue.end());
            return;
        }

        AddNextChar();

        if (!ProcessIntegerNumber(&LexerUtils::IsDigit)) {
            MakeError("Illegal underscore");
            AddNextChar();
        }
    }

    if (myInputBuffer.GetChar() == 'e') {
        myLexemeType = LexemeType::Double;
        AddNextChar();

        if (myInputBuffer.GetChar() == '+' || myInputBuffer.GetChar() == '-') {
            AddNextChar();
        }

        if (LexerUtils::IsDigit(myInputBuffer.GetChar())) {
            if (!ProcessIntegerNumber(&LexerUtils::IsDigit)) {
                MakeError("Illegal underscore");
                AddNextChar();
            }
        } else {
            MakeError("The value is out of range");
            return;
        }
    }

    if (myInputBuffer.GetChar() == 'f' || myInputBuffer.GetChar() == 'F') {
        AddNextChar();
        myLexemeType = LexemeType::Float;
    }

    if ((myInputBuffer.GetChar() == 'u' || myInputBuffer.GetChar() == 'U') && myLexemeType != LexemeType::Double && myLexemeType != LexemeType::Float) {
        AddNextChar();
        myLexemeType = LexemeType::UInt;
    }

    if (myInputBuffer.GetChar() == 'L' && myLexemeType != LexemeType::Double && myLexemeType != LexemeType::Float) {
        AddNextChar();
        myLexemeType = (myLexemeType == LexemeType::UInt ? LexemeType::ULong : LexemeType::Long);
    }

    if (LexerUtils::IsAlphabetic(myInputBuffer.GetChar()) || LexerUtils::IsDigit(myInputBuffer.GetChar()) || myInputBuffer.GetChar() == '_') {
        ConsumeLexeme();
        MakeError("Illegal suffix");
    }

    if (isError) {
        return;
    }

    myLexemeValue = myLexemeText;
    myLexemeValue.erase(std::remove(myLexemeValue.begin(), myLexemeValue.end(), '_'), myLexemeValue.end());

    if ((myLexemeType == LexemeType::Double || myLexemeType == LexemeType::Float)) {
        if (!LexerUtils::TryGetReal(myLexemeValue)) {
            MakeError("The value is out of range");
        }
    } else if (!LexerUtils::TryGetInteger(myLexemeValue, myLexemeType)) {
        MakeError("The value is out of range");
    }
}

bool Lexer::ProcessIntegerNumber(std::function<bool(int)> isNumber) {
    if (!isNumber(myInputBuffer.GetChar())) {
        return true;
    }

    AddNextChar();
    int len = 0;

    while ((isNumber(myInputBuffer.GetChar()) || myInputBuffer.GetChar() == '_')
           && (isNumber(myInputBuffer.LookAhead(1)) || myInputBuffer.LookAhead(1) == '_')) {
        len++;
        AddNextChar();
    }

    if (!isNumber(myInputBuffer.GetChar())) {
        return len == 0;
    }

    AddNextChar();
    return true;
}

void Lexer::ProcessPrefixNumber(std::function<bool(int)> isNumber) {
    myLexemeType = LexemeType::Int;
    AddNextChar(2);

    bool isDigitEnd = ProcessIntegerNumber(isNumber);

    if (myInputBuffer.GetChar() == 'u' || myInputBuffer.GetChar() == 'U') {
        myLexemeType = LexemeType::UInt;
        AddNextChar();
        isDigitEnd = true;
    }

    if (myInputBuffer.GetChar() == 'L') {
        if (myLexemeType == LexemeType::UInt) {
            myLexemeType = LexemeType::ULong;
        } else {
            myLexemeType = LexemeType::Long;
        }

        AddNextChar();
        isDigitEnd = true;
    }

    if (!isDigitEnd) {
        MakeError("Illegal underscore");
        AddNextChar();
    }

    if (LexerUtils::IsAlphabetic(myInputBuffer.GetChar()) || LexerUtils::IsDigit(myInputBuffer.GetChar()) || myInputBuffer.GetChar() == '_') {
        ConsumeLexeme();
        MakeError("Illegal suffix");
    }

    if (isError) {
        return;
    }

    myLexemeValue = myLexemeText.substr(2);
    myLexemeValue.erase(std::remove(myLexemeValue.begin(), myLexemeValue.end(), '_'), myLexemeValue.end());
    if (!LexerUtils::TryGetInteger(myLexemeValue, myLexemeType, isNumber('F') ? 16 : 2)) {
        MakeError("The value is out of range");
    }
}

void Lexer::ProcessChar() {
    myLexemeType = LexemeType::CharLiteral;
    AddNextChar();
    bool isValidEscape = true;

    if (myInputBuffer.GetChar() == '\\') {
        AddNextChar();
        isValidEscape = LexerUtils::EscapeCharset.count(myInputBuffer.GetChar());
        myLexemeValue.push_back(LexerUtils::EscapeToChar(myInputBuffer.GetChar()));
    }
    if (myInputBuffer.GetChar() == BUFFER_EOF || LexerUtils::NewlineCharset.count(myInputBuffer.GetChar())) {
        MakeError("Incorrect character literal");
        return;
    }

    if (myLexemeValue.empty()) {
        myLexemeValue.push_back(myInputBuffer.GetChar());
    }

    AddNextChar();

    if (myInputBuffer.GetChar() == '\'') {
        AddNextChar();
        if (!isValidEscape) {
            MakeError("Invalid escape");
            return;
        }

        return;
    }

    while (myInputBuffer.GetChar() != BUFFER_EOF && !LexerUtils::SpacingCharset.count(myInputBuffer.GetChar()) && myInputBuffer.GetChar() != '\'') {
        AddNextChar();
    }

    if (myInputBuffer.GetChar() == '\'') {
        AddNextChar();
        MakeError("Too many characters in a character literal");
        return;
    }

    MakeError("Incorrect character literal");
}

void Lexer::ProcessString() {
    bool needUnlock = false;
    if (!isInString) {
        isInString = true;
        needUnlock = true;
    }

    AddNextChar();
    bool isValidEscape = true;
    while (myInputBuffer.GetChar() != BUFFER_EOF && !LexerUtils::NewlineCharset.count(myInputBuffer.GetChar()) && myInputBuffer.GetChar() != '\"') {
        if (myInputBuffer.GetChar() == '$') {
            ProcessStringTemplate(LexemeType::String);
            continue;
        }

        if (myInputBuffer.GetChar() == '\\') {
            if (myInputBuffer.LookAhead(1) == BUFFER_EOF || !LexerUtils::EscapeCharset.count(myInputBuffer.LookAhead(1))) {
                isValidEscape = false;
            }

            AddNextChar();
            myLexemeValue.push_back(LexerUtils::EscapeToChar(myInputBuffer.GetChar()));
            AddNextChar();
            continue;
        }

        myLexemeValue.push_back(myInputBuffer.GetChar());
        AddNextChar();
    }

    myLexemeType = LexemeType::String;

    if (myInputBuffer.GetChar() == '\"') {
        AddNextChar();
        if (!isValidEscape) {
            MakeError("Illegal escape");
        }
    } else {
        MakeError("Expecting '\"'");
    }

    ReturnCurrentStringLexeme(needUnlock);
}

void Lexer::ProcessRawString() {
    myLexemeType = LexemeType::RawString;
    bool needUnlock = false;
    if (!isInString) {
        isInString = true;
        needUnlock = true;
    }

    AddNextChar(3);

    while (!(myInputBuffer.GetChar() == BUFFER_EOF
             || myInputBuffer.GetChar() == '\"' && myInputBuffer.LookAhead(1) == '\"' && myInputBuffer.LookAhead(2) == '\"'
             && myInputBuffer.LookAhead(3) != '\"')) {
        if (myInputBuffer.GetChar() == '$') {
            ProcessStringTemplate(LexemeType::RawString);
            continue;
        }

        myLexemeValue.push_back(myInputBuffer.GetChar());
        AddNextChar();
    }

    if (myInputBuffer.GetChar() == BUFFER_EOF) {
        MakeError("Expecting '\"'");
    } else {
        AddNextChar(3);
    }

    ReturnCurrentStringLexeme(needUnlock);
}

bool Lexer::ProcessStringTemplate(LexemeType stringType) {
    if (myInputBuffer.LookAhead(1) == '{' || LexerUtils::IsAlphabetic(myInputBuffer.LookAhead(1)) || myInputBuffer.LookAhead(1) == '_') {
        myLexemeBuffer.emplace_back(myStartCol, myStartRow, myLexemeText, stringType, myLexemeValue);
        ResetLexeme();

        if (myInputBuffer.LookAhead(1) == '{') {
            AddNextChar(2);
            myLexemeBuffer.emplace_back(myCol - 2, myRow, myLexemeText, LexemeType::StringExpr, myLexemeText);
            ProcessStrExpression();
        } else {
            AddNextChar();
            ProcessIdentifier();
            myLexemeBuffer.emplace_back(myStartCol, myStartRow, myLexemeText, LexemeType::StringRef, myLexemeValue.substr(1));
        }

        ResetLexeme();
        myLexemeType = stringType;
        return true;
    }

    return false;
}

void Lexer::ReturnCurrentStringLexeme(bool unlockString) {
    myLexemeBuffer.emplace_back(myStartCol, myStartRow, myLexemeText, myLexemeType, myLexemeValue);
    if (unlockString) {
        isInString = false;
    } else if (isInString) {
        myLexemeText.clear();
        myLexemeType = LexemeType::Ignored;
        return;
    }

    Lexeme frontLexeme = myLexemeBuffer.front();
    myStartCol = frontLexeme.GetColumn();
    myStartRow = frontLexeme.GetRow();
    myLexemeText = frontLexeme.GetText();
    myLexemeValue = frontLexeme.GetValue<std::string>();
    myLexemeType = frontLexeme.GetType();
    myLexemeBuffer.pop_front();
}

void Lexer::ProcessStrExpression() {
    Lexeme lexeme = NextFromInput();
    while (lexeme.GetType() != LexemeType::EndOfFile) {
        myLexemeBuffer.push_back(lexeme);
        if (lexeme.GetText() == "{") {
            ProcessStrExpression();
        } else if (lexeme.GetText() == "}") {
            return;
        }
        lexeme = NextFromInput();
    }
}

void Lexer::ProcessOperation() {
    std::string operation;
    for (int i = 0; i < 3; i++) {
        int curChar = myInputBuffer.LookAhead(i);
        if (curChar == BUFFER_EOF) {
            break;
        }
        operation.push_back(myInputBuffer.LookAhead(i));
    }

    while (!operation.empty()) {
        if (LexerUtils::OperationsSets[operation.size() - 1].count(operation)) {
            for (auto c : operation) {
                AddNextChar();
            }

            myLexemeType = LexerUtils::OperationsSets[operation.size() - 1].at(operation);
            myLexemeValue = myLexemeText;
            return;
        }

        operation.pop_back();
    }

    AddNextChar();
    myLexemeType = LexemeType::Error;
    MakeError("Unknown operator");
}

void Lexer::ProcessUnknown() {
    myLexemeType = LexemeType::Error;
    ConsumeLexeme();
    MakeError("Unknown lexeme");
}

void Lexer::ConsumeLexeme() {
    while (!LexerUtils::SpacingCharset.count(myInputBuffer.GetChar())
           && !LexerUtils::OperationsCharset.count(myInputBuffer.GetChar())
           && myInputBuffer.GetChar() != BUFFER_EOF) {
        AddNextChar();
    }
}

int Lexer::GetNextChar() {
    int character = myInputBuffer.NextChar();
    if (character != BUFFER_EOF) {
        myCol++;
        if (LexerUtils::NewlineCharset.count(character)) {
            myRow++;
            myCol = 0;
        }
    }

    return character;
}

void Lexer::AddNextChar(int cnt) {
    for (int i = 0; i < cnt; i++) {
        myLexemeText.push_back(GetNextChar());
    }
}

void Lexer::ResetLexeme() {
    myStartCol = myCol;
    myStartRow = myRow;
    myLexemeText.clear();
    myLexemeValue.clear();
    isError = false;
}

void Lexer::MakeError(const std::string& errorMessage) {
    isError = true;
    myLexemeValue = errorMessage;
}
