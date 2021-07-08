#pragma once

#include <stack>

#include "DeclarationNodes.h"
#include "ExpressionNodes.h"
#include "ISyntaxNode.h"
#include "SimpleNodes.h"
#include "StatementNodes.h"
#include "../Lexer/Lexer.h"
#include "Semantics/SymbolTable.h"

class ParserError;

class Parser {
public:
    explicit Parser(Lexer& lexer, SymbolTable* symbolTable);

    const Lexer& GetLexer() const;
    const SymbolTable& GetSymbolTable() const;

    Pointer<DeclarationBlock> Parse();

    const std::vector<ParserError>& GetParsingErrors() const;
    const std::vector<ParserError>& GetSemanticsErrors() const;
private:
    Pointer<DeclarationBlock> ParseDeclarations(bool isClass);
    Pointer<ClassDeclaration> ParseClass();

    Pointer<FunctionDeclaration> ParseFunction();
    Pointer<ParameterList> ParseParameters();
    Pointer<ParameterNode> ParseParameter();
    Pointer<TypeNode> ParseType();

    Pointer<IAnnotatedNode> ParseStatement();

    Pointer<BlockNode> ParseBlock();
    Pointer<IAnnotatedNode> ParseControlStructureBody(bool acceptSemicolons);
    Pointer<ForNode> ParseForLoop(const Lexeme& lexeme);
    Pointer<WhileNode> ParseWhileLoop(const Lexeme& lexeme);
    Pointer<DoWhileNode> ParseDoWhileLoop(const Lexeme& lexeme);

    Pointer<IAnnotatedNode> ParseAssignment();

    Pointer<PropertyDeclaration> ParseProperty(const Lexeme& keyword);
    Pointer<VariableNode> ParseVariable();
    Pointer<IdentifierNode> ParseIdentifier(const std::string& errorMessage = "Identifier expected");

    Pointer<IAnnotatedNode> ParseExpression();
    Pointer<IAnnotatedNode> ParseLeftAssociative(size_t priority);
    Pointer<IAnnotatedNode> ParsePrefix();

    Pointer<IAnnotatedNode> ParsePostfix();
    Pointer<MemberAccessNode> ParseMemberAccess(const Lexeme& operationLexeme, Pointer<IAnnotatedNode> operand);
    Pointer<CallSuffixNode> ParseCall(const Lexeme& operationLexeme, Pointer<IAnnotatedNode> operand);

    Pointer<TypeArgumentsNode> ParseTypeArguments();
    Pointer<CallArgumentsNode> ParseArguments(LexemeType rParen);

    Pointer<IAnnotatedNode> ParsePrimary();

    Pointer<IfExpression> ParseIfExpression(const Lexeme& lexeme);

    Pointer<IdentifierNode> CreateEmptyIdentifier(const Lexeme& lexeme) const;
    void AddParsingError(const Lexeme& location, const std::string& error);
    void AddSemanticsError(const Lexeme& location, const std::string& error);

    bool RequireLexeme(LexemeType lexemeType, const std::string& error);
    bool RequireLexeme(LexemeType lexemeType, const std::string& text, const std::string& error);
    void ConsumeSemicolons();

    Pointer<EmptyStatement> CreateEmptyStatement(const Lexeme& lexeme);
    Pointer<AbstractType> IsApplicable(LexemeType operation, const AbstractType* left, const AbstractType* right);
    Pointer<AbstractType> IsApplicable(LexemeType operation, const AbstractType* left);

    const ISymbol* CheckUnresolvedType(const ISymbol* symbol, const std::string& error, const Lexeme& lexeme);

    template<typename T>
    bool CheckType(const AbstractType* symbol, const Lexeme& lexeme) {
        if (*symbol == T(myRootTable)) {
            return true;
        }

        AddSemanticsError(lexeme, symbol->GetName() + " does not conform to the expected type " + T(myRootTable).GetName());
        return false;
    }

    bool AcceptLexeme(LexemeType lexType);
    bool AcceptLexeme(LexemeType lexType, const std::string& text);
    bool AcceptLexeme(std::initializer_list<LexemeType> lexTypes);
    

    template<typename T>
    Pointer<T> CreateLexemeNode(const Lexeme& lexeme) {
        Pointer<T> node = std::make_unique<T>(lexeme);
        if (lexeme.IsError()) {
            AddParsingError(lexeme, lexeme.GetValue<std::string>());
        }
        return node;
    }

    template<typename T, typename U = AbstractType>
    Pointer<T> CreateLexemeNode(const Lexeme& lexeme, const U* type) {
        Pointer<T> node = std::make_unique<T>(lexeme, type);
        if (lexeme.IsError()) {
            AddParsingError(lexeme, lexeme.GetValue<std::string>());
        }
        return node;
    }

    Pointer<IdentifierNode> CreateLexemeNode(const Lexeme& lexeme, const AbstractType* type, const std::vector<const ISymbol*>& candidates) {
        Pointer<IdentifierNode> node = std::make_unique<IdentifierNode>(lexeme, type, candidates);
        if (lexeme.IsError()) {
            AddParsingError(lexeme, lexeme.GetValue<std::string>());
        }
        return node;
    }

    Lexer& myLexer;
    SymbolTable* myRootTable;
    SymbolTable* myTable;
    std::vector<ParserError> myParsingErrors;
    std::vector<ParserError> mySemanticsErrors;
    bool wasError = false;

    std::stack<const AbstractType*> myReturns;
    int inLoop = 0;
};
