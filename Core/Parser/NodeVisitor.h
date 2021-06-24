#pragma once

#include <string>
#include <stack>
#include <vector>

class IVisitable;

class NodeVisitor {
public:
    virtual ~NodeVisitor();

    void VisitNode(const IVisitable& node, int depth);
protected:
    virtual void EnterNode(const IVisitable& node, int depth) = 0;
    virtual void ExitNode(const IVisitable& node, int depth) = 0;
};

class ToStringVisitor : public NodeVisitor {
public:
    ToStringVisitor();

    std::vector<std::string> GetStringData() const;
protected:
    void EnterNode(const IVisitable& node, int depth) override;
    void ExitNode(const IVisitable& node, int depth) override;

private:
    std::vector<std::string> myStringData;
};

class CuteToStringVisitor : public NodeVisitor {
public:
    CuteToStringVisitor();

    std::vector<std::string> GetStringData() const;

    void ShowSemanticsAnnotations();
protected:
    void EnterNode(const IVisitable& node, int depth) override;
    void ExitNode(const IVisitable& node, int depth) override;

private:
    std::stack<std::vector<std::string>> myStack;
    static const char LINK_CHAR = '|';
    static const char CHILD_CHAR = '|';

    std::vector<std::string> myStringData;
    bool myShowSemantics = false;
};