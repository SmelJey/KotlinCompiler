#pragma once

#include "DeclarationNodes.h"
#include "ExpressionNodes.h"
#include "ISyntaxNode.h"
#include "SimpleNodes.h"
#include "StatementNodes.h"
#include "../Lexer/Lexer.h"

template<typename T>
using Pointer = std::unique_ptr<T>;

class Parser {
public:
    explicit Parser(Lexer& lexer);

    Lexer& GetLexer();

    Pointer<DeclarationBlock> Parse();
private:
    Pointer<DeclarationBlock> ParseDeclarations(bool isClass);
    Pointer<IDeclaration> ParseClass();

    Pointer<IDeclaration> ParseFunction();
    Pointer<ParameterList> ParseParameters();
    Pointer<ParameterNode> ParseParameter();
    Pointer<ISyntaxNode> ParseType();

    Pointer<ISyntaxNode> ParseStatement();

    Pointer<BlockNode> ParseBlock();
    Pointer<ISyntaxNode> ParseControlStructureBody(bool acceptSemicolons);
    Pointer<ForNode> ParseForLoop();
    Pointer<WhileNode> ParseWhileLoop();
    Pointer<DoWhileNode> ParseDoWhileLoop();

    Pointer<ISyntaxNode> ParseAssignment();

    Pointer<IDeclaration> ParseProperty();
    Pointer<VariableNode> ParseVariable();
    Pointer<IdentifierNode> ParseIdentifier(const std::string& errorMessage = "Identifier expected");

    Pointer<ILexemeNode> ParseExpression();
    Pointer<ILexemeNode> ParseLeftAssociative(size_t priority);
    Pointer<ILexemeNode> ParsePrefix();
    Pointer<ILexemeNode> ParsePostfix();
    std::vector<Pointer<ISyntaxNode>> ParseArguments(ISyntaxNode& host, Lexeme::LexemeType rParen);

    Pointer<ILexemeNode> ParsePrimary();

    Pointer<ILexemeNode> ParseIfExpression();

    void AddError(ISyntaxNode& root, const Lexeme& location, const std::string& error) const;

    bool ConsumeLexeme(Lexeme::LexemeType lexemeType, ISyntaxNode& host, const std::string& error);
    bool ConsumeLexeme(Lexeme::LexemeType lexemeType, const std::string& text, ISyntaxNode& host, const std::string& error);
    void ConsumeSemicolons();

    template<typename T>
    Pointer<ILexemeNode> CreateNodeOfType(const Lexeme& lexeme, Lexeme::LexemeType typeConstraint) {
        if (lexeme.GetType() != typeConstraint) {
            return std::make_unique<ErrorNode>(ErrorNode(lexeme, "Unexpected lexeme"));
        }
        return CreateNodeOfType<T>(lexeme);
    }

    template<typename T>
    Pointer<ILexemeNode> CreateNodeOfType(const Lexeme& lexeme) {
        if (lexeme.IsError()) {
            return std::make_unique<ErrorNode>(ErrorNode(lexeme, lexeme.GetValue<std::string>()));
        }
        return std::make_unique<T>(T(lexeme));
    }

    Lexer& myLexer;
};
