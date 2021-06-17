#include "NodeVisitor.h"

#include "AbstractNode.h"

NodeVisitor::~NodeVisitor() = default;

void NodeVisitor::VisitNode(const AbstractNode& node, int depth) {
    EnterNode(node, depth);
    node.InternalAcceptVisitor(*this, depth + 1);
    ExitNode(node, depth);
}

ToStringVisitor::ToStringVisitor() = default;

std::vector<std::string> ToStringVisitor::GetStringData() const {
    return myStringData;
}

void ToStringVisitor::EnterNode(const AbstractNode& node, int depth) {
    std::string indent;
    for (int d = 0; d < depth; d++) {
        indent += "| ";
    }

    myStringData.push_back(indent + node.ToString());
}

void ToStringVisitor::ExitNode(const AbstractNode& node, int depth) {}

CuteToStringVisitor::CuteToStringVisitor() = default;

std::vector<std::string> CuteToStringVisitor::GetStringData() const {
    return myStringData;
}

void CuteToStringVisitor::EnterNode(const AbstractNode & node, int depth) {
    myStack.push(std::vector<std::string>());
}

void CuteToStringVisitor::ExitNode(const AbstractNode & node, int depth) {
    auto myVec = myStack.top();
    myStack.pop();
    std::vector<std::string>* topVec;
    if (depth != 0) {
        topVec = &myStack.top();
    } else {
        topVec = &myStringData;
    }

    topVec->push_back(node.ToString());

    int lastChild = 0;
    for (int i = 0; i < myVec.size(); i++) {
        std::string str = myVec[i];
        if (str[0] != LINK_CHAR && str[0] != ' ' && str[0] != CHILD_CHAR) {
            lastChild = i;
        }
    }

    for (int i = 0; i < myVec.size(); i++) {
        std::string str = myVec[i];
        if (i <= lastChild) {
            if (str[0] == LINK_CHAR || str[0] == ' ' || str[0] == CHILD_CHAR) {
                topVec->push_back(std::string{ LINK_CHAR } + " " + str);
            } else {
                topVec->push_back(std::string{ CHILD_CHAR } + "-" + str);
            }
        } else {
            topVec->push_back("  " + str);
        }
    }
}
