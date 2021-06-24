#include "FunctionSymbol.h"

FunctionSymbol::FunctionSymbol(const std::string& name, const ITypeSymbol* returnType,
    const std::vector<const ITypeSymbol*>& params, Pointer<SymbolTable> table)
        : myName(name), myReturnType(returnType), myParameters(params), myTable(std::move(table)) {}

std::string FunctionSymbol::GetName() const {
    return myName;
}

const ITypeSymbol* FunctionSymbol::GetReturnType() const {
    return myReturnType;
}

int FunctionSymbol::GetParametersCount() const {
    return myParameters.size();
}

const ITypeSymbol& FunctionSymbol::GetParameter(int idx) const {
    return *myParameters[idx];
}

bool FunctionSymbol::CheckArgument(const ITypeSymbol& type, int idx) {
    if (idx >= myParameters.size()) {
        return false;
    }

    return type == *myParameters[idx];
}

bool FunctionSymbol::operator<(const ISymbol& rhs) const {
    bool res = ISymbol::operator<(rhs);
    if (!res) {
        const auto& rhsFunc = dynamic_cast<const FunctionSymbol&>(rhs);
        if (GetReturnType() == rhsFunc.GetReturnType()) {
            if (GetParametersCount() == rhsFunc.GetParametersCount()) {
                for (int i = 0; i < myParameters.size(); i++) {
                    if (*myParameters[i] < rhsFunc.GetParameter(i)) {
                        return true;
                    }
                }

                return false;
            }

            return GetParametersCount() < rhsFunc.GetParametersCount();
        }

        return GetReturnType() < rhsFunc.GetReturnType();
    }

    return res;
}
