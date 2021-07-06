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

const IVariable* Interpreter::LoadOnHeap(Pointer<IVariable> variable) {
    myHeap.push_back(std::move(variable));
    return myHeap.rbegin()->get();
}

void Interpreter::LoadOnStack(Pointer<IVariable> variable) {
    if (dynamic_cast<const StructRange*>(variable.get())) {
        auto rangeStruct = dynamic_cast<const StructRange*>(LoadOnHeap(std::move(variable)));
        LoadOnStack(std::make_unique<Range>(rangeStruct));
        return;
    }

    myStack.top().Load(std::move(variable));
}

Pointer<IVariable> Interpreter::PopFromStack() {
    return myStack.top().Pop();
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
        } else if (funcSym->GetName() == "arrayOf") {
            std::vector<const IVariable*> params;
            for (int i = 0; i < funcSym->GetParametersCount(); i++) {
                params.push_back(LoadOnHeap(PopFromStack()));
            }
            std::reverse(params.begin(), params.end());
            const StructArray* arrRef = dynamic_cast<const StructArray*>(LoadOnHeap(std::make_unique<StructArray>(params)));
            LoadOnStack(std::make_unique<Array>(arrRef));
        }

        return;
    }

    funcSym->GetDeclaration()->RunVisitor(*this);
}

void Interpreter::EnterNode(const BinOperationNode& node) {
    node.GetLeftOperand().RunVisitor(*this);
    node.GetRightOperand().RunVisitor(*this);


    Pointer<IVariable> rhs = PopFromStack();
    Pointer<IVariable> lhs = PopFromStack();
    LoadOnStack(rhs->ApplyOperation(node.GetLexeme().GetType(), lhs.get()));
}

void Interpreter::EnterNode(const IntegerNode& node) {
    LoadOnStack(std::make_unique<Integer>(node.GetLexeme().GetValue<uint64_t>()));
}

void Interpreter::EnterNode(const DoubleNode& node) {
    LoadOnStack(std::make_unique<Double>(node.GetLexeme().GetValue<double>()));
}

void Interpreter::EnterNode(const BooleanNode& node) {
    LoadOnStack(std::make_unique<Boolean>(node.GetLexeme().GetValue<std::string>() == "true"));
}

void Interpreter::EnterNode(const StringNode& node) {
    LoadOnStack(std::make_unique<String>(node.GetLexeme().GetValue<std::string>()));
}

