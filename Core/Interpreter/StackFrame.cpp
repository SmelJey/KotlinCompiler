#include "StackFrame.h"
#include "InterpreterUtil.h"

void StackFrame::SetVariable(const std::string& name, Pointer<IVariable> variable) {
    myLocals[name] = std::move(variable);
}

Pointer<Reference> StackFrame::GetVariable(const std::string& name) const {
    return InterpreterUtil::CreateReference(myLocals.at(name).get());
}

void StackFrame::Load(Pointer<IVariable> val) {
    myExecutionStack.push(std::move(val));
}

Pointer<IVariable> StackFrame::Pop() {
    Pointer<IVariable> res = std::move(myExecutionStack.top());
    myExecutionStack.pop();
    return res;
}

