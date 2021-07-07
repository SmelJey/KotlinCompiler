#pragma once
#include "StackFrame.h"

class StackGuard {
public:
    explicit StackGuard(std::stack<StackFrame>& stack, const StackFrame& frame, bool shouldReturn = false);

    ~StackGuard();

private:
    std::stack<StackFrame>& myStack;
    bool myReturn;
};
