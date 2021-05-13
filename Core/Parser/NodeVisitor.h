#pragma once

class ISyntaxNode;

class NodeVisitor {
public:
    virtual ~NodeVisitor();

    void VisitNode(ISyntaxNode& node, int depth);
protected:
    virtual void ProcessNode(ISyntaxNode& node, int depth) = 0;
};

class PrintVisitor : public NodeVisitor {
public:
    PrintVisitor();

protected:
    void ProcessNode(ISyntaxNode& node, int depth) override;
};