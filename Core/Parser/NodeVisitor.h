#pragma once

#include <string>
#include <stack>
#include <vector>

class AbstractNode;

class NodeVisitor {
public:
    virtual ~NodeVisitor();

    void VisitNode(const AbstractNode& node, int depth);
protected:
    virtual void EnterNode(const AbstractNode& node, int depth) = 0;
    virtual void ExitNode(const AbstractNode& node, int depth) = 0;
};

class ToStringVisitor : public NodeVisitor {
public:
    ToStringVisitor();

    std::vector<std::string> GetStringData() const;
protected:
    void EnterNode(const AbstractNode& node, int depth) override;
    void ExitNode(const AbstractNode& node, int depth) override;

private:
    std::vector<std::string> myStringData;
};

class CuteToStringVisitor : public NodeVisitor {
public:
    CuteToStringVisitor();

    std::vector<std::string> GetStringData() const;
protected:
    void EnterNode(const AbstractNode& node, int depth) override;
    void ExitNode(const AbstractNode& node, int depth) override;

private:
    std::stack<std::vector<std::string>> myStack;
    static const char LINK_CHAR = '|';
    static const char CHILD_CHAR = '|';

    std::vector<std::string> myStringData;
};