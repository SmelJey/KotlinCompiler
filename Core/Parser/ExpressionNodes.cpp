#include "ExpressionNodes.h"

#include "ISyntaxNode.h"
#include "StatementNodes.h"
#include "INodeVisitor.h"
#include "Semantics/FunctionSymbol.h"

BinOperationNode::BinOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> left,
                                   Pointer<IAnnotatedNode> right, const ITypeSymbol* type)
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

const ITypeSymbol* BinOperationNode::GetType() const {
    return myType;
}

std::string BinOperationNode::GetName() const {
    return "Bin op :: " + GetOperation();
}

void BinOperationNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(GetLeftOperand(), depth);
    visitor.VisitNode(GetRightOperand(), depth);
}

AbstractUnaryOperationNode::AbstractUnaryOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const ITypeSymbol* type)
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

const ITypeSymbol* AbstractUnaryOperationNode::GetType() const {
    return myType;
}

void AbstractUnaryOperationNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myOperand, depth);
}

UnaryPrefixOperationNode::UnaryPrefixOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const ITypeSymbol* type)
    : AbstractUnaryOperationNode(operation, std::move(operand), type) {}

std::string UnaryPrefixOperationNode::GetName() const {
    return "Prefix Op :: " + GetOperation();
}

UnaryPostfixOperationNode::UnaryPostfixOperationNode(const Lexeme& operation, Pointer<IAnnotatedNode> operand, const ITypeSymbol* type)
    : AbstractUnaryOperationNode(operation, std::move(operand), type) {}

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

std::string CallArgumentsNode::GetName() const {
    return "Args";
}

void CallArgumentsNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    for (auto& arg : myArguments) {
        visitor.VisitNode(*arg, depth);
    }
}

TypeArgumentsNode::TypeArgumentsNode(const Lexeme& lexeme) : AbstractNode(lexeme.CopyEmptyOfType(LexemeType::Ignored)) {}

const std::vector<Pointer<TypeNode>>& TypeArgumentsNode::GetArguments() const {
    return myArguments;
}

void TypeArgumentsNode::AddArgument(Pointer<TypeNode> argument) {
    myArguments.push_back(std::move(argument));
}

std::string TypeArgumentsNode::GetName() const {
    return "TypeArgs";
}

void TypeArgumentsNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    for (auto& arg : myArguments) {
        visitor.VisitNode(*arg, depth);
    }
}

PostfixCallNode::PostfixCallNode(Pointer<IAnnotatedNode> expression, Pointer<CallArgumentsNode> arguments, const ITypeSymbol* type)
    : AbstractUnaryPostfixNode(expression->GetLexeme().CopyEmptyOfType(LexemeType::Ignored)), myExpression(std::move(expression)),
        myArgumentsNode(std::move(arguments)), myType(type) {}

const CallArgumentsNode& PostfixCallNode::GetArguments() const {
    return *myArgumentsNode;
}

const IAnnotatedNode* PostfixCallNode::GetExpression() const {
    return myExpression.get();
}

const ISymbol* PostfixCallNode::GetSymbol() const {
    return myType;
}

const ITypeSymbol* PostfixCallNode::GetType() const {
    return myType;
}

void PostfixCallNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myArgumentsNode, depth);
}

IndexSuffixNode::IndexSuffixNode(Pointer<IAnnotatedNode> expression, Pointer<CallArgumentsNode> arguments, const ITypeSymbol* type)
    : PostfixCallNode(std::move(expression), std::move(arguments), type) {}

bool IndexSuffixNode::IsAssignable() const {
    return true;
}

std::string IndexSuffixNode::GetName() const {
    return "IndexSuffix";
}

CallSuffixNode::CallSuffixNode(Pointer<IAnnotatedNode> expression, Pointer<CallArgumentsNode> arguments, const ITypeSymbol* type)
    : PostfixCallNode(std::move(expression), std::move(arguments), type) {}

const TypeArgumentsNode& CallSuffixNode::GetTypeArguments() const {
    return *myTypeArguments;
}

void CallSuffixNode::SetTypeArguments(Pointer<TypeArgumentsNode> arguments) {
    myTypeArguments = std::move(arguments);
}

bool CallSuffixNode::HasTypeArguments() const {
    return myTypeArguments != nullptr;
}

void CallSuffixNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    PostfixCallNode::AcceptVisitor(visitor, depth);
    if (HasTypeArguments()) {
        visitor.VisitNode(*myTypeArguments, depth);
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

const ITypeSymbol* MemberAccessNode::GetType() const {
    auto varSym = dynamic_cast<const VariableSymbol*>(GetSymbol());
    if (varSym != nullptr) {
        return varSym->GetType();
    }
    auto funcSym = dynamic_cast<const FunctionSymbol*>(GetSymbol());
    if (funcSym != nullptr) {
        return funcSym->GetReturnType();
    }

    return nullptr;
}

bool MemberAccessNode::IsAssignable() const {
    auto varSym = dynamic_cast<const VariableSymbol*>(GetSymbol());
    if (varSym == nullptr) {
        return false;
    }

    return varSym->IsMutable();
}

std::string MemberAccessNode::GetName() const {
    return "MemberAccess :: " + GetOperation();
}

void MemberAccessNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myMemberNode, depth);
}

IfExpression::IfExpression(const Lexeme& lexeme, const ITypeSymbol* type, Pointer<IAnnotatedNode> expression)
    : AbstractNode(lexeme), myType(type), myExpression(std::move(expression)) {}

const IAnnotatedNode* IfExpression::GetExpression() const {
    return myExpression.get();
}

const ISyntaxNode* IfExpression::GetIfBody() const {
    return myIfBody.get();
}

void IfExpression::SetIfBody(Pointer<ISyntaxNode> body) {
    myIfBody = std::move(body);
}

bool IfExpression::HasIfBody() const {
    return dynamic_cast<EmptyStatement*>(myIfBody.get()) == nullptr;
}

const ISyntaxNode* IfExpression::GetElseBody() const {
    return myElseBody.get();
}

void IfExpression::SetElseBody(Pointer<ISyntaxNode> body) {
    myElseBody = std::move(body);
}

bool IfExpression::HasElseBody() const {
    return dynamic_cast<EmptyStatement*>(myElseBody.get()) == nullptr;
}

const ISymbol* IfExpression::GetSymbol() const {
    return myType;
}

const ITypeSymbol* IfExpression::GetType() const {
    return myType;
}

std::string IfExpression::GetName() const {
    return "If Expr";
}

void IfExpression::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    visitor.VisitNode(*myExpression, depth);
    visitor.VisitNode(*myIfBody, depth);
    visitor.VisitNode(*myElseBody, depth);
}

BlockNode::BlockNode(const Lexeme& lexeme) : AbstractNode(lexeme) {}

const std::vector<Pointer<ISyntaxNode>>& BlockNode::GetStatements() const {
    return myStatements;
}

void BlockNode::AddStatement(Pointer<ISyntaxNode> statement) {
    myStatements.push_back(std::move(statement));
}

const ISymbol* BlockNode::GetSymbol() const {
    return myReturn;
}

const ITypeSymbol* BlockNode::GetType() const {
    return myReturn;
}

void BlockNode::SetSymbol(const ITypeSymbol* returnSym) {
    myReturn = returnSym;
}

std::string BlockNode::GetName() const {
    return "Block";
}

void BlockNode::AcceptVisitor(INodeVisitor& visitor, int depth) const {
    for (auto& statement : myStatements) {
        visitor.VisitNode(*statement, depth);
    }
}
