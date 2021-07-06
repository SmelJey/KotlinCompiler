#include "Interpreter.h"

#include <iostream>

#include "InterpreterUtil.h"
#include "../Parser/DeclarationNodes.h"
#include "../Parser/ExpressionNodes.h"
#include "../Parser/StatementNodes.h"

Interpreter::Interpreter(const DeclarationBlock* syntaxTree, const SymbolTable* symbolTable)
    : myTree(syntaxTree), myTable(symbolTable), myMain(InterpreterUtil::FindMainEntry(symbolTable)) {}

void Interpreter::RunMain() {
    // TODO: Check if no main function
    myMain->GetDeclaration()->RunVisitor(*this);
}

void Interpreter::EnterNode(const IVisitable& node) {}

void Interpreter::EnterNode(const FunctionDeclaration& node) {
    std::cout << "Entered function " << node.GetIdentifierName() << std::endl;
}

void Interpreter::EnterNode(const CallSuffixNode& node) {
    const FunctionSymbol* funcSym = dynamic_cast<const FunctionSymbol*>(node.GetExpression()->GetSymbol());
    if (funcSym->GetDeclaration() == nullptr) {
        std::cout << "Call to built-in function " << funcSym->GetName() << std::endl; 
        // TODO: process built in functions

        return;
    }

    funcSym->GetDeclaration()->RunVisitor(*this);
}

void Interpreter::ExitNode(const BinOperationNode& node) {
    
}
