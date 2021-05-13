#pragma once

#include "ISyntaxNode.h"
#include "../Lexer/Lexer.h"

template<typename T>
using Pointer = std::unique_ptr<T>;

class Parser {
public:
    explicit Parser(Lexer& lexer);

    Lexer& GetLexer();

    Pointer<ISyntaxNode> Parse();
private:
    Pointer<ISyntaxNode> ParseLeftRecursive(size_t priority);

    Pointer<ISyntaxNode> ParseFactor();

    void AddError(ISyntaxNode& root, const Lexeme& location, const std::string& error) const;

    Lexer& myLexer;
};
