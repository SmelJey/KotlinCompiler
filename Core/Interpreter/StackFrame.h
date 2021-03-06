#pragma once
#include <map>
#include <string>
#include <stack>

#include "Variable.h"

class StackFrame {
public:
    StackFrame Clone() const;

    void SetVariable(const std::string& name, Pointer<IVariable> variable);
    void AddGlobal(const std::string& name, IVariable* variable);
    Pointer<Reference> GetVariable(const std::string& name) const;
    bool Contains(const std::string& name) const;

    void Load(Pointer<IVariable> val);
    Pointer<IVariable> Pop();
    bool Empty() const;

private:
    std::map<std::string, Pointer<IVariable>> myLocals;
    std::map<std::string, IVariable*> myGlobals;
    std::stack<Pointer<IVariable>> myExecutionStack;
};
