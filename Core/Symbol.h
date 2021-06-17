#pragma once
#include <string>
#include <vector>

#include "Parser/AbstractNode.h"

class Symbol {
public:
    Symbol();

private:
    std::string myName;

    AbstractNode& myDeclaration;

    std::vector<Symbol> mySymbols;
};

