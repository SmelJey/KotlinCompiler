#pragma once
#include "../Lexer/Lexeme.h"

class ISyntaxNode {
public:
    virtual ~ISyntaxNode();

    std::string ToString() const;

    friend std::ostream& operator<<(std::ostream& os, const ISyntaxNode& node);
protected:
    virtual std::string GetName() const = 0;
};
