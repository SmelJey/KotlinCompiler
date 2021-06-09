#pragma once

#include "DeclarationNodes.h"
#include "ISyntaxNode.h"
#include "SimpleNodes.h"
#include "../Lexer/Lexer.h"

template<typename T>
using Pointer = std::unique_ptr<T>;

class Parser {
public:
    explicit Parser(Lexer& lexer);

    Lexer& GetLexer();

    Pointer<DeclarationBlock> Parse();
private:
    Pointer<IDeclaration> ParseClass();

    Pointer<IDeclaration> ParseFunction();
    Pointer<ParameterList> ParseParameters(bool isDeclaration);
    Pointer<Parameter> ParseParameter(bool isDeclaration);

    Pointer<TypeNode> ParseType();

    Pointer<BlockNode> ParseBlock();

    Pointer<IDeclaration> ParseProperty();

    Pointer<ISyntaxNode> ParseExpression();
    Pointer<ISyntaxNode> ParseLeftAssociative(size_t priority);

    Pointer<ISyntaxNode> ParseFactor();

    void AddError(ISyntaxNode& root, const Lexeme& location, const std::string& error) const;

    template<typename T>
    Pointer<ISyntaxNode> ReturnNode(const Lexeme& lexeme, Lexeme::LexemeType typeConstraint) {
        if (lexeme.GetType() != typeConstraint) {
            return std::make_unique<ErrorNode>(ErrorNode(lexeme, lexeme.GetValue<std::string>()));
        }
        return ReturnNode<T>(lexeme);
    }

    template<typename T>
    Pointer<ISyntaxNode> ReturnNode(const Lexeme& lexeme) {
        if (lexeme.IsError()) {
            return std::make_unique<ErrorNode>(ErrorNode(lexeme, lexeme.GetValue<std::string>()));
        }
        return std::make_unique<T>(T(lexeme));
    }

    Lexer& myLexer;
};
