#include "Interpreter.h"

#include <iomanip>
#include <iostream>

#include "Class.h"
#include "InterpreterUtil.h"
#include "InterpreterExceptions.h"
#include "StackGuard.h"
#include "../Parser/DeclarationNodes.h"
#include "../Parser/ExpressionNodes.h"
#include "../Parser/StatementNodes.h"


Interpreter::Interpreter(const DeclarationBlock* syntaxTree, const SymbolTable* symbolTable)
    : myTree(syntaxTree), myTable(symbolTable), myMain(InterpreterUtil::FindMainEntry(symbolTable)) {}

void Interpreter::RunMain() {
    if (myMain == nullptr) {
        std::cout << "No main method found in project" << std::endl;
        return;
    }

    myStack.push(StackFrame());
    myTree->RunVisitor(*this);

    myStack.push(myStack.top().Clone());
    try {
        dynamic_cast<const FunctionDeclaration*>(myMain->GetDeclaration())->GetBody().RunVisitor(*this);
    } catch (const ReturnException&) {}
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

void Interpreter::EnterNode(const DeclarationBlock& node) {
    for (auto& it : node.GetDeclarations()) {
        it->RunVisitor(*this);
    }
}

void Interpreter::EnterNode(const IVisitable& node) {}

void Interpreter::EnterNode(const FunctionDeclaration& node) {
    myVisibilityMap.emplace(node.GetSymbol(), myStack.top().Clone());
}

void Interpreter::EnterNode(const ClassDeclaration& node) {
    myVisibilityMap.emplace(node.GetSymbol(), myStack.top().Clone());
}

void Interpreter::EnterNode(const BlockNode& node) {
    for (auto& it : node.GetStatements()) {
        it->RunVisitor(*this);
    }
}

void Interpreter::EnterNode(const CallSuffixNode& node) {
    Pointer<IVariable> exprRes;
    if (!dynamic_cast<const IdentifierNode*>(node.GetExpression())) {
        node.GetExpression()->RunVisitor(*this);
        exprRes = PopFromStack();
    }

    for (auto& it : node.GetArguments().GetArguments()) {
        it->RunVisitor(*this);
    }

    const FunctionSymbol* funcSym = dynamic_cast<const FunctionSymbol*>(node.GetExpression()->GetSymbol());
    std::vector<Pointer<IVariable>> refParams;
    std::vector<IVariable*> params;
    for (int i = 0; i < funcSym->GetParametersCount(); i++) {
        refParams.push_back(PopFromStack());
        params.push_back(InterpreterUtil::TryDereference(refParams[i].get()));
    }
    std::reverse(params.begin(), params.end());

    if (funcSym->GetDeclaration() == nullptr) {
        if (funcSym->GetName() == "println") {
            Println(funcSym, params);
        } else if (funcSym->GetName() == "arrayOf") {
            ArrayOf(funcSym, params);
        } else {
            Cast(funcSym, exprRes.get());
        }

        return;
    }

    auto funcDecl = dynamic_cast<const FunctionDeclaration*>(funcSym->GetDeclaration());
    if (funcDecl == nullptr) {
        auto classDecl = dynamic_cast<const ClassDeclaration*>(funcSym->GetDeclaration());
        myStack.push(myVisibilityMap[classDecl->GetSymbol()].Clone());

        if (classDecl->HasBody()) {
            classDecl->GetBody().RunVisitor(*this);
        }

        Struct* classData = dynamic_cast<Struct*>(LoadOnHeap(std::make_unique<Struct>(std::move(myStack.top()))));
        myStack.pop();
        LoadOnStack(std::make_unique<Class>(classData));
        return;
    }

    StackFrame frame = myVisibilityMap[funcSym].Clone();
    if (exprRes != nullptr) {
        frame = dynamic_cast<Class*>(InterpreterUtil::TryDereference(exprRes.get()))->GetLocalSpace();
    }
    StackGuard guard(myStack, frame, true);

    for (uint32_t i = 0; i < params.size(); i++) {
        myStack.top().AddGlobal(funcDecl->GetParameters().GetParameters()[i]->GetIdentifierName(), params[i]);
    }

    try {
        funcDecl->GetBody().RunVisitor(*this);
    } catch (const ReturnException&) {
        if (myReturn != nullptr) {
            LoadOnStack(std::move(myReturn));
        }

        myReturn = nullptr;
    }
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

    Pointer<IVariable> rhsRef = PopFromStack();
    Pointer<IVariable> lhsRef = PopFromStack();
    IVariable* rhs = InterpreterUtil::TryDereference(rhsRef.get());
    IVariable* lhs = InterpreterUtil::TryDereference(lhsRef.get());
    LoadOnStack(rhs->ApplyOperation(node.GetLexeme().GetType(), lhs));
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
            exprRes = InterpreterUtil::TryDereference(exprRes.get())->ApplyOperation(
                LexemeType::OpAdd, InterpreterUtil::TryDereference(assignable.get()));
            break;
        case LexemeType::OpMinusAssign:
            exprRes = InterpreterUtil::TryDereference(exprRes.get())->ApplyOperation(
                LexemeType::OpSub, InterpreterUtil::TryDereference(assignable.get()));
            break;
        case LexemeType::OpMultAssign:
            exprRes = InterpreterUtil::TryDereference(exprRes.get())->ApplyOperation(
                LexemeType::OpMult, InterpreterUtil::TryDereference(assignable.get()));
            break;
        case LexemeType::OpDivAssign:
            exprRes = InterpreterUtil::TryDereference(exprRes.get())->ApplyOperation(
                LexemeType::OpDiv, InterpreterUtil::TryDereference(assignable.get()));
            break;
        case LexemeType::OpModAssign:
            exprRes = InterpreterUtil::TryDereference(exprRes.get())->ApplyOperation(
                LexemeType::OpMod, InterpreterUtil::TryDereference(assignable.get()));
            break;
    }

    *assignable->GetValue<IVariable*>() = *InterpreterUtil::TryDereference(exprRes.get())->Clone();
}

