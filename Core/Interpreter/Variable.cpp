#include "Variable.h"

#include <iomanip>
#include <sstream>

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

Pointer<IVariable> IVariable::ApplyOperation(LexemeType operation) const {
    throw std::invalid_argument("Invalid operation");
}

Pointer<IVariable> ValueType::CastFrom(const Integer* val) const {
    throw std::invalid_argument("Invalid operation");
}

Pointer<IVariable> ValueType::CastFrom(const Double* val) const {
    throw std::invalid_argument("Invalid operation");
}

Pointer<IVariable> ValueType::CastFrom(const String* val) const {
    throw std::invalid_argument("Invalid operation");
}

Pointer<IVariable> ValueType::CastFrom(const Boolean* val) const {
    throw std::invalid_argument("Invalid operation");
}

Integer::Integer(int value) {
    SetValue(value);
}

Pointer<IVariable> Integer::Clone() const {
    return std::make_unique<Integer>(GetValue<int>());
}

Pointer<IVariable> Integer::Cast(const ValueType& resType) const {
    return resType.CastFrom(this);
}

Pointer<IVariable> Integer::CastFrom(const Double* val) const {
    return std::make_unique<Integer>(val->GetValue<double>());
}

Pointer<IVariable> Integer::CastFrom(const Boolean* val) const {
    return std::make_unique<Integer>(val->GetValue<bool>());
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
            return std::make_unique<Double>(std::fmod(GetValue<int>(), rhs->GetValue<double>()));
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
        return std::make_unique<Boolean>(rhs->Dereference<StructArray>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->Dereference<StructArray>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Integer::ApplyOperation(LexemeType operation, const Range* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->Dereference<StructRange>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->Dereference<StructRange>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Integer::ApplyOperation(LexemeType operation) const {
    if (operation == LexemeType::OpAdd) {
        return std::make_unique<Integer>(GetValue<int>());
    }
    if (operation == LexemeType::OpSub) {
        return std::make_unique<Integer>(-GetValue<int>());
    }

    return IVariable::ApplyOperation(operation);
}


Double::Double(double value) {
    SetValue(value);
}

Pointer<IVariable> Double::Clone() const {
    return std::make_unique<Double>(GetValue<double>());
}

Pointer<IVariable> Double::Cast(const ValueType& resType) const {
    return resType.CastFrom(this);
}

Pointer<IVariable> Double::CastFrom(const Integer* val) const {
    return std::make_unique<Double>(val->GetValue<int>());
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
            return std::make_unique<Double>(std::fmod(GetValue<double>(), rhs->GetValue<int>()));
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
            return std::make_unique<Double>(std::fmod(GetValue<double>(), rhs->GetValue<double>()));
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
        return std::make_unique<Boolean>(rhs->Dereference<StructArray>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->Dereference<StructArray>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Double::ApplyOperation(LexemeType operation, const Range* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->Dereference<StructRange>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->Dereference<StructRange>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Double::ApplyOperation(LexemeType operation) const {
    if (operation == LexemeType::OpAdd) {
        return std::make_unique<Double>(GetValue<double>());
    }
    if (operation == LexemeType::OpSub) {
        return std::make_unique<Double>(-GetValue<double>());
    }

    return IVariable::ApplyOperation(operation);
}

Boolean::Boolean(bool value) {
    SetValue(value);
}

Pointer<IVariable> Boolean::Clone() const {
    return std::make_unique<Boolean>(GetValue<bool>());
}

Pointer<IVariable> Boolean::Cast(const ValueType& resType) const {
    return resType.CastFrom(this);
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
            return std::make_unique<StructRange>(this, rhs);
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Boolean::ApplyOperation(LexemeType operation, const Range* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->Dereference<StructRange>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->Dereference<StructRange>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Boolean::ApplyOperation(LexemeType operation, const Array* rhs) const {
    if (operation == LexemeType::OpIn) {
        return std::make_unique<Boolean>(rhs->Dereference<StructArray>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->Dereference<StructArray>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Pointer<IVariable> Boolean::ApplyOperation(LexemeType operation) const {
    if (operation == LexemeType::OpExclMark) {
        return std::make_unique<Boolean>(!GetValue<bool>());
    }

    return IVariable::ApplyOperation(operation);
}

String::String(const std::string& value) {
    SetValue(value);
}

Pointer<IVariable> String::Cast(const ValueType& resType) const {
    return resType.CastFrom(this);
}

Pointer<IVariable> String::CastFrom(const Boolean* val) const {
    return std::make_unique<String>(val->GetValue<bool>() ? "true" : "false");
}

Pointer<IVariable> String::CastFrom(const Double* val) const {
    std::stringstream ss;
    double integral;
    if (std::modf(val->GetValue<double>(), &integral) == 0) {
        ss << std::fixed << std::setprecision(1) << val->GetValue<double>();
    } else {
        ss << val->GetValue<double>();
    }
    return std::make_unique<String>(ss.str());
}

Pointer<IVariable> String::CastFrom(const Integer* val) const {
    return std::make_unique<String>(std::to_string(val->GetValue<int>()));
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
        return std::make_unique<Boolean>(rhs->Dereference<StructArray>()->In(this));
    }
    if (operation == LexemeType::OpNotIn) {
        return std::make_unique<Boolean>(!rhs->Dereference<StructArray>()->In(this));
    }

    return IVariable::ApplyOperation(operation, rhs);
}

Reference::Reference(IVariable* src) {
    SetValue<IVariable*>(src);
}

Pointer<Reference> Reference::CloneRef() const {
    return std::make_unique<Reference>(GetValue<IVariable*>());
}

Pointer<IVariable> Reference::Clone() const {
    return CloneRef();
}

Pointer<Boolean> Reference::CheckStrictEquality(LexemeType operation, const IVariable* lhs) const {
    auto rhsRef = this->GetValue<IVariable*>();
    auto lhsRef = lhs->GetValue<IVariable*>();

    if (operation == LexemeType::OpStrictEq || operation == LexemeType::OpEqual) {
        return std::make_unique<Boolean>(rhsRef == lhsRef);
    }
    if (operation == LexemeType::OpStrictIneq || operation == LexemeType::OpInequal) {
        return std::make_unique<Boolean>(rhsRef != lhsRef);
    }
}

Pointer<IVariable> Reference::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    if (operation == LexemeType::OpStrictEq || operation == LexemeType::OpStrictIneq) {
        return CheckStrictEquality(operation, lhs);
    }
    return GetValue<IVariable*>()->ApplyOperation(operation, lhs);
}

Pointer<IVariable> Reference::ApplyOperation(LexemeType operation, const Integer* rhs) const {
    return GetValue<IVariable*>()->ApplyOperation(operation, rhs);
}
Pointer<IVariable> Reference::ApplyOperation(LexemeType operation, const Double* rhs) const {
    return GetValue<IVariable*>()->ApplyOperation(operation, rhs);
}
Pointer<IVariable> Reference::ApplyOperation(LexemeType operation, const Boolean* rhs) const {
    return GetValue<IVariable*>()->ApplyOperation(operation, rhs);
}
Pointer<IVariable> Reference::ApplyOperation(LexemeType operation, const String* rhs) const {
    return GetValue<IVariable*>()->ApplyOperation(operation, rhs);
}
Pointer<IVariable> Reference::ApplyOperation(LexemeType operation, const Array* rhs) const {
    return GetValue<IVariable*>()->ApplyOperation(operation, rhs);
}
Pointer<IVariable> Reference::ApplyOperation(LexemeType operation, const Range* rhs) const {
    return GetValue<IVariable*>()->ApplyOperation(operation, rhs);
}
Pointer<IVariable> Reference::ApplyOperation(LexemeType operation, const Class* rhs) const {
    return GetValue<IVariable*>()->ApplyOperation(operation, rhs);
}

Pointer<IVariable> Reference::ApplyOperation(LexemeType operation) const {
    if (operation == LexemeType::OpInc || operation == LexemeType::OpDec) {
        IVariable* val = GetValue<IVariable*>();
        if (dynamic_cast<Integer*>(val)) {
            val->SetValue(val->GetValue<int>() + (operation == LexemeType::OpInc ? 1 : -1));
            return val->Clone();
        }
        if (dynamic_cast<Double*>(val)) {
            val->SetValue(val->GetValue<double>() + (operation == LexemeType::OpInc ? 1 : -1));
            return val->Clone();
        }
    }

    return GetValue<IVariable*>()->ApplyOperation(operation);
}

StructArray::StructArray(const std::vector<IVariable*>& src) {
    for (auto it : src) {
        myVariables.push_back(it->Clone());
    }
}

Pointer<IVariable> StructArray::Clone() const {
    throw std::invalid_argument("Unsupported operation");
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

Pointer<Reference> StructArray::Get(int idx) const {
    return std::make_unique<Reference>(myVariables[idx].get());
}

int StructArray::Size() const {
    return myVariables.size();
}

bool StructArray::In(const IVariable* val) const {
    for (int i = 0; i < Size(); i++) {
        try {
            if (val->ApplyOperation(LexemeType::OpStrictEq, myVariables[i].get())->GetValue<bool>()) {
                return true;
            }
        } catch (const std::invalid_argument&) {}
    }

    return false;
}

IterableRef::IterableRef(IVariable* src) : Reference(src) {}

Array::Array(StructArray* elements) : IterableRef(elements) {}

Pointer<Reference> Array::CloneRef() const {
    return std::make_unique<Array>(Dereference<StructArray>());
}

Pointer<IVariable> Array::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    if (LexerUtils::IsEqualityOperation(operation)) {
        return CheckStrictEquality(operation, lhs);
    }
    return lhs->ApplyOperation(operation, this);
}

Pointer<IVariable> Array::GetIterator(int idx) const {
    return Dereference<StructArray>()->Get(idx);
}

int Array::Size() const {
    return Dereference<StructArray>()->Size();
}

StructRange::StructRange(const IVariable* left, const IVariable* right) {
    myLeft = left->Clone();
    myRight = right->Clone();
}

Pointer<IVariable> StructRange::Clone() const {
    throw std::invalid_argument("Unsupported operation");
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

Range::Range(StructRange* range) : IterableRef(range) {
    //SetValue(range);
}

Pointer<Reference> Range::CloneRef() const {
    return std::make_unique<Range>(Dereference<StructRange>());
}

Pointer<IVariable> Range::ApplyOperation(LexemeType operation, const IVariable* lhs) const {
    if (operation == LexemeType::OpStrictEq || operation == LexemeType::OpStrictIneq) {
        return CheckStrictEquality(operation, lhs);
    }
    return lhs->ApplyOperation(operation, this);
}

Pointer<IVariable> Range::ApplyOperation(LexemeType operation, const Range* rhs) const {
    switch (operation) {
        case LexemeType::OpEqual:
            return std::make_unique<Boolean>(*Dereference<StructRange>() == *rhs->Dereference<StructRange>());
        case LexemeType::OpInequal:
            return std::make_unique<Boolean>(*Dereference<StructRange>() != *rhs->Dereference<StructRange>());
    }

    return IVariable::ApplyOperation(operation, rhs);
}

const IVariable* Range::GetLeft() const {
    return Dereference<StructRange>()->GetLeft();
}

const IVariable* Range::GetRight() const {
    return Dereference<StructRange>()->GetRight();
}

Pointer<IVariable> Range::GetIterator(int idx) const {
    Integer offset(idx);
    return GetLeft()->ApplyOperation(LexemeType::OpAdd, &offset);
}

int Range::Size() const {
    Pointer<IVariable> res = GetLeft()->ApplyOperation(LexemeType::OpSub, GetRight());
    if (dynamic_cast<Double*>(res.get())) {
        return res->GetValue<double>() + 1;
    }
    return res->GetValue<int>() + 1;
}
