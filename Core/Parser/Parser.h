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
    Pointer<ISyntaxNode> ParseExpression();

    Pointer<ISyntaxNode> ParseMult();

    Pointer<ISyntaxNode> ParseFactor();

    Lexer& myLexer;
};
