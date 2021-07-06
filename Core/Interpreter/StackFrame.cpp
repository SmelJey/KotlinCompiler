#include "StackFrame.h"

void StackFrame::AddVariable(const std::string& name, Pointer<IVariable> variable) {
    myLocals[name] = std::move(variable);
}

const IVariable* StackFrame::GetVariable(const std::string& name) const {
    return myLocals.at(name).get();
}

void StackFrame::Load(Pointer<IVariable> val) {
    myExecutionStack.push(std::move(val));
}

Pointer<IVariable> StackFrame::Pop() {
    Pointer<IVariable> res = std::move(myExecutionStack.top());
    myExecutionStack.pop();
    return res;
}

