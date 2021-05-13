#include "Parser.h"
#include "BinOperationNode.h"
#include "SimpleNodes.h"

Parser::Parser(Lexer& lexer) : myLexer(lexer) {
    myLexer.NextLexeme();
}

Lexer& Parser::GetLexer() {
    return myLexer;
}

Pointer<ISyntaxNode> Parser::Parse() {
    Pointer<ISyntaxNode> root = ParseExpression();
    

    return root;
}

// Mult | Mult + Expr | Mult - Expr
Pointer<ISyntaxNode> Parser::ParseExpression() {
    Pointer<ISyntaxNode> leftOperand = ParseMult();
    Lexeme operation = myLexer.GetLexeme();
    if (operation.GetType() == Lexeme::LexemeType::OpAdd || operation.GetType() == Lexeme::LexemeType::OpSub) {
        myLexer.NextLexeme();
        Pointer<ISyntaxNode> rightOperand = ParseExpression();
        
        return std::make_unique<BinOperationNode>(BinOperationNode(operation, std::move(leftOperand), std::move(rightOperand)));
    }

    return leftOperand;
}

// Factor | Factor * Mult | Factor / Mult
Pointer<ISyntaxNode> Parser::ParseMult() {
    Pointer<ISyntaxNode> leftOperand = ParseFactor();
    Lexeme operation = myLexer.GetLexeme();
    if (operation.GetType() == Lexeme::LexemeType::OpMult || operation.GetType() == Lexeme::LexemeType::OpDiv) {
        myLexer.NextLexeme();
        Pointer<ISyntaxNode> rightOperand = ParseMult();
        
        return std::make_unique<BinOperationNode>(BinOperationNode(operation, std::move(leftOperand), std::move(rightOperand)));
    }

    return leftOperand;
}

// Identifier | Number
Pointer<ISyntaxNode> Parser::ParseFactor() {
    Lexeme curLexeme = myLexer.NextLexeme();
    if (curLexeme.GetType() == Lexeme::LexemeType::Identifier) {
        return std::make_unique<IdentifierNode>(IdentifierNode(curLexeme));
    }
    if (LexerUtils::IsIntegerType(curLexeme.GetType())) {
        return std::make_unique<IntegerNode>(IntegerNode(curLexeme));
    }

    if (curLexeme.GetType() == Lexeme::LexemeType::LParen) {
        Pointer<ISyntaxNode> expr = ParseExpression();
        Lexeme rParen = myLexer.GetLexeme();
        if (rParen.GetType() != Lexeme::LexemeType::RParen) {
            // TODO: handle exception
            throw "No rparen"; 
        }

        myLexer.NextLexeme();
        return expr;
    }

    // TODO: handle exception
    throw "Unexpected lexeme";
}

