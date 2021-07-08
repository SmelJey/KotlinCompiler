#include "Class.h"

#include "StackFrame.h"

Struct::Struct(StackFrame frame) : myFrame(std::move(frame)) {}

Pointer<Reference> Struct::GetVariable(const std::string& name) {
    return GetVariable(name);
}

StackFrame Struct::GetLocalSpace() {
    return myFrame.Clone();
}

Pointer<IVariable> Struct::Clone() const {
    // TODO: fix
    throw;
}

Pointer<IVariable> Struct::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    return lhs->ApplyOperation(operation, this);
}

Class::Class(Struct* data) : Reference(data) {}

Pointer<Reference> Class::GetVariable(const std::string& name) {
    return Dereference<Struct>()->GetVariable(name);
}

StackFrame Class::GetLocalSpace() {
    return Dereference<Struct>()->GetLocalSpace();
}

Pointer<Reference> Class::CloneRef() const {
    return std::make_unique<Class>(Dereference<Struct>());
}

Pointer<IVariable> Class::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    if (LexerUtils::IsEqualityOperation(operation)) {
        return CheckStrictEquality(operation, lhs);
    }
    return lhs->ApplyOperation(operation, this);
}
