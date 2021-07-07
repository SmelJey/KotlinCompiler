#include "StackFrame.h"
#include "InterpreterUtil.h"

StackFrame StackFrame::Clone() const {
    StackFrame newFrame;
    newFrame.myGlobals = myGlobals;
    for (auto& local : myLocals) {
        newFrame.myGlobals[local.first] = local.second.get();
    }
    return newFrame;
}

void StackFrame::SetVariable(const std::string& name, Pointer<IVariable> variable) {
    myLocals[name] = std::move(variable);
}

void StackFrame::AddGlobal(const std::string& name, IVariable* variable) {
    myGlobals[name] = variable;
}

Pointer<Reference> StackFrame::GetVariable(const std::string& name) const {
    if (myLocals.count(name)) {
        return InterpreterUtil::CreateReference(myLocals.at(name).get());
    }

    return InterpreterUtil::CreateReference(myGlobals.at(name));
}

void StackFrame::Load(Pointer<IVariable> val) {
    myExecutionStack.push(std::move(val));
}

Pointer<IVariable> StackFrame::Pop() {
    Pointer<IVariable> res = std::move(myExecutionStack.top());
    myExecutionStack.pop();
    return res;
}

bool StackFrame::Empty() const {
    return myExecutionStack.empty();
}

