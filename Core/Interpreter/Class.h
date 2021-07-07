#pragma once
#include "StackFrame.h"
#include "Variable.h"

class Struct : public IVariable {
public:
    explicit Struct(StackFrame frame);

    Pointer<Reference> GetVariable(const std::string& name);

    Pointer<IVariable> Clone() const override;
    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;

private:
    StackFrame myFrame;
};

class Class : public Reference {
public:
    explicit Class(Struct* data);

    Pointer<Reference> GetVariable(const std::string& name);

    Pointer<Reference> CloneRef() const override;

    Pointer<IVariable> ApplyOperation(LexemeType operation, const IVariable* lhs) const override;
};
