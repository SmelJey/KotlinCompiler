#include "Variable.h"

Pointer<IVariable> IVariable::ApplyOperation(LexemeType operation, const Integer* rhs) const {
    throw std::invalid_argument("Invalid operation");
}

Pointer<IVariable> IVariable::ApplyOperation(LexemeType operation, const Double* rhs) const {
    throw std::invalid_argument("Invalid operation");
}

Pointer<IVariable> IVariable::ApplyOperation(LexemeType operation, const Boolean* rhs) const {
    throw std::invalid_argument("Invalid operation");
}

Pointer<IVariable> IVariable::ApplyOperation(LexemeType operation, const String* rhs) const {
    throw std::invalid_argument("Invalid operation");
}

Pointer<IVariable> IVariable::ApplyOperation(LexemeType operation, const Array* rhs) const {
    throw std::invalid_argument("Invalid operation");
}

Pointer<IVariable> IVariable::ApplyOperation(LexemeType operation, const Range* rhs) const {
    throw std::invalid_argument("Invalid operation");
}

Pointer<IVariable> IVariable::ApplyOperation(LexemeType operation, const Class* rhs) const {
    throw std::invalid_argument("Invalid operation");
}

Integer::Integer(int value) {
    SetValue(value);
}

Pointer<IVariable> Integer::Clone() const {
    return std::make_unique<Integer>(GetValue<int>());
}

