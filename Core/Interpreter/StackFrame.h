#pragma once
#include <map>
#include <string>
#include <stack>

#include "Variable.h"

class StackFrame {
public:
    void AddVariable(const std::string& name, Pointer<IVariable> variable);
    const IVariable* GetVariable(const std::string& name) const;

    void Load(Pointer<IVariable> val);
    Pointer<IVariable> Pop();

private:
    std::map<std::string, Pointer<IVariable>> myLocals;
    std::map<std::string, const IVariable*> myGlobals;
    std::stack<Pointer<IVariable>> myExecutionStack;
};