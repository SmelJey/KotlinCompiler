#include "NodeVisitor.h"

#include <iostream>

#include "ISyntaxNode.h"

NodeVisitor::~NodeVisitor() = default;

void NodeVisitor::VisitNode(const ISyntaxNode& node, int depth) {
    ProcessNode(node, depth);
    node.InternalAcceptVisitor(*this, depth + 1);
}

PrintVisitor::PrintVisitor() = default;

void PrintVisitor::ProcessNode(const ISyntaxNode& node, int depth) {
    std::string indent;
    for (int d = 0; d < depth; d++) {
        indent += "| ";
    }

    std::cout << indent << node << std::endl;
}

ToStringVisitor::ToStringVisitor() = default;

std::vector<std::string> ToStringVisitor::GetStringData() const {
    return myStringData;
}

void ToStringVisitor::ProcessNode(const ISyntaxNode& node, int depth) {
    std::string indent;
    for (int d = 0; d < depth; d++) {
        indent += "| ";
    }

    myStringData.push_back(indent + node.ToString());
}
