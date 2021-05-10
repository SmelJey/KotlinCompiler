#pragma once
#include "ISyntaxNode.h"
#include "../Lexer/Lexer.h"

class Parser {
public:
    Lexer& GetLexer();

    ISyntaxNode* Parse();
private:
    ISyntaxNode* ParseExpression();

    ISyntaxNode* ParseMult();

    Lexer& myLexer;
};
