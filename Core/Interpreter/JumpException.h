#pragma once
#include <stdexcept>

class JumpException : public std::exception {
public:
    JumpException(const char* message);
    char const* what() const override;
};

class ContinueException : public JumpException {
public:
    ContinueException();
};

class BreakException : public JumpException {
public:
    BreakException();
};

class ReturnException : public JumpException {
public:
    ReturnException();
};

