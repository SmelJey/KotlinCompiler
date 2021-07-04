#include "FunctionSymbol.h"

#include "../INodeVisitor.h"

FunctionSymbol::FunctionSymbol(const std::string& name, const AbstractType* returnType,
    const std::vector<const AbstractType*>& params, Pointer<SymbolTable> table, const AbstractDeclaration* decl)
    : myName(name), myReturnType(returnType), myParameters(params), myTable(std::move(table)), myDeclaration(decl) {}

std::string FunctionSymbol::GetName() const {
    return myName;
}

const AbstractType* FunctionSymbol::GetReturnType() const {
    return myReturnType;
}

int FunctionSymbol::GetParametersCount() const {
    return myParameters.size();
}

const AbstractType& FunctionSymbol::GetParameter(int idx) const {
    return *myParameters[idx];
}

bool FunctionSymbol::CheckArgument(const AbstractType& type, int idx) {
    if (idx >= myParameters.size()) {
        return false;
    }

    return type == *myParameters[idx];
}

const AbstractDeclaration* FunctionSymbol::GetDeclaration() const {
    return myDeclaration;
}

bool FunctionSymbol::operator<(const ISymbol& rhs) const {
    bool res = ISymbol::operator<(rhs);
    if (!res) {
        const auto& rhsFunc = dynamic_cast<const FunctionSymbol&>(rhs);

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

    return res;
}

std::string FunctionSymbol::ToString() const {
    std::string res = "Fun " + GetName() + "(";
    for (auto param : myParameters) {
        res += param->GetName();
        res += ", ";
    }
    if (myParameters.size() > 0) {
        res.pop_back();
        res.pop_back();
    }
    res += ") -> " + GetReturnType()->GetName();
    return res;
}

void FunctionSymbol::AcceptVisitor(INodeVisitor& visitor) const {
    if (!myTable->IsEmpty()) {
        myTable->RunVisitor(visitor);
    }
}
