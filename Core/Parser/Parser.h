#pragma once

#include "DeclarationNodes.h"
#include "ExpressionNodes.h"
#include "AbstractNode.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"
#include "StatementNodes.h"
#include "../Lexer/Lexer.h"

class Parser {
public:
    explicit Parser(Lexer& lexer);

    Lexer& GetLexer();

    Pointer<DeclarationBlock> Parse();
private:
    Pointer<DeclarationBlock> ParseDeclarations(bool isClass);
    Pointer<ClassDeclaration> ParseClass();

    Pointer<FunctionDeclaration> ParseFunction();
    Pointer<ParameterList> ParseParameters();
    Pointer<ParameterNode> ParseParameter();
    Pointer<AbstractNode> ParseType();

    Pointer<AbstractNode> ParseStatement();

    Pointer<BlockNode> ParseBlock();
    Pointer<AbstractNode> ParseControlStructureBody(bool acceptSemicolons);
    Pointer<ForNode> ParseForLoop();
    Pointer<WhileNode> ParseWhileLoop();
    Pointer<DoWhileNode> ParseDoWhileLoop();

    Pointer<AbstractNode> ParseAssignment();

    Pointer<PropertyDeclaration> ParseProperty();
    Pointer<VariableNode> ParseVariable();
    Pointer<IdentifierNode> ParseIdentifier(const std::string& errorMessage = "Identifier expected");

    Pointer<AbstractNode> ParseExpression();
    Pointer<AbstractNode> ParseLeftAssociative(size_t priority);
    Pointer<AbstractNode> ParsePrefix();
    Pointer<AbstractNode> ParsePostfix();
    Pointer<CallArgumentsNode> ParseArguments(LexemeType rParen);

    Pointer<AbstractNode> ParsePrimary();

    Pointer<IfExpression> ParseIfExpression();

    void AddError(AbstractNode& root, const Lexeme& location, const std::string& error) const;

    bool ConsumeLexeme(LexemeType lexemeType, AbstractNode& host, const std::string& error);
    bool ConsumeLexeme(LexemeType lexemeType, const std::string& text, AbstractNode& host, const std::string& error);
    void ConsumeSemicolons();

    template<typename T>
    Pointer<T> CreateLexemeNode(const Lexeme& lexeme) {
        Pointer<T> node = std::make_unique<T>(T(lexeme));
        if (lexeme.IsError()) {
            AddError(*node, lexeme, lexeme.GetValue<std::string>());
        }
        return node;
    }

    Lexer& myLexer;
};