void Interpreter::EnterNode(const ContinueNode& node) {
    throw ContinueException();
}

void Interpreter::EnterNode(const BreakNode& node) {
    throw BreakException();
}

void Interpreter::EnterNode(const ReturnNode& node) {
    if (node.HasExpression() && dynamic_cast<const EmptyStatement*>(node.GetExpression()) == nullptr) {
        node.GetExpression()->RunVisitor(*this);
        myReturn = InterpreterUtil::TryDereference(PopFromStack().get())->Clone();
    }

    throw ReturnException();
}

void Interpreter::EnterNode(const IfExpression& node) {
    StackGuard guard(myStack, myStack.top(), true);

    node.GetExpression()->RunVisitor(*this);

    Pointer<IVariable> exprRes = PopFromStack();
    if (InterpreterUtil::TryDereference(exprRes.get())->GetValue<bool>()) {
        node.GetIfBody()->RunVisitor(*this);
    } else {
        node.GetElseBody()->RunVisitor(*this);
    }
}

void Interpreter::EnterNode(const WhileNode& node) {
    node.GetExpression().RunVisitor(*this);

    Pointer<IVariable> exprRes = PopFromStack();
    while (InterpreterUtil::TryDereference(exprRes.get())->GetValue<bool>()) {
        {
            StackGuard guard(myStack, myStack.top(), false);
            try {
                node.GetBody().RunVisitor(*this);
            } catch (const ContinueException&) {
            } catch (const BreakException&) {
                break;
            }
        }

        node.GetExpression().RunVisitor(*this);
        exprRes = PopFromStack();
    }
}

void Interpreter::EnterNode(const DoWhileNode& node) {
    Pointer<IVariable> exprRes = nullptr;
    do {  
        StackGuard guard(myStack, myStack.top(), false);
        try {
            node.GetBody().RunVisitor(*this);
        } catch (const ContinueException&) {
        } catch (const BreakException&) {
            break;
        }

        node.GetExpression().RunVisitor(*this);
        exprRes = PopFromStack();
        
    } while (InterpreterUtil::TryDereference(exprRes.get())->GetValue<bool>());
}

void Interpreter::EnterNode(const ForNode& node) {
    node.GetExpression().RunVisitor(*this);
    Pointer<IVariable> iterablePtr = PopFromStack();
    
    auto iterable = dynamic_cast<IterableRef*>(InterpreterUtil::TryDereference(iterablePtr.get()));
    int iterableSize = iterable->Size();

    for (int i = 0; i < iterableSize; i++) {
        Pointer<IVariable> iteratorRef = iterable->GetIterator(i);
        IVariable* iterator = InterpreterUtil::TryDereference(iteratorRef.get());

        StackGuard guard(myStack, myStack.top(), false);
        myStack.top().AddGlobal(node.GetVariable().GetIdentifierName(), iterator);

        try {
            node.GetBody().RunVisitor(*this);
        } catch (const ContinueException&) {
        } catch (const BreakException&) {
            break;
        }
    }
}

void Interpreter::EnterNode(const MemberAccessNode& node) {
    node.GetExpression()->RunVisitor(*this);
    auto lhs = PopFromStack();
    auto classVar = dynamic_cast<Class*>(InterpreterUtil::TryDereference(lhs.get()));

    if (classVar == nullptr || !classVar->Contains(node.GetMember()->GetIdentifier())) {
        LoadOnStack(std::move(lhs));
        return;
    }

    StackGuard guard(myStack, classVar->GetLocalSpace(), true);
    node.GetMember()->RunVisitor(*this);
}

void Interpreter::Println(const FunctionSymbol* sym, const std::vector<IVariable*>& params) {
    std::cout.unsetf(std::ios_base::fixed);
    std::cout << std::setprecision(16);

    if (dynamic_cast<const IntegerSymbol*>(sym->GetParameter(0))) {
        std::cout << params[0]->GetValue<int>() << std::endl;
    } else if (dynamic_cast<const DoubleSymbol*>(sym->GetParameter(0))) {
        double integral;
        if (std::modf(params[0]->GetValue<double>(), &integral) == 0) {
            std::cout << std::fixed << std::setprecision(1) << params[0]->GetValue<double>() << std::endl;
        } else {
            std::cout << params[0]->GetValue<double>() << std::endl;
        }
    } else if (dynamic_cast<const StringSymbol*>(sym->GetParameter(0))) {
        std::cout << params[0]->GetValue<std::string>() << std::endl;
    } else if (dynamic_cast<const BooleanSymbol*>(sym->GetParameter(0))) {
        std::cout << (params[0]->GetValue<bool>() ? "true" : "false") << std::endl;
    }
}

void Interpreter::ArrayOf(const FunctionSymbol* sym, const std::vector<IVariable*>& params) {
    std::vector<IVariable*> argsRefs;
    for (int i = 0; i < sym->GetParametersCount(); i++) {
        argsRefs.push_back(LoadOnHeap(params[i]->Clone()));
    }

    StructArray* arrRef = dynamic_cast<StructArray*>(LoadOnHeap(std::make_unique<StructArray>(argsRefs)));
    LoadOnStack(std::make_unique<Array>(arrRef));
}

void Interpreter::Cast(const FunctionSymbol* sym, IVariable* var) {
    auto valType = dynamic_cast<ValueType*>(InterpreterUtil::TryDereference(var));
    Pointer<IVariable> res;
    if (sym->GetName() == "toInt") {
        res = valType->Cast(Integer(0));
    } else if (sym->GetName() == "toString") {
        res = valType->Cast(String(""));
    } else if (sym->GetName() == "toDouble") {
        res = valType->Cast(Double(0));
    }
    LoadOnStack(std::move(res));
}

