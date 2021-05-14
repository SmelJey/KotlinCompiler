#pragma once

#include "ISyntaxNode.h"
#include "SimpleNodes.h"
#include "../Lexer/Lexer.h"

template<typename T>
using Pointer = std::unique_ptr<T>;

class Parser {
public:
    explicit Parser(Lexer& lexer);

    Lexer& GetLexer();

    Pointer<ISyntaxNode> Parse();
private:
    Pointer<ISyntaxNode> ParseLeftAssociative(size_t priority);

    Pointer<ISyntaxNode> ParseFactor();

    void AddError(ISyntaxNode& root, const Lexeme& location, const std::string& error) const;

    template<typename T>
    Pointer<ISyntaxNode> ReturnNode(const Lexeme& lexeme) {
        if (lexeme.IsError()) {
            return std::make_unique<ErrorNode>(ErrorNode(lexeme, lexeme.GetValue<std::string>()));
        }
        return std::make_unique<T>(T(lexeme));
    }

    Lexer& myLexer;
};
