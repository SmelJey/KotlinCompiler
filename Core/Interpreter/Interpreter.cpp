#include "Interpreter.h"

#include <iomanip>
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

IVariable* Interpreter::LoadOnHeap(Pointer<IVariable> variable) {
    myHeap.push_back(std::move(variable));
    return myHeap.rbegin()->get();
}

void Interpreter::LoadOnStack(Pointer<IVariable> variable) {
    if (dynamic_cast<StructRange*>(variable.get())) {
        auto rangeStruct = dynamic_cast<StructRange*>(LoadOnHeap(std::move(variable)));
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
            Pointer<IVariable> refArg = PopFromStack();
            const IVariable* arg = InterpreterUtil::TryDereference(refArg.get());

            if (dynamic_cast<const IntegerSymbol*>(funcSym->GetParameter(0))) {
                std::cout << arg->GetValue<int>() << std::endl;
            } else if (dynamic_cast<const DoubleSymbol*>(funcSym->GetParameter(0))) {
                double integral;
                if (std::modf(arg->GetValue<double>(), &integral) == 0) {
                    std::cout << std::fixed << std::setprecision(1) << arg->GetValue<double>() << std::endl;
                } else {
                    std::cout << arg->GetValue<double>() << std::endl;
                }
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
            StructArray* arrRef = dynamic_cast<StructArray*>(LoadOnHeap(std::make_unique<StructArray>(params)));
            LoadOnStack(std::make_unique<Array>(arrRef));
        }

        return;
    }

    funcSym->GetDeclaration()->RunVisitor(*this);
}

void Interpreter::EnterNode(const UnaryPrefixOperationNode& node) {
    node.GetOperand().RunVisitor(*this);
    LoadOnStack(PopFromStack()->ApplyOperation(node.GetLexeme().GetType()));
}

void Interpreter::EnterNode(const UnaryPostfixOperationNode& node) {
    node.GetOperand().RunVisitor(*this);
    auto var = PopFromStack();
    LoadOnStack(InterpreterUtil::TryDereference(var.get())->Clone());
    var->ApplyOperation(node.GetLexeme().GetType());
}

void Interpreter::EnterNode(const IndexSuffixNode& node) {
    for (auto& it : node.GetArguments().GetArguments()) {
        it->RunVisitor(*this);
    }

    node.GetExpression()->RunVisitor(*this);
    Pointer<IVariable> arrRef = PopFromStack();
    Pointer<IVariable> idxRef = PopFromStack();
    IVariable* arr = InterpreterUtil::TryDereference(arrRef.get());
    IVariable* idx = InterpreterUtil::TryDereference(idxRef.get());
    LoadOnStack(dynamic_cast<Array*>(arr)->GetIterator(idx->GetValue<int>()));
}

