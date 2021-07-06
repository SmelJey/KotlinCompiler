#pragma once

class PropertyDeclaration;
class FunctionDeclaration;
class ParameterList;
class VariableNode;
class ParameterNode;
class ClassDeclaration;
class DeclarationBlock;
class BlockNode;
class IfExpression;
class MemberAccessNode;
class CallSuffixNode;
class IndexSuffixNode;
class TypeArgumentsNode;
class CallArgumentsNode;
class UnaryPostfixOperationNode;
class UnaryPrefixOperationNode;
class BinOperationNode;
class ForNode;
class DoWhileNode;
class WhileNode;
class Assignment;
class EmptyStatement;
class BreakNode;
class ContinueNode;
class ReturnNode;
class StringNode;
class BooleanNode;
class DoubleNode;
class IntegerNode;
class IdentifierNode;
class TypeNode;
class ClassSymbol;
class IVisitable;

class INodeVisitor {
public:
    virtual ~INodeVisitor();

    virtual void EnterNode(const IVisitable& node);

    virtual void EnterNode(const IdentifierNode& node);
    virtual void EnterNode(const IntegerNode& node);
    virtual void EnterNode(const DoubleNode& node);
    virtual void EnterNode(const BooleanNode& node);
    virtual void EnterNode(const StringNode& node);
    virtual void EnterNode(const TypeNode& node);
    virtual void EnterNode(const BreakNode& node);
    virtual void EnterNode(const ContinueNode& node);
    virtual void EnterNode(const ReturnNode& node);

    virtual void EnterNode(const EmptyStatement& node);
    virtual void EnterNode(const Assignment& node);
    virtual void EnterNode(const WhileNode& node);
    virtual void EnterNode(const DoWhileNode& node);
    virtual void EnterNode(const ForNode& node);

    virtual void EnterNode(const BinOperationNode& node);
    virtual void EnterNode(const UnaryPrefixOperationNode& node);
    virtual void EnterNode(const UnaryPostfixOperationNode& node);
    virtual void EnterNode(const CallArgumentsNode& node);
    virtual void EnterNode(const TypeArgumentsNode& node);
    virtual void EnterNode(const IndexSuffixNode& node);
    virtual void EnterNode(const CallSuffixNode& node);
    virtual void EnterNode(const MemberAccessNode& node);
    virtual void EnterNode(const IfExpression& node);
    virtual void EnterNode(const BlockNode& node);

    virtual void EnterNode(const DeclarationBlock& node);
    virtual void EnterNode(const ClassDeclaration& node);
    virtual void EnterNode(const ParameterNode& node);
    virtual void EnterNode(const VariableNode& node);
    virtual void EnterNode(const ParameterList& node);
    virtual void EnterNode(const FunctionDeclaration& node);
    virtual void EnterNode(const PropertyDeclaration& node);

    virtual void ExitNode(const IVisitable& node);

    virtual void ExitNode(const IdentifierNode& node);
    virtual void ExitNode(const IntegerNode& node);
    virtual void ExitNode(const DoubleNode& node);
    virtual void ExitNode(const BooleanNode& node);
    virtual void ExitNode(const StringNode& node);
    virtual void ExitNode(const TypeNode& node);
    virtual void ExitNode(const BreakNode& node);
    virtual void ExitNode(const ContinueNode& node);
    virtual void ExitNode(const ReturnNode& node);

    virtual void ExitNode(const EmptyStatement& node);
    virtual void ExitNode(const Assignment& node);
    virtual void ExitNode(const WhileNode& node);
    virtual void ExitNode(const DoWhileNode& node);
    virtual void ExitNode(const ForNode& node);

    virtual void ExitNode(const BinOperationNode& node);
    virtual void ExitNode(const UnaryPrefixOperationNode& node);
    virtual void ExitNode(const UnaryPostfixOperationNode& node);
    virtual void ExitNode(const CallArgumentsNode& node);
    virtual void ExitNode(const TypeArgumentsNode& node);
    virtual void ExitNode(const IndexSuffixNode& node);
    virtual void ExitNode(const CallSuffixNode& node);
    virtual void ExitNode(const MemberAccessNode& node);
    virtual void ExitNode(const IfExpression& node);
    virtual void ExitNode(const BlockNode& node);

    virtual void ExitNode(const DeclarationBlock& node);
    virtual void ExitNode(const ClassDeclaration& node);
    virtual void ExitNode(const ParameterNode& node);
    virtual void ExitNode(const VariableNode& node);
    virtual void ExitNode(const ParameterList& node);
    virtual void ExitNode(const FunctionDeclaration& node);
    virtual void ExitNode(const PropertyDeclaration& node);
};
