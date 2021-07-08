#include "InterpreterExceptions.h"

JumpException::JumpException(const char* message) : std::exception(message) {}

char const* JumpException::what() const {
    return "Jump statement occurred";
}

ContinueException::ContinueException() : JumpException("Continue statement occurred") {}
BreakException::BreakException() : JumpException("Break statement occurred") {}
ReturnException::ReturnException() : JumpException("Return statement occurred") {}
