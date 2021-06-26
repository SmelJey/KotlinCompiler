#pragma once

#include <string>
#include "../Lexer/Lexeme.h"

class ParserError {
public:
    ParserError(const Lexeme& lexeme, const std::string& error);

    std::pair<int, int> GetLocation() const;
    Lexeme GetLexeme() const;
    std::string GetError() const;

    std::string ToString() const;
private:
    Lexeme myLexeme;
    std::string myError;
};

std::ostream& operator<<(std::ostream& stream, const ParserError& error);
