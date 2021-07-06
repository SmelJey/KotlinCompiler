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

    void EnterNode(const IVisitable& node) override;
    void EnterNode(const FunctionDeclaration& node) override;
    void EnterNode(const BlockNode& node) override;

    void EnterNode(const CallSuffixNode& node) override;

    void EnterNode(const BinOperationNode& node) override;
    void EnterNode(const IntegerNode& node) override;
private:
    const DeclarationBlock* myTree;
    const SymbolTable* myTable;

    const FunctionSymbol* myMain;

    std::stack<StackFrame> myStack;
    //std::vector<>
};