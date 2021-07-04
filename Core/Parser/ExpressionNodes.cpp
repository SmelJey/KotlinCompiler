#include "ExpressionNodes.h"

#include "ISyntaxNode.h"
#include "StatementNodes.h"
#include "INodeVisitor.h"
#include "Semantics/FunctionSymbol.h"

BinOperationNode::BinOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> left,
                                   Pointer<IAnnotatedNode> right, const AbstractType* type)
    : AbstractNode(operation), myLeftOperand(std::move(left)), myRightOperand(std::move(right)), myType(type) {}

const IAnnotatedNode& BinOperationNode::GetLeftOperand() const {
    return *myLeftOperand;
}

const IAnnotatedNode& BinOperationNode::GetRightOperand() const {
    return *myRightOperand;
}

std::string BinOperationNode::GetOperation() const {
    return GetLexeme().GetValue<std::string>();
}

const ISymbol* BinOperationNode::GetSymbol() const {
    return myType;
}

const AbstractType* BinOperationNode::GetType() const {
    return myType;
}

void BinOperationNode::RunVisitor(INodeVisitor& visitor) const {
    IVisitable::RunVisitor(visitor);
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

std::string BinOperationNode::GetName() const {
    return "Bin op :: " + GetOperation();
}

void BinOperationNode::AcceptVisitor(INodeVisitor& visitor) const {
    GetLeftOperand().RunVisitor(visitor);
    GetRightOperand().RunVisitor(visitor);
}

AbstractUnaryOperationNode::AbstractUnaryOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const AbstractType* type)
    : AbstractNode(operation), myOperand(std::move(operand)), myType(type) {}

const IAnnotatedNode& AbstractUnaryOperationNode::GetOperand() const {
    return *myOperand;
}

std::string AbstractUnaryOperationNode::GetOperation() const {
    return GetLexeme().GetValue<std::string>();
}

const ISymbol* AbstractUnaryOperationNode::GetSymbol() const {
    return myType;
}

const AbstractType* AbstractUnaryOperationNode::GetType() const {
    return myType;
}

void AbstractUnaryOperationNode::AcceptVisitor(INodeVisitor& visitor) const {
    myOperand->RunVisitor(visitor);
}

UnaryPrefixOperationNode::UnaryPrefixOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const AbstractType* type)
    : AbstractUnaryOperationNode(operation, std::move(operand), type) {}

void UnaryPrefixOperationNode::RunVisitor(INodeVisitor& visitor) const {
    IVisitable::RunVisitor(visitor);
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

std::string UnaryPrefixOperationNode::GetName() const {
    return "Prefix Op :: " + GetOperation();
}

UnaryPostfixOperationNode::UnaryPostfixOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const AbstractType* type)
    : AbstractUnaryOperationNode(operation, std::move(operand), type) {}

void UnaryPostfixOperationNode::RunVisitor(INodeVisitor& visitor) const {
    IVisitable::RunVisitor(visitor);
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

std::string UnaryPostfixOperationNode::GetName() const {
    return "Postfix Op :: " + GetOperation();
}

AbstractUnaryPostfixNode::AbstractUnaryPostfixNode(const Lexeme& lexeme) : AbstractNode(lexeme) {}

CallArgumentsNode::CallArgumentsNode(const Lexeme& lexeme) : AbstractNode(lexeme.CopyEmptyOfType(LexemeType::Ignored)) {}

const std::vector<Pointer<IAnnotatedNode>>& CallArgumentsNode::GetArguments() const {
    return myArguments;
}

void CallArgumentsNode::AddArgument(Pointer<IAnnotatedNode> argument) {
    myArguments.push_back(std::move(argument));
}

const std::vector<const AbstractType*> CallArgumentsNode::GetTypes() const {
    std::vector<const AbstractType*> types;
    for (auto& arg : myArguments) {
        types.push_back(arg->GetType());
    }

    return types;
}

void CallArgumentsNode::RunVisitor(INodeVisitor& visitor) const {
    IVisitable::RunVisitor(visitor);
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

std::string CallArgumentsNode::GetName() const {
    return "Args";
}

void CallArgumentsNode::AcceptVisitor(INodeVisitor& visitor) const {
    for (auto& arg : myArguments) {
        arg->RunVisitor(visitor);
    }
}

TypeArgumentsNode::TypeArgumentsNode(const Lexeme& lexeme) : AbstractNode(lexeme.CopyEmptyOfType(LexemeType::Ignored)) {}

const std::vector<Pointer<TypeNode>>& TypeArgumentsNode::GetArguments() const {
    return myArguments;
}

void TypeArgumentsNode::AddArgument(Pointer<TypeNode> argument) {
    myArguments.push_back(std::move(argument));
}

const std::vector<const AbstractType*> TypeArgumentsNode::GetTypes() const {
    std::vector<const AbstractType*> types;
    for (auto& arg : myArguments) {
        types.push_back(arg->GetType());
    }

    return types;
}

void TypeArgumentsNode::RunVisitor(INodeVisitor& visitor) const {
    IVisitable::RunVisitor(visitor);
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

std::string TypeArgumentsNode::GetName() const {
    return "TypeArgs";
}

void TypeArgumentsNode::AcceptVisitor(INodeVisitor& visitor) const {
    for (auto& arg : myArguments) {
        arg->RunVisitor(visitor);
    }
}

AbstractPostfixCallNode::AbstractPostfixCallNode(Pointer<IAnnotatedNode> expression, Pointer<CallArgumentsNode> arguments, const AbstractType* type)
    : AbstractUnaryPostfixNode(expression->GetLexeme().CopyEmptyOfType(LexemeType::Ignored)), myExpression(std::move(expression)),
        myArgumentsNode(std::move(arguments)), myType(type) {}

const CallArgumentsNode& AbstractPostfixCallNode::GetArguments() const {
    return *myArgumentsNode;
}

const IAnnotatedNode* AbstractPostfixCallNode::GetExpression() const {
    return myExpression.get();
}

const ISymbol* AbstractPostfixCallNode::GetSymbol() const {
    return myType;
}

const AbstractType* AbstractPostfixCallNode::GetType() const {
    return myType;
}

void AbstractPostfixCallNode::AcceptVisitor(INodeVisitor& visitor) const {
    myExpression->RunVisitor(visitor);
    myArgumentsNode->RunVisitor(visitor);
}

IndexSuffixNode::IndexSuffixNode(Pointer<IAnnotatedNode> expression, Pointer<CallArgumentsNode> arguments, const AbstractType* type)
    : AbstractPostfixCallNode(std::move(expression), std::move(arguments), type) {}

bool IndexSuffixNode::IsAssignable() const {
    return true;
}

void IndexSuffixNode::RunVisitor(INodeVisitor& visitor) const {
    IVisitable::RunVisitor(visitor);
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

std::string IndexSuffixNode::GetName() const {
    return "IndexSuffix";
}

CallSuffixNode::CallSuffixNode(Pointer<IAnnotatedNode> expression, Pointer<CallArgumentsNode> arguments, const AbstractType* type)
    : AbstractPostfixCallNode(std::move(expression), std::move(arguments), type) {}

const TypeArgumentsNode& CallSuffixNode::GetTypeArguments() const {
    return *myTypeArguments;
}

void CallSuffixNode::SetTypeArguments(Pointer<TypeArgumentsNode> arguments) {
    myTypeArguments = std::move(arguments);
}

bool CallSuffixNode::HasTypeArguments() const {
    return myTypeArguments != nullptr;
}

void CallSuffixNode::RunVisitor(INodeVisitor& visitor) const {
    IVisitable::RunVisitor(visitor);
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

void CallSuffixNode::AcceptVisitor(INodeVisitor& visitor) const {
    AbstractPostfixCallNode::AcceptVisitor(visitor);
    if (HasTypeArguments()) {
        myTypeArguments->RunVisitor(visitor);
    }
}

std::string CallSuffixNode::GetName() const {
    return "CallSuffix";
}

MemberAccessNode::MemberAccessNode(const Lexeme& lexeme, Pointer<IAnnotatedNode> expression, Pointer<IAnnotatedNode> member)
    : AbstractUnaryPostfixNode(lexeme), myExpression(std::move(expression)), myMemberNode(std::move(member)) {}

std::string MemberAccessNode::GetOperation() const {
    return GetLexeme().GetValue<std::string>();
}

const IAnnotatedNode* MemberAccessNode::GetExpression() const {
    return myExpression.get();
}

IAnnotatedNode* MemberAccessNode::GetMember() const {
    return myMemberNode.get();
}

const ISymbol* MemberAccessNode::GetSymbol() const {
    return GetMember()->GetSymbol();
}

const AbstractType* MemberAccessNode::GetType() const {
    auto varSym = dynamic_cast<const VariableSymbol*>(GetSymbol());
    if (varSym != nullptr) {
        return varSym->GetType();
    }
    auto funcSym = dynamic_cast<const FunctionSymbol*>(GetSymbol());
    if (funcSym != nullptr) {
        return funcSym->GetReturnType();
    }

    return dynamic_cast<const UnresolvedSymbol*>(GetSymbol());;
}

bool MemberAccessNode::IsAssignable() const {
    auto varSym = dynamic_cast<const VariableSymbol*>(GetSymbol());
    if (varSym == nullptr) {
        return false;
    }

    return varSym->IsMutable();
}

void MemberAccessNode::RunVisitor(INodeVisitor& visitor) const {
    IVisitable::RunVisitor(visitor);
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

std::string MemberAccessNode::GetName() const {
    return "MemberAccess :: " + GetOperation();
}

void MemberAccessNode::AcceptVisitor(INodeVisitor& visitor) const {
    myExpression->RunVisitor(visitor);
    myMemberNode->RunVisitor(visitor);
}

IfExpression::IfExpression(const Lexeme& lexeme, const UnitTypeSymbol* type, Pointer<IAnnotatedNode> expression, Pointer<IAnnotatedNode> ifBody, Pointer<IAnnotatedNode> elseBody)
    : UnitTypedNode(lexeme, type), myExpression(std::move(expression)), myIfBody(std::move(ifBody)), myElseBody(std::move(elseBody)) {}

const IAnnotatedNode* IfExpression::GetExpression() const {
    return myExpression.get();
}

const IAnnotatedNode* IfExpression::GetIfBody() const {
    return myIfBody.get();
}

const IAnnotatedNode* IfExpression::GetElseBody() const {
    return myElseBody.get();
}

const ISymbol* IfExpression::GetSymbol() const {
    return GetType();
}

const AbstractType* IfExpression::GetType() const {
    if (*myIfBody->GetType() == *myElseBody->GetType()) {
        return myIfBody->GetType();
    }

    return UnitTypedNode::GetType();
}

void IfExpression::RunVisitor(INodeVisitor& visitor) const {
    IVisitable::RunVisitor(visitor);
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

std::string IfExpression::GetName() const {
    return "If Expr";
}

void IfExpression::AcceptVisitor(INodeVisitor& visitor) const {
    myExpression->RunVisitor(visitor);
    myIfBody->RunVisitor(visitor);
    myElseBody->RunVisitor(visitor);
}

BlockNode::BlockNode(const Lexeme& lexeme, const UnitTypeSymbol* type) : AbstractNode(lexeme), myReturn(type) {}

const std::vector<Pointer<IAnnotatedNode>>& BlockNode::GetStatements() const {
    return myStatements;
}

void BlockNode::AddStatement(Pointer<IAnnotatedNode> statement) {
    myReturn = statement->GetType();
    myStatements.push_back(std::move(statement));
}

const ISymbol* BlockNode::GetSymbol() const {
    return myReturn;
}

const AbstractType* BlockNode::GetType() const {
    return myReturn;
}

void BlockNode::SetSymbol(const AbstractType* returnSym) {
    myReturn = returnSym;
}

void BlockNode::RunVisitor(INodeVisitor& visitor) const {
    IVisitable::RunVisitor(visitor);
    visitor.EnterNode(*this);
    visitor.ExitNode(*this);
}

std::string BlockNode::GetName() const {
    return "Block";
}

void BlockNode::AcceptVisitor(INodeVisitor& visitor) const {
    for (auto& statement : myStatements) {
        statement->RunVisitor(visitor);
    }
}
