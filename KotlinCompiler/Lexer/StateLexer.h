#pragma once

#include "ILexer.h"
#include "Lexeme.h"
#include "LexerState.h"
#include "LexerStates.h"
#include "InputBuffer.h"
#include <fstream>

template<typename InputType>
class StateLexer : public ILexer {
public:
    constexpr static char EOF_VAL = -1;

    StateLexer(const std::string& filepath) : ILexer(), myInput(filepath), myLexerState(&StartState::Instance()) {}
private:

    void SyncCoords(int character) {
        if (character == EOF_VAL) {
            return;
        }

        myCurCol++;
        if (LexerState::NewlineCharset.count(character)) {
            myCurRow++;
            myCurCol = 0;
        }
    }

    Lexeme GetSingleLexemeFromInput() {
        std::string newLexemeString;

        int startRow = myCurRow;
        int startCol = myCurCol;

        if (myInput.eof()) {
            return Lexeme(startCol, startRow, "", Lexeme::LexemeType::EndOfFile);
        }

        if (myLastChar != EOF_VAL) {
            newLexemeString.push_back(myLastChar);
            startCol--;
        }

        do {
            //myLastChar = myInput.NextChar();
            if (!myInput.get(myLastChar)) {
                myLastChar = EOF_VAL;
            }

            SyncCoords(myLastChar);

            auto [newLexerState, newLexemeType] = myLexerState->ProcessCharacter(myLastChar);
            myLexemeType = newLexemeType;
            myLexerState = &newLexerState;
            if (myLexemeType == Lexeme::LexemeType::Ignored) {
                startCol = myCurCol - 1;
                startRow = myCurRow;
                newLexemeString.clear();
            }

            newLexemeString.push_back(myLastChar);
        } while (!myInput.eof() && (myLexemeType == Lexeme::LexemeType::Incomplete || myLexemeType == Lexeme::LexemeType::Ignored));

        newLexemeString.pop_back();
        return  Lexeme(startCol, startRow, newLexemeString, myLexemeType);
    }

    Lexeme GetLexemeFromInput() override {
        Lexeme currentLexeme;
        do {
            currentLexeme = myNextLexeme;
            if (currentLexeme.GetType() == Lexeme::LexemeType::EndOfFile) {
                currentLexeme = GetSingleLexemeFromInput();
            }

            myNextLexeme = GetSingleLexemeFromInput();
            LexerState* newState = &currentLexeme.TryToMerge(myNextLexeme);

            while (newState != &BadState::Instance()) {
                myLexerState = newState;
                auto [newLexerState, lexemeType] = myLexerState->ProcessCharacter(myLastChar);
                myLexerState = &newLexerState;
                myLexemeType = lexemeType;
                myNextLexeme = GetSingleLexemeFromInput();
               
                newState = &currentLexeme.TryToMerge(myNextLexeme);
            }

            
        } while (currentLexeme.GetType() == Lexeme::LexemeType::Ignored);
        return currentLexeme;
    }

    int myCurRow = 0;
    int myCurCol = 0;
    char myLastChar = 0;
    InputType myInput;
    //InputBuffer<InputType> myInput;
    LexerState* myLexerState;
    Lexeme::LexemeType myLexemeType = Lexeme::LexemeType::Ignored;
    Lexeme myNextLexeme = Lexeme(0, 0, "", Lexeme::LexemeType::EndOfFile);
};