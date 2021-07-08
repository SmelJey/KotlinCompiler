#pragma once

#include <stack>

#include "StackFrame.h"
#include "../Parser/INodeVisitor.h"
#include "../Parser/Semantics/SymbolTable.h"

#include "Variable.h"

class Interpreter : public INodeVisitor {
public:
    Interpreter(const DeclarationBlock* syntaxTree, const SymbolTable* symbolTable);

    void RunMain();

    IVariable* LoadOnHeap(Pointer<IVariable> variable);
    void LoadOnStack(Pointer<IVariable> variable);
    Pointer<IVariable> PopFromStack();

    void EnterNode(const DeclarationBlock& node) override;

    void EnterNode(const IVisitable& node) override;
    void EnterNode(const FunctionDeclaration& node) override;
    void EnterNode(const ClassDeclaration& node) override;

    void EnterNode(const BlockNode& node) override;

    void EnterNode(const CallSuffixNode& node) override;
    void EnterNode(const UnaryPrefixOperationNode& node) override;
    void EnterNode(const UnaryPostfixOperationNode& node) override;
    void EnterNode(const IndexSuffixNode& node) override;

    void EnterNode(const BinOperationNode& node) override;
    void EnterNode(const IntegerNode& node) override;
    void EnterNode(const DoubleNode& node) override;
    void EnterNode(const BooleanNode& node) override;
    void EnterNode(const StringNode& node) override;
    void EnterNode(const IdentifierNode& node) override;

    void EnterNode(const PropertyDeclaration& node) override;
    void EnterNode(const Assignment& node) override;

    void EnterNode(const ContinueNode& node) override;
    void EnterNode(const BreakNode& node) override;
    void EnterNode(const ReturnNode& node) override;

    void EnterNode(const IfExpression& node) override;
    void EnterNode(const WhileNode& node) override;
    void EnterNode(const DoWhileNode& node) override;
    void EnterNode(const ForNode& node) override;

    void EnterNode(const MemberAccessNode& node) override;

    void Println(const FunctionSymbol* sym, const std::vector<IVariable*>& params);
    void ArrayOf(const FunctionSymbol* sym, const std::vector<IVariable*>& params);
    void Cast(const FunctionSymbol* sym, IVariable* var);

private:
    const DeclarationBlock* myTree;
    const SymbolTable* myTable;

    const FunctionSymbol* myMain;

    std::stack<StackFrame> myStack;

    std::vector<Pointer<IVariable>> myHeap;
    std::map<const ISymbol*, StackFrame> myVisibilityMap;
    Pointer<IVariable> myReturn;
};
