#include "Parser.h"

Lexer& Parser::GetLexer() {
    return myLexer;
}

ISyntaxNode* Parser::Parse() {
    ISyntaxNode* root = ParseExpression();
    return nullptr;
}

ISyntaxNode* Parser::ParseExpression() {
    ISyntaxNode* leftOperand = ParseMult();
    return nullptr;
}

ISyntaxNode* Parser::ParseMult() {
    return nullptr;   
}

