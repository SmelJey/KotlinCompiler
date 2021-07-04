#include "PrintVisitors.h"

#include "Parser/ISyntaxNode.h"

ToStringVisitor::ToStringVisitor() : myDepth(0) {}

std::vector<std::string> ToStringVisitor::GetStringData() const {
    return myStringData;
}

void ToStringVisitor::EnterNode(const IVisitable& node) {
    std::string indent;
    for (int d = 0; d < myDepth; d++) {
        indent += "| ";
    }

    myStringData.push_back(indent + node.ToString());

    myDepth++;
    INodeVisitor::EnterNode(node);
}

void ToStringVisitor::ExitNode(const IVisitable& node) {
    myDepth--;
}

CuteToStringVisitor::CuteToStringVisitor() : myDepth(0) {}

std::vector<std::string> CuteToStringVisitor::GetStringData() const {
    return myStringData;
}

void CuteToStringVisitor::ShowSemanticsAnnotations() {
    myShowSemantics = true;
}

void CuteToStringVisitor::EnterNode(const IVisitable& node) {
    myStack.push(std::vector<std::string>());

    myDepth++;
    INodeVisitor::EnterNode(node);
}

void CuteToStringVisitor::ExitNode(const IVisitable& node) {
    myDepth--;

    auto myVec = myStack.top();
    myStack.pop();
    std::vector<std::string>* topVec;
    if (myDepth != 0) {
        topVec = &myStack.top();
    } else {
        topVec = &myStringData;
    }

    std::string res = node.ToString();
    if (myShowSemantics) {
        const IAnnotatedNode* typeSym = dynamic_cast<const IAnnotatedNode*>(&node);

        int spacingRequired = 60 - myDepth * 2 - res.size();
        std::string spacing = "";
        for (int i = 0; i < spacingRequired; i++) {
            spacing += " ";
        }
        if (typeSym != nullptr) {
            res.append(spacing + ":: " + typeSym->GetSymbol()->GetName());
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
