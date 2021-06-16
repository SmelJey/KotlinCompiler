#pragma once

#include "DeclarationNodes.h"
#include "ExpressionNodes.h"
#include "ISyntaxNode.h"
#include "SimpleNodes.h"
#include "StatementNodes.h"
#include "../Lexer/Lexer.h"
#include "ParserUtils.h"

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

    Pointer<ISyntaxNode> ParseExpression();
    Pointer<ISyntaxNode> ParseLeftAssociative(size_t priority);
    Pointer<ISyntaxNode> ParsePrefix();
    Pointer<ISyntaxNode> ParsePostfix();
    Pointer<CallArgumentsNode> ParseArguments(Lexeme::LexemeType rParen);

    Pointer<ISyntaxNode> ParsePrimary();

    Pointer<ISyntaxNode> ParseIfExpression();

    void AddError(ISyntaxNode& root, const Lexeme& location, const std::string& error) const;

    bool ConsumeLexeme(Lexeme::LexemeType lexemeType, ISyntaxNode& host, const std::string& error);
    bool ConsumeLexeme(Lexeme::LexemeType lexemeType, const std::string& text, ISyntaxNode& host, const std::string& error);
    void ConsumeSemicolons();

    template<typename T>
    Pointer<ILexemeNode> CreateLexemeNode(const Lexeme& lexeme) {
        Pointer<ILexemeNode> node = std::make_unique<T>(T(lexeme));
        if (lexeme.IsError()) {
            AddError(*node, lexeme, lexeme.GetValue<std::string>());
        }
        return node;
    }

    Lexer& myLexer;
};
