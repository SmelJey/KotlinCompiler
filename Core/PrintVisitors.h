#pragma once
#include <string>
#include <stack>
#include <vector>

#include "Parser/INodeVisitor.h"

class ToStringVisitor : public INodeVisitor {
public:
    ToStringVisitor();

    std::vector<std::string> GetStringData() const;

    void EnterNode(const IVisitable& node) override;
    void ExitNode(const IVisitable& node) override;

private:
    std::vector<std::string> myStringData;
    int myDepth;
};

class CuteToStringVisitor : public INodeVisitor {
public:
    CuteToStringVisitor();

    std::vector<std::string> GetStringData() const;

    void ShowSemanticsAnnotations();

    void EnterNode(const IVisitable& node) override;
    void ExitNode(const IVisitable& node) override;

private:
    int myDepth;
    std::stack<std::vector<std::string>> myStack;
    static const char LINK_CHAR = '|';
    static const char CHILD_CHAR = '|';

    std::vector<std::string> myStringData;
    bool myShowSemantics = false;
};