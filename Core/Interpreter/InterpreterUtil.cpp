#include "InterpreterUtil.h"
#include "../Parser/Semantics/Symbols.h"

const FunctionSymbol* InterpreterUtil::FindMainEntry(const SymbolTable* symbolTable) {
    return dynamic_cast<const FunctionSymbol*>(symbolTable->GetFunction("main", std::vector<const AbstractType*>()));
}

IVariable* InterpreterUtil::TryDereference(IVariable* var) {
    auto ref = dynamic_cast<const Reference*>(var);
    if (ref != nullptr && !dynamic_cast<const Array*>(ref) && !dynamic_cast<const Range*>(ref)) {
        return ref->GetValue<IVariable*>();
    }

    return var;
}

Pointer<Reference> InterpreterUtil::CreateReference(IVariable* var) {
    /*auto ref = dynamic_cast<Reference*>(var);
    if (ref != nullptr) {
        return ref->CloneRef();
    }*/
    return std::make_unique<Reference>(var);
}

