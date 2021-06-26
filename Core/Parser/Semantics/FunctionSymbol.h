#pragma once
#include <vector>

#include "Symbols.h"
#include "SymbolTable.h"
#include "../ParserUtils.h"


class FunctionSymbol : public ISymbol {
public:
    FunctionSymbol(const std::string& name, const ITypeSymbol* returnType, const std::vector<const ITypeSymbol*>& params, Pointer<SymbolTable> table);

    std::string GetName() const override;

    const ITypeSymbol* GetReturnType() const;

    int GetParametersCount() const;

    const ITypeSymbol& GetParameter(int idx) const;
    bool CheckArgument(const ITypeSymbol& type, int idx);

    bool operator<(const ISymbol& rhs) const override;

    std::string ToString() const override;

protected:
    void AcceptVisitor(INodeVisitor& visitor, int depth) const override;

private:
    std::string myName;
    const ITypeSymbol* myReturnType;
    std::vector<const ITypeSymbol*> myParameters;
    Pointer<SymbolTable> myTable;
};
