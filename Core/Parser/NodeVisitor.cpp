#include "NodeVisitor.h"

#include "ISyntaxNode.h"

NodeVisitor::~NodeVisitor() = default;

void NodeVisitor::VisitNode(const IVisitable & node, int depth) {
    EnterNode(node, depth);
    node.AcceptVisitor(*this, depth + 1);
    ExitNode(node, depth);
}

ToStringVisitor::ToStringVisitor() = default;

std::vector<std::string> ToStringVisitor::GetStringData() const {
    return myStringData;
}

void ToStringVisitor::EnterNode(const IVisitable& node, int depth) {
    std::string indent;
    for (int d = 0; d < depth; d++) {
        indent += "| ";
    }

    myStringData.push_back(indent + node.ToString());
}

void ToStringVisitor::ExitNode(const IVisitable& node, int depth) {}

CuteToStringVisitor::CuteToStringVisitor() = default;

std::vector<std::string> CuteToStringVisitor::GetStringData() const {
    return myStringData;
}

void CuteToStringVisitor::ShowSemanticsAnnotations() {
    myShowSemantics = true;
}

void CuteToStringVisitor::EnterNode(const IVisitable& node, int depth) {
    myStack.push(std::vector<std::string>());
}

void CuteToStringVisitor::ExitNode(const IVisitable& node, int depth) {
    auto myVec = myStack.top();
    myStack.pop();
    std::vector<std::string>* topVec;
    if (depth != 0) {
        topVec = &myStack.top();
    } else {
        topVec = &myStringData;
    }

    std::string res = node.ToString();
    if (myShowSemantics) {
        const ITypedNode* typeSym = dynamic_cast<const ITypedNode*>(&node);

        if (typeSym != nullptr) {
            res.append(" \t\t\t :: " + typeSym->GetType()->GetName());
        } else {
            const ISymbolAnnotatedNode* symbolAnnotated = dynamic_cast<const ISymbolAnnotatedNode*>(&node);
            if (symbolAnnotated != nullptr) {
                res.append(" \t\t\t :: " + symbolAnnotated->GetSymbol()->GetName());
            }
        }
    }

    topVec->push_back(res);

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
