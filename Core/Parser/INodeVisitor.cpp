#include "INodeVisitor.h"

#include "ISyntaxNode.h"

INodeVisitor::~INodeVisitor() = default;
void INodeVisitor::EnterNode(const IVisitable& node) {
    node.AcceptVisitor(*this);
}

void INodeVisitor::EnterNode(const IdentifierNode& node) {}
void INodeVisitor::EnterNode(const IntegerNode& node) {}
void INodeVisitor::EnterNode(const DoubleNode& node) {}
void INodeVisitor::EnterNode(const BooleanNode& node) {}
void INodeVisitor::EnterNode(const StringNode& node) {}
void INodeVisitor::EnterNode(const TypeNode& node) {}
void INodeVisitor::EnterNode(const BreakNode& node) {}
void INodeVisitor::EnterNode(const ContinueNode& node) {}
void INodeVisitor::EnterNode(const ReturnNode& node) {}
void INodeVisitor::EnterNode(const EmptyStatement& node) {}
void INodeVisitor::EnterNode(const Assignment& node) {}
void INodeVisitor::EnterNode(const WhileNode& node) {}
void INodeVisitor::EnterNode(const DoWhileNode& node) {}
void INodeVisitor::EnterNode(const ForNode& node) {}
void INodeVisitor::EnterNode(const BinOperationNode& node) {}
void INodeVisitor::EnterNode(const UnaryPrefixOperationNode& node) {}
void INodeVisitor::EnterNode(const UnaryPostfixOperationNode& node) {}
void INodeVisitor::EnterNode(const CallArgumentsNode& node) {}
void INodeVisitor::EnterNode(const TypeArgumentsNode& node) {}
void INodeVisitor::EnterNode(const IndexSuffixNode& node) {}
void INodeVisitor::EnterNode(const CallSuffixNode& node) {}
void INodeVisitor::EnterNode(const MemberAccessNode& node) {}
void INodeVisitor::EnterNode(const IfExpression& node) {}
void INodeVisitor::EnterNode(const BlockNode& node) {}
void INodeVisitor::EnterNode(const DeclarationBlock& node) {}
void INodeVisitor::EnterNode(const ClassDeclaration& node) {}
void INodeVisitor::EnterNode(const ParameterNode& node) {}
void INodeVisitor::EnterNode(const VariableNode& node) {}
void INodeVisitor::EnterNode(const ParameterList& node) {}
void INodeVisitor::EnterNode(const FunctionDeclaration& node) {}
void INodeVisitor::EnterNode(const PropertyDeclaration& node) {}

void INodeVisitor::ExitNode(const IVisitable& node) {}
void INodeVisitor::ExitNode(const IdentifierNode& node) {}
void INodeVisitor::ExitNode(const IntegerNode& node) {}
void INodeVisitor::ExitNode(const DoubleNode& node) {}
void INodeVisitor::ExitNode(const BooleanNode& node) {}
void INodeVisitor::ExitNode(const StringNode& node) {}
void INodeVisitor::ExitNode(const TypeNode& node) {}
void INodeVisitor::ExitNode(const BreakNode& node) {}
void INodeVisitor::ExitNode(const ContinueNode& node) {}
void INodeVisitor::ExitNode(const ReturnNode& node) {}
void INodeVisitor::ExitNode(const EmptyStatement& node) {}
void INodeVisitor::ExitNode(const Assignment& node) {}
void INodeVisitor::ExitNode(const WhileNode& node) {}
void INodeVisitor::ExitNode(const DoWhileNode& node) {}
void INodeVisitor::ExitNode(const ForNode& node) {}
void INodeVisitor::ExitNode(const BinOperationNode& node) {}
void INodeVisitor::ExitNode(const UnaryPrefixOperationNode& node) {}
void INodeVisitor::ExitNode(const UnaryPostfixOperationNode& node) {}
void INodeVisitor::ExitNode(const CallArgumentsNode& node) {}
void INodeVisitor::ExitNode(const TypeArgumentsNode& node) {}
void INodeVisitor::ExitNode(const IndexSuffixNode& node) {}
void INodeVisitor::ExitNode(const CallSuffixNode& node) {}
void INodeVisitor::ExitNode(const MemberAccessNode& node) {}
void INodeVisitor::ExitNode(const IfExpression& node) {}
void INodeVisitor::ExitNode(const BlockNode& node) {}
void INodeVisitor::ExitNode(const DeclarationBlock& node) {}
void INodeVisitor::ExitNode(const ClassDeclaration& node) {}
void INodeVisitor::ExitNode(const ParameterNode& node) {}
void INodeVisitor::ExitNode(const VariableNode& node) {}
void INodeVisitor::ExitNode(const ParameterList& node) {}
void INodeVisitor::ExitNode(const FunctionDeclaration& node) {}
void INodeVisitor::ExitNode(const PropertyDeclaration& node) {}
