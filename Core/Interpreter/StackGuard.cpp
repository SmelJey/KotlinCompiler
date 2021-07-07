#include "StackGuard.h"

StackGuard::StackGuard(std::stack<StackFrame>& stack, const StackFrame& frame, bool shouldReturn) : myStack(stack), myReturn(shouldReturn) {
    stack.push(frame.Clone());
}

StackGuard::~StackGuard() {
    StackFrame lastFrame = std::move(myStack.top());
    myStack.pop();
    if (myReturn && !lastFrame.Empty()) {
        myStack.top().Load(lastFrame.Pop());
    }
}
