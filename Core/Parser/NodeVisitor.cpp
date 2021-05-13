#include "NodeVisitor.h"

#include <iostream>


#include "ISyntaxNode.h"

NodeVisitor::~NodeVisitor() = default;

void NodeVisitor::VisitNode(ISyntaxNode& node, int depth) {
    ProcessNode(node, depth);
    node.AcceptVisitor(*this, depth + 1);
}

PrintVisitor::PrintVisitor() = default;

void PrintVisitor::ProcessNode(ISyntaxNode& node, int depth) {
    std::string indent;
    for (int d = 0; d < depth; d++) {
        indent += " |";
    }

    std::cout << indent << " |" << node << std::endl;
}
