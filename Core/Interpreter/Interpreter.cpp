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
    myStack.push(StackFrame());
    dynamic_cast<const FunctionDeclaration*>(myMain->GetDeclaration())->GetBody().RunVisitor(*this);
}

void Interpreter::EnterNode(const IVisitable& node) {}

void Interpreter::EnterNode(const FunctionDeclaration& node) {
    //std::cout << "Entered function " << node.GetIdentifierName() << std::endl;

    myStack.push(StackFrame());
    node.GetBody().RunVisitor(*this);
}

void Interpreter::EnterNode(const BlockNode& node) {
    for (auto& it : node.GetStatements()) {
        it->RunVisitor(*this);
    }
}

void Interpreter::EnterNode(const CallSuffixNode& node) {
    for (auto& it : node.GetArguments().GetArguments()) {
        it->RunVisitor(*this);
    }

    const FunctionSymbol* funcSym = dynamic_cast<const FunctionSymbol*>(node.GetExpression()->GetSymbol());
    if (funcSym->GetDeclaration() == nullptr) {
        //std::cout << "Call to built-in function " << funcSym->GetName() << std::endl;

        if (funcSym->GetName() == "println") {
            Pointer<IVariable> arg = myStack.top().Pop();
            if (dynamic_cast<const IntegerSymbol*>(funcSym->GetParameter(0))) {
                std::cout << arg->GetValue<int>() << std::endl;
            } else if (dynamic_cast<const DoubleSymbol*>(funcSym->GetParameter(0))) {
                std::cout << arg->GetValue<double>() << std::endl;
            } else if (dynamic_cast<const StringSymbol*>(funcSym->GetParameter(0))) {
                std::cout << arg->GetValue<std::string>() << std::endl;
            } else if (dynamic_cast<const BooleanSymbol*>(funcSym->GetParameter(0))) {
                std::cout << (arg->GetValue<bool>() ? "true" : "false") << std::endl;
            }
        } 

        return;
    }

    funcSym->GetDeclaration()->RunVisitor(*this);
}

void Interpreter::EnterNode(const BinOperationNode& node) {
    node.GetLeftOperand().RunVisitor(*this);
    node.GetRightOperand().RunVisitor(*this);


    Pointer<IVariable> rhs = myStack.top().Pop();
    Pointer<IVariable> lhs = myStack.top().Pop();
    myStack.top().Load(rhs->ApplyOperation(node.GetLexeme().GetType(), lhs.get()));
}

void Interpreter::EnterNode(const IntegerNode& node) {
    myStack.top().Load(std::make_unique<Integer>(node.GetLexeme().GetValue<uint64_t>()));
}

void Interpreter::EnterNode(const DoubleNode& node) {
    myStack.top().Load(std::make_unique<Double>(node.GetLexeme().GetValue<double>()));
}

void Interpreter::EnterNode(const BooleanNode& node) {
    myStack.top().Load(std::make_unique<Boolean>(node.GetLexeme().GetValue<bool>()));
}

void Interpreter::EnterNode(const StringNode& node) {
    myStack.top().Load(std::make_unique<String>(node.GetLexeme().GetValue<std::string>()));
}

