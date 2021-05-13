#include "Parser.h"
#include "OperationNode.h"
#include "SimpleNodes.h"

Parser::Parser(Lexer& lexer) : myLexer(lexer) {
    myLexer.NextLexeme();
}

Lexer& Parser::GetLexer() {
    return myLexer;
}

Pointer<ISyntaxNode> Parser::Parse() {
    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::EndOfFile) {
        return nullptr;
    }

    Pointer<ISyntaxNode> root = ParseExpression();
    return root;
}

// Mult | Mult + Expr | Mult - Expr
Pointer<ISyntaxNode> Parser::ParseExpression() {
    Pointer<ISyntaxNode> leftOperand = ParseMult();
    Lexeme operation = myLexer.GetLexeme();
    while (operation.GetType() == Lexeme::LexemeType::OpAdd || operation.GetType() == Lexeme::LexemeType::OpSub) {
        myLexer.NextLexeme();
        Pointer<ISyntaxNode> rightOperand = ParseMult();
        
        leftOperand = std::make_unique<BinOperationNode>(BinOperationNode(operation, std::move(leftOperand), std::move(rightOperand)));
        operation = myLexer.GetLexeme();
    }

    return leftOperand;
}

// Factor | Factor * Mult | Factor / Mult
Pointer<ISyntaxNode> Parser::ParseMult() {
    Pointer<ISyntaxNode> leftOperand = ParseFactor();
    Lexeme operation = myLexer.GetLexeme();
    while (operation.GetType() == Lexeme::LexemeType::OpMult || operation.GetType() == Lexeme::LexemeType::OpDiv) {
        myLexer.NextLexeme();
        Pointer<ISyntaxNode> rightOperand = ParseFactor();
        
        leftOperand = std::make_unique<BinOperationNode>(BinOperationNode(operation, std::move(leftOperand), std::move(rightOperand)));
        operation = myLexer.GetLexeme();
    }

    return leftOperand;
}

// Identifier | Number
Pointer<ISyntaxNode> Parser::ParseFactor() {
    const Lexeme curLexeme = myLexer.NextLexeme();
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
            expr->AddError(MakeError("Expecting \")\""));
        }

        myLexer.NextLexeme();
        return expr;
    }

    if (curLexeme.GetType() == Lexeme::LexemeType::OpSub) {
        Pointer<ISyntaxNode> operand = ParseFactor();
        return std::make_unique<UnaryOperationNode>(UnaryOperationNode(curLexeme, std::move(operand)));
    }

    return std::make_unique<UnexpectedNode>(UnexpectedNode(curLexeme));
}

Pointer<ISyntaxNode> Parser::MakeError(const std::string& error) const {
    return std::make_unique<ErrorNode>(ErrorNode(error));
}

