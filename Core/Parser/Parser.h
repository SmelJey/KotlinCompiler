#pragma once

#include "DeclarationNodes.h"
#include "ExpressionNodes.h"
#include "ISyntaxNode.h"
#include "SimpleNodes.h"
#include "StatementNodes.h"
#include "../Lexer/Lexer.h"
#include "Semantics/SymbolTable.h"

class Parser {
public:
    explicit Parser(Lexer& lexer, SymbolTable* symbolTable);

    const Lexer& GetLexer() const;
    const SymbolTable& GetSymbolTable() const;

    Pointer<DeclarationBlock> Parse();

    const std::vector<ErrorNode>& GetParsingErrors() const;
    const std::vector<ErrorNode>& GetSemanticsErrors() const;
private:
    Pointer<DeclarationBlock> ParseDeclarations(bool isClass);
    Pointer<ClassDeclaration> ParseClass();

    Pointer<FunctionDeclaration> ParseFunction();
    Pointer<ParameterList> ParseParameters();
    Pointer<ParameterNode> ParseParameter();
    Pointer<TypeNode> ParseType();

    Pointer<ISyntaxNode> ParseStatement();

    Pointer<BlockNode> ParseBlock();
    Pointer<ISyntaxNode> ParseControlStructureBody(bool acceptSemicolons);
    Pointer<ForNode> ParseForLoop();
    Pointer<WhileNode> ParseWhileLoop();
    Pointer<DoWhileNode> ParseDoWhileLoop();

    Pointer<ISyntaxNode> ParseAssignment();

    Pointer<PropertyDeclaration> ParseProperty();
    Pointer<VariableNode> ParseVariable();
    Pointer<IdentifierNode> ParseIdentifier(const std::string& errorMessage = "Identifier expected");

    Pointer<ITypedNode> ParseExpression();
    Pointer<ITypedNode> ParseLeftAssociative(size_t priority);
    Pointer<ITypedNode> ParsePrefix();
    Pointer<ITypedNode> ParsePostfix();

    Pointer<TypeArgumentsNode> ParseTypeArguments();
    Pointer<CallArgumentsNode> ParseArguments(LexemeType rParen);

    Pointer<ITypedNode> ParsePrimary();

    Pointer<IfExpression> ParseIfExpression();

    Pointer<IdentifierNode> CreateEmptyIdentifier(const Lexeme& lexeme);
    void AddParsingError(const Lexeme& location, const std::string& error);
    void AddSemanticsError(const Lexeme& location, const std::string& error);

    bool ConsumeLexeme(LexemeType lexemeType, ISyntaxNode& host, const std::string& error);
    bool ConsumeLexeme(LexemeType lexemeType, const std::string& text, ISyntaxNode& host, const std::string& error);
    void ConsumeSemicolons();

    template<typename T>
    Pointer<T> CreateLexemeNode(const Lexeme& lexeme) {
        Pointer<T> node = std::make_unique<T>(lexeme);
        if (lexeme.IsError()) {
            AddParsingError(lexeme, lexeme.GetValue<std::string>());
        }
        return node;
    }

    template<typename T>
    Pointer<T> CreateLexemeNode(const Lexeme& lexeme, const ITypeSymbol* type) {
        Pointer<T> node = std::make_unique<T>(lexeme, type);
        if (lexeme.IsError()) {
            AddParsingError(lexeme, lexeme.GetValue<std::string>());
        }
        return node;
    }

    Pointer<IdentifierNode> CreateLexemeNode(const Lexeme& lexeme, const ITypeSymbol* type, const std::vector<const ISymbol*>& candidates) {
        Pointer<IdentifierNode> node = std::make_unique<IdentifierNode>(lexeme, type, candidates);
        if (lexeme.IsError()) {
            AddParsingError(lexeme, lexeme.GetValue<std::string>());
        }
        return node;
    }

    Lexer& myLexer;
    SymbolTable* myRootTable;
    SymbolTable* myTable;
    std::vector<ErrorNode> myParsingErrors;
    std::vector<ErrorNode> mySemanticsErrors;
};