Pointer<IVariable> Integer::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    return lhs->ApplyOperation(operation, this);
}
Pointer<IVariable> Integer::ApplyOperation(LexemeType operation, const Integer* rhs) const {
    switch (operation) {
        case LexemeType::OpAdd:
            return std::make_unique<Integer>(GetValue<int>() + rhs->GetValue<int>());
        case LexemeType::OpSub:
            return std::make_unique<Integer>(GetValue<int>() - rhs->GetValue<int>());
        case LexemeType::OpMult:
            return std::make_unique<Integer>(GetValue<int>() * rhs->GetValue<int>());
            // TODO: check for zero division
        case LexemeType::OpDiv:
            return std::make_unique<Integer>(GetValue<int>() / rhs->GetValue<int>());
        case LexemeType::OpMod:
            return std::make_unique<Integer>(GetValue<int>() % rhs->GetValue<int>());
        case LexemeType::OpEqual:
        case LexemeType::OpStrictEq:
            return std::make_unique<Boolean>(GetValue<int>() == rhs->GetValue<int>());
        case LexemeType::OpInequal:
        case LexemeType::OpStrictIneq:
            return std::make_unique<Boolean>(GetValue<int>() != rhs->GetValue<int>());
        case LexemeType::OpLess:
            return std::make_unique<Boolean>(GetValue<int>() < rhs->GetValue<int>());
        case LexemeType::OpLessOrEq:
            return std::make_unique<Boolean>(GetValue<int>() <= rhs->GetValue<int>());
        case LexemeType::OpGreater:
            return std::make_unique<Boolean>(GetValue<int>() > rhs->GetValue<int>());
        case LexemeType::OpGreaterOrEq:
            return std::make_unique<Boolean>(GetValue<int>() >= rhs->GetValue<int>());
        case LexemeType::OpDDot:
            return std::make_unique<StructRange>(this, rhs);
    }

    return IVariable::ApplyOperation(operation, rhs);
}
Pointer<IVariable> Integer::ApplyOperation(LexemeType operation, const Double* rhs) const {
    switch (operation) {
        case LexemeType::OpAdd:
            return std::make_unique<Double>(GetValue<int>() + rhs->GetValue<double>());
        case LexemeType::OpSub:
            return std::make_unique<Double>(GetValue<int>() - rhs->GetValue<double>());
        case LexemeType::OpMult:
            return std::make_unique<Double>(GetValue<int>() * rhs->GetValue<double>());
        case LexemeType::OpDiv:
            return std::make_unique<Double>(GetValue<int>() / rhs->GetValue<double>());
        case LexemeType::OpMod:
            // TODO: make it work
            throw;
            //return std::make_unique<Double>(GetValue<int>() % rhs->GetValue<double>());
        case LexemeType::OpLess:
            return std::make_unique<Boolean>(GetValue<int>() < rhs->GetValue<double>());
        case LexemeType::OpLessOrEq:
            return std::make_unique<Boolean>(GetValue<int>() <= rhs->GetValue<double>());
        case LexemeType::OpGreater:
            return std::make_unique<Boolean>(GetValue<int>() > rhs->GetValue<double>());
        case LexemeType::OpGreaterOrEq:
            return std::make_unique<Boolean>(GetValue<int>() >= rhs->GetValue<double>());
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Integer::ApplyOperation(LexemeType operation, const Array* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->GetValue<const StructArray*>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->GetValue<const StructArray*>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Integer::ApplyOperation(LexemeType operation, const Range* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->GetValue<const StructRange*>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->GetValue<const StructRange*>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}


Double::Double(double value) {
    SetValue(value);
}

Pointer<IVariable> Double::Clone() const {
    return std::make_unique<Double>(GetValue<double>());
}

Pointer<IVariable> Double::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    return lhs->ApplyOperation(operation, this);
}

Pointer<IVariable> Double::ApplyOperation(LexemeType operation, const Integer* rhs) const {
    switch (operation) {
        case LexemeType::OpAdd:
            return std::make_unique<Double>(GetValue<double>() + rhs->GetValue<int>());
        case LexemeType::OpSub:
            return std::make_unique<Double>(GetValue<double>() - rhs->GetValue<int>());
        case LexemeType::OpMult:
            return std::make_unique<Double>(GetValue<double>() * rhs->GetValue<int>());
        case LexemeType::OpDiv:
            return std::make_unique<Double>(GetValue<double>() / rhs->GetValue<int>());
        case LexemeType::OpMod:
            // TODO: make it work
            throw;
            //return std::make_unique<Double>(GetValue<double>() % rhs->GetValue<int>());
        case LexemeType::OpLess:
            return std::make_unique<Boolean>(GetValue<double>() < rhs->GetValue<int>());
        case LexemeType::OpLessOrEq:
            return std::make_unique<Boolean>(GetValue<double>() <= rhs->GetValue<int>());
        case LexemeType::OpGreater:
            return std::make_unique<Boolean>(GetValue<double>() > rhs->GetValue<int>());
        case LexemeType::OpGreaterOrEq:
            return std::make_unique<Boolean>(GetValue<double>() >= rhs->GetValue<int>());
    }

    return IVariable::ApplyOperation(operation, rhs);
}
Pointer<IVariable> Double::ApplyOperation(LexemeType operation, const Double* rhs) const {
    switch (operation) {
        case LexemeType::OpAdd:
            return std::make_unique<Double>(GetValue<double>() + rhs->GetValue<double>());
        case LexemeType::OpSub:
            return std::make_unique<Double>(GetValue<double>() - rhs->GetValue<double>());
        case LexemeType::OpMult:
            return std::make_unique<Double>(GetValue<double>() * rhs->GetValue<double>());
        case LexemeType::OpDiv:
            return std::make_unique<Double>(GetValue<double>() / rhs->GetValue<double>());
        case LexemeType::OpMod:
            // TODO: make it work
            //return std::make_unique<Double>(GetValue<double>() % rhs->GetValue<double>());
            throw;
        case LexemeType::OpEqual:
        case LexemeType::OpStrictEq:
            return std::make_unique<Boolean>(GetValue<double>() == rhs->GetValue<double>());
        case LexemeType::OpInequal:
        case LexemeType::OpStrictIneq:
            return std::make_unique<Boolean>(GetValue<double>() != rhs->GetValue<double>());
        case LexemeType::OpLess:
            return std::make_unique<Boolean>(GetValue<double>() < rhs->GetValue<double>());
        case LexemeType::OpLessOrEq:
            return std::make_unique<Boolean>(GetValue<double>() <= rhs->GetValue<double>());
        case LexemeType::OpGreater:
            return std::make_unique<Boolean>(GetValue<double>() > rhs->GetValue<double>());
        case LexemeType::OpGreaterOrEq:
            return std::make_unique<Boolean>(GetValue<double>() >= rhs->GetValue<double>());
        case LexemeType::OpDDot:
            return std::make_unique<StructRange>(this, rhs);
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Double::ApplyOperation(LexemeType operation, const Array* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->GetValue<const StructArray*>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->GetValue<const StructArray*>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Double::ApplyOperation(LexemeType operation, const Range* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->GetValue<const StructRange*>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->GetValue<const StructRange*>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Boolean::Boolean(bool value) {
    SetValue(value);
}

Pointer<IVariable> Boolean::Clone() const {
    return std::make_unique<Boolean>(GetValue<bool>());
}

Pointer<IVariable> Boolean::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    return lhs->ApplyOperation(operation, this);
}

Pointer<IVariable> Boolean::ApplyOperation(LexemeType operation, const Boolean* rhs) const {
    switch (operation) {
        case LexemeType::OpAnd:
            return std::make_unique<Boolean>(GetValue<bool>() && rhs->GetValue<bool>());
        case LexemeType::OpOr:
            return std::make_unique<Boolean>(GetValue<bool>() || rhs->GetValue<bool>());
        case LexemeType::OpEqual:
        case LexemeType::OpStrictEq:
            return std::make_unique<Boolean>(GetValue<bool>() == rhs->GetValue<bool>());
        case LexemeType::OpInequal:
        case LexemeType::OpStrictIneq:
            return std::make_unique<Boolean>(GetValue<bool>() != rhs->GetValue<bool>());
        case LexemeType::OpLess:
            return std::make_unique<Boolean>(GetValue<bool>() < rhs->GetValue<bool>());
        case LexemeType::OpLessOrEq:
            return std::make_unique<Boolean>(GetValue<bool>() <= rhs->GetValue<bool>());
        case LexemeType::OpGreater:
            return std::make_unique<Boolean>(GetValue<bool>() > rhs->GetValue<bool>());
        case LexemeType::OpGreaterOrEq:
            return std::make_unique<Boolean>(GetValue<bool>() >= rhs->GetValue<bool>());
        case LexemeType::OpDDot:
            // TODO: make it work
            throw;
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Boolean::ApplyOperation(LexemeType operation, const Range* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->GetValue<const StructRange*>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->GetValue<const StructRange*>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Boolean::ApplyOperation(LexemeType operation, const Array* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->GetValue<const StructArray*>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->GetValue<const StructArray*>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

String::String(const std::string& value) {
    SetValue(value);
}

Pointer<IVariable> String::Clone() const {
    return std::make_unique<String>(GetValue<std::string>());
}

Pointer<IVariable> String::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    return lhs->ApplyOperation(operation, this);
}

Pointer<IVariable> String::ApplyOperation(LexemeType operation, const String* rhs) const {
    switch (operation) {
        case LexemeType::OpAdd:
            return std::make_unique<String>(GetValue<std::string>() + rhs->GetValue<std::string>());
        case LexemeType::OpEqual:
        case LexemeType::OpStrictEq:
            return std::make_unique<Boolean>(GetValue<std::string>() == rhs->GetValue<std::string>());
        case LexemeType::OpInequal:
        case LexemeType::OpStrictIneq:
            return std::make_unique<Boolean>(GetValue<std::string>() != rhs->GetValue<std::string>());
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> String::ApplyOperation(LexemeType operation, const Array* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->GetValue<const StructArray*>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->GetValue<const StructArray*>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

StructArray::StructArray(const std::vector<const IVariable*>& src) {
    for (auto it : src) {
        myVariables.push_back(it->Clone());
    }
}

Pointer<IVariable> StructArray::Clone() const {
    // TODO: fix it
    throw;
}

Pointer<IVariable> StructArray::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    return lhs->ApplyOperation(operation, this);
}

std::vector<const IVariable*> StructArray::Get() const {
    std::vector<const IVariable*> res;
    for (auto& it : myVariables) {
        res.push_back(it.get());
    }
    return res;
}

const IVariable* StructArray::Get(int idx) const {
    return myVariables[idx].get();
}

int StructArray::Size() const {
    return myVariables.size();
}

bool StructArray::In(const IVariable* val) const {
    for (int i = 0; i < Size(); i++) {
        try {
            if (val->ApplyOperation(LexemeType::OpStrictEq, val)) {
                return true;
            }
        } catch (std::invalid_argument) {}
    }

    return false;
}

Array::Array(const StructArray* elements) {
    SetValue(elements);
}

Pointer<IVariable> Array::Clone() const {
    return std::make_unique<Array>(GetValue<const StructArray*>());
}

Pointer<IVariable> Array::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    return lhs->ApplyOperation(operation, this);
}

Pointer<IVariable> Array::ApplyOperation(LexemeType operation, const Array* rhs) const {
    switch (operation) {
        case LexemeType::OpEqual:
        case LexemeType::OpStrictEq:
            return std::make_unique<Boolean>(GetValue<const StructArray*>() == rhs->GetValue<const StructArray*>());
        case LexemeType::OpInequal:
        case LexemeType::OpStrictIneq:
            return std::make_unique<Boolean>(GetValue<const StructArray*>() != rhs->GetValue<const StructArray*>());
    }

    return IVariable::ApplyOperation(operation, rhs);
}

StructRange::StructRange(const IVariable* left, const IVariable* right) {
    myLeft = left->Clone();
    myRight = right->Clone();
}

Pointer<IVariable> StructRange::Clone() const {
    // TODO: fix it
    throw;
}

Pointer<IVariable> StructRange::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    return lhs->ApplyOperation(operation, this);
}

const IVariable* StructRange::GetLeft() const {
    return myLeft.get();
}

const IVariable* StructRange::GetRight() const {
    return myRight.get();
}

bool StructRange::In(const IVariable* val) const {
    return myLeft->ApplyOperation(LexemeType::OpGreaterOrEq, val)->GetValue<bool>()
        && myRight->ApplyOperation(LexemeType::OpLessOrEq, val)->GetValue<bool>();
}

bool operator==(const StructRange& lhs, const StructRange& rhs) {
    return lhs.GetLeft()->ApplyOperation(LexemeType::OpEqual, rhs.GetLeft())->GetValue<bool>()
        && lhs.GetRight()->ApplyOperation(LexemeType::OpEqual, rhs.GetRight())->GetValue<bool>();
}

bool operator!=(const StructRange& lhs, const StructRange& rhs) {
    return !(lhs == rhs);
}

Range::Range(const StructRange* range) {
    SetValue(range);
}

Pointer<IVariable> Range::Clone() const {
    return std::make_unique<Range>(GetValue<const StructRange*>());
}

Pointer<IVariable> Range::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    return lhs->ApplyOperation(operation, this);
}

Pointer<IVariable> Range::ApplyOperation(LexemeType operation, const Range* rhs) const {
    switch (operation) {
        case LexemeType::OpEqual:
            return std::make_unique<Boolean>(*GetValue<const StructRange*>() == *rhs->GetValue<const StructRange*>());
        case LexemeType::OpStrictEq:
            return std::make_unique<Boolean>(GetValue<const StructRange*>() == rhs->GetValue<const StructRange*>());
        case LexemeType::OpInequal:
            return std::make_unique<Boolean>(*GetValue<const StructRange*>() != *rhs->GetValue<const StructRange*>());
        case LexemeType::OpStrictIneq:
            return std::make_unique<Boolean>(GetValue<const StructRange*>() != rhs->GetValue<const StructRange*>());
    }

    return IVariable::ApplyOperation(operation, rhs);
}