void Interpreter::EnterNode(const BinOperationNode& node) {
    node.GetLeftOperand().RunVisitor(*this);
    node.GetRightOperand().RunVisitor(*this);


    Pointer<IVariable> rhs = PopFromStack();
    Pointer<IVariable> lhs = PopFromStack();
    LoadOnStack(InterpreterUtil::TryDereference(rhs.get())->ApplyOperation(node.GetLexeme().GetType(), InterpreterUtil::TryDereference(lhs.get())));
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

void Interpreter::EnterNode(const IdentifierNode& node) {
    LoadOnStack(myStack.top().GetVariable(node.GetIdentifier()));
}

void Interpreter::EnterNode(const PropertyDeclaration& node) {
    node.GetInitialization().RunVisitor(*this);
    Pointer<IVariable> initRes = PopFromStack();
    Pointer<IVariable> var = InterpreterUtil::TryDereference(initRes.get())->Clone();
    myStack.top().SetVariable(node.GetIdentifierName(), std::move(var));
}

void Interpreter::EnterNode(const Assignment& node) {
    node.GetAssignable().RunVisitor(*this);
    node.GetExpression().RunVisitor(*this);

    Pointer<IVariable> exprRes = PopFromStack();
    Pointer<IVariable> assignable = PopFromStack();

    switch (node.GetLexeme().GetType()) {
        case LexemeType::OpPlusAssign:
            exprRes = InterpreterUtil::TryDereference(exprRes.get())->ApplyOperation(LexemeType::OpAdd, InterpreterUtil::TryDereference(assignable.get()));
            break;
        case LexemeType::OpMinusAssign:
            exprRes = InterpreterUtil::TryDereference(exprRes.get())->ApplyOperation(LexemeType::OpSub, InterpreterUtil::TryDereference(assignable.get()));
            break;
        case LexemeType::OpMultAssign:
            exprRes = InterpreterUtil::TryDereference(exprRes.get())->ApplyOperation(LexemeType::OpMult, InterpreterUtil::TryDereference(assignable.get()));
            break;
        case LexemeType::OpDivAssign:
            exprRes = InterpreterUtil::TryDereference(exprRes.get())->ApplyOperation(LexemeType::OpDiv, InterpreterUtil::TryDereference(assignable.get()));
            break;
        case LexemeType::OpModAssign:
            exprRes = InterpreterUtil::TryDereference(exprRes.get())->ApplyOperation(LexemeType::OpMod, InterpreterUtil::TryDereference(assignable.get()));
            break;
    }

    *assignable->GetValue<IVariable*>() = *InterpreterUtil::TryDereference(exprRes.get())->Clone();
}

void Interpreter::EnterNode(const IfExpression& node) {
    myStack.push(myStack.top().Clone());

    node.GetExpression()->RunVisitor(*this);

    Pointer<IVariable> exprRes = PopFromStack();
    if (InterpreterUtil::TryDereference(exprRes.get())->GetValue<bool>()) {
        node.GetIfBody()->RunVisitor(*this);
    } else {
        node.GetElseBody()->RunVisitor(*this);
    }

    StackFrame lastFrame = std::move(myStack.top());
    myStack.pop();
    if (!lastFrame.Empty()) {
        LoadOnStack(lastFrame.Pop());
    }

}

void Interpreter::EnterNode(const WhileNode& node) {
    node.GetExpression().RunVisitor(*this);

    Pointer<IVariable> exprRes = PopFromStack();
    while (InterpreterUtil::TryDereference(exprRes.get())->GetValue<bool>()) {
        myStack.push(myStack.top().Clone());

        node.GetBody().RunVisitor(*this);

        myStack.pop();

        node.GetExpression().RunVisitor(*this);
        exprRes = PopFromStack();
    }
}

void Interpreter::EnterNode(const DoWhileNode& node) {
    Pointer<IVariable> exprRes = nullptr;
    do {
        myStack.push(myStack.top().Clone());
        node.GetBody().RunVisitor(*this);

        node.GetExpression().RunVisitor(*this);
        exprRes = PopFromStack();

        myStack.pop();
    } while (InterpreterUtil::TryDereference(exprRes.get())->GetValue<bool>());
}

void Interpreter::EnterNode(const ForNode& node) {
    node.GetExpression().RunVisitor(*this);
    Pointer<IVariable> iterablePtr = PopFromStack();
    
    auto iterable = dynamic_cast<IterableRef*>(InterpreterUtil::TryDereference(iterablePtr.get()));
    int iterableSize = iterable->Size();
    if (iterableSize <= 0) {
        return;
    }

    for (int i = 0; i < iterableSize; i++) {
        Pointer<IVariable> iteratorRef = iterable->GetIterator(i);
        IVariable* iterator = InterpreterUtil::TryDereference(iteratorRef.get());

        myStack.push(myStack.top().Clone());
        myStack.top().AddGlobal(node.GetVariable().GetIdentifierName(), iterator);

        node.GetBody().RunVisitor(*this);

        myStack.pop();

    }
}

