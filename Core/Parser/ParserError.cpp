#include "ParserError.h"

#include <sstream>

ParserError::ParserError(const Lexeme& lexeme, const std::string& error) : myLexeme(lexeme), myError(error) {}

std::pair<int, int> ParserError::GetLocation() const {
    return std::make_pair(myLexeme.GetRow() + 1, myLexeme.GetColumn() + 1);
}

Lexeme ParserError::GetLexeme() const {
    return myLexeme;
}

std::string ParserError::GetError() const {
    return myError;
}

std::string ParserError::ToString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

std::ostream& operator<<(std::ostream& stream, const ParserError& error) {
    stream << "Error " << error.GetLocation().first << ":" << error.GetLocation().second << " :: " << error.GetError();
    return stream;
}
