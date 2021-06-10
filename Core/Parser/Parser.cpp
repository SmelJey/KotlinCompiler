#include "Parser.h"

#include "ExpressionNodes.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"
#include "StatementNodes.h"

Parser::Parser(Lexer& lexer) : myLexer(lexer) {
    myLexer.NextLexeme();
}

Lexer& Parser::GetLexer() {
    return myLexer;
}


Pointer<DeclarationBlock> Parser::Parse() {
    return ParseDeclarations(false);
}

// (classDeclaration | functionDeclaration | propertyDeclaration semis?)*
Pointer<DeclarationBlock> Parser::ParseDeclarations(bool isClass) {
    Pointer<DeclarationBlock> declarations = std::make_unique<DeclarationBlock>();

    ConsumeSemicolons();
    Lexeme curLexeme = myLexer.GetLexeme();

    while (curLexeme.GetType() != Lexeme::LexemeType::EndOfFile && !(curLexeme.GetType() == Lexeme::LexemeType::RCurl && isClass)) {
        if (curLexeme.GetType() != Lexeme::LexemeType::Keyword) {
            AddError(*declarations, curLexeme, (isClass ? "Expecting member declaration"
                                                           : "Expecting a top level declaration"));

            myLexer.NextLexeme();
        } else {
            std::string keyword = curLexeme.GetValue<std::string>();

            if (keyword == "class") {
                if (isClass) {
                    myLexer.NextLexeme();
                    AddError(*declarations, curLexeme, "Class is not allowed here");
                } else {
                    declarations->AddDeclaration(ParseClass());
                }
            } else if (keyword == "fun") {
                declarations->AddDeclaration(ParseFunction());
            } else if (keyword == "var" || keyword == "val") {
                declarations->AddDeclaration(ParseProperty());
            } else {
                AddError(*declarations, curLexeme, "Unsupported keyword");
                myLexer.NextLexeme();
            }
        }

        ConsumeSemicolons();
        curLexeme = myLexer.GetLexeme();
    }

    return declarations;
}

// 'class' simpleIdentifier classBody?
Pointer<IDeclaration> Parser::ParseClass() {
    myLexer.NextLexeme();

    Pointer<ClassDeclaration> classDecl = std::make_unique<ClassDeclaration>();
    classDecl->SetIdentifier(ParseIdentifier("Class declaration must have a name"));

    if (myLexer.GetLexeme().GetType() != Lexeme::LexemeType::LCurl) {
        return classDecl;
    }

    myLexer.NextLexeme();

    classDecl->SetBody(ParseDeclarations(true));
    ConsumeLexeme(Lexeme::LexemeType::RCurl, *classDecl, "Expecting '}'");
    return classDecl;
}

// 'fun' simpleIdentifier functionValueParameters (':' type)? functionBody
Pointer<IDeclaration> Parser::ParseFunction() {
    myLexer.NextLexeme();

    Pointer<FunctionDeclaration> functionDecl = std::make_unique<FunctionDeclaration>();
    functionDecl->SetIdentifier(ParseIdentifier("Function declaration must have a name"));
    functionDecl->SetParameters(ParseParameters());

    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpColon) {
        functionDecl->SetReturnNode(ParseType());
    }

    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpAssign) {
        myLexer.NextLexeme();
        functionDecl->SetBody(ParseExpression());
    } else {
        functionDecl->SetBody(ParseBlock());
    }

    return functionDecl;
}

// '(' (functionValueParameter (',' functionValueParameter)* ','?)? ')'
Pointer<ParameterList> Parser::ParseParameters() {
    Pointer<ParameterList> params = std::make_unique<ParameterList>();
    ConsumeLexeme(Lexeme::LexemeType::LParen, *params, "Expecting '('");

    bool isLastArg = false;
    while (myLexer.GetLexeme().GetType() != Lexeme::LexemeType::RParen && myLexer.GetLexeme().GetType() != Lexeme::LexemeType::EndOfFile) {
        if (isLastArg) {
            AddError(*params, myLexer.GetLexeme(), "Expecting ','");
        }

        params->AddParameter(std::move(ParseParameter()));

        isLastArg = true;
        if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpComma) {
            myLexer.NextLexeme();
            isLastArg = false;
        }
    }

    ConsumeLexeme(Lexeme::LexemeType::RParen, *params, "Expecting ')'");
    return params;
}

// simpleIdentifier ':' type ('=' expression)? 
Pointer<ParameterNode> Parser::ParseParameter() {
    Pointer<ParameterNode> param = std::make_unique<ParameterNode>();
    Lexeme curLexeme = myLexer.GetLexeme();
    param->SetIdentifier(ParseIdentifier("Parameter name expected"));

    bool isError = curLexeme.GetType() != Lexeme::LexemeType::Identifier;
    curLexeme = myLexer.GetLexeme();

    if (curLexeme.GetType() != Lexeme::LexemeType::OpColon) {
        param->SetTypeNode(std::make_unique<ErrorNode>(curLexeme.CopyEmptyOfType(Lexeme::LexemeType::Identifier),
                                                       "A type annotation is required on a value parameter"));
        if (isError) {
            myLexer.NextLexeme();
        }
        return param;
    }
    myLexer.NextLexeme();
    param->SetTypeNode(std::move(ParseType()));

    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpAssign) {
        myLexer.NextLexeme();
        param->SetDefaultNode(ParseExpression());
    }

    return param;
}

// ( ('(' type ')') | (simpleIdentifier ('.' simpleIdentifier)*) )
Pointer<ISyntaxNode> Parser::ParseType() {
    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::LParen) {
        myLexer.NextLexeme();
        Pointer<ISyntaxNode> typeNode = ParseType();
        ConsumeLexeme(Lexeme::LexemeType::RParen, *typeNode, "Expecting ')'");

        return typeNode;
    }

    Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() != Lexeme::LexemeType::Identifier) {
        return std::make_unique<ErrorNode>(curLexeme, "Type expected");
    }

    myLexer.NextLexeme();
    return std::make_unique<TypeNode>(curLexeme);
}

Pointer<ISyntaxNode> Parser::ParseStatement() {
    Lexeme curLexeme = myLexer.GetLexeme();

    if (curLexeme.GetType() == Lexeme::LexemeType::Keyword) {
        std::string keyword = curLexeme.GetValue<std::string>();
        if (keyword == "val" || keyword == "var") {
            return ParseProperty();
        }
        if (keyword == "fun") {
            return ParseFunction();
        }
        if (keyword == "class") {
            return ParseClass();
        }
        
        if (keyword == "for") {
            return ParseForLoop();
        } 
        if (keyword == "while") {
            return ParseWhileLoop();
        } 
        if (keyword == "do") {
            return ParseDoWhileLoop();
        }
        if (keyword == "if" || keyword == "break" || keyword == "continue" || keyword == "return") {
            return ParsePrimary();
        }
        return std::make_unique<ErrorNode>(myLexer.NextLexeme(), "Unsupported keyword");
    }

    return ParseAssignment();
}

// '{' (statement (semis statement)*)? semis? '}'
Pointer<BlockNode> Parser::ParseBlock() {
    Pointer<BlockNode> blockNode = std::make_unique<BlockNode>();
    ConsumeLexeme(Lexeme::LexemeType::LCurl, *blockNode, "Expecting '{'");
    ConsumeSemicolons();

    while (myLexer.GetLexeme().GetType() != Lexeme::LexemeType::EndOfFile && myLexer.GetLexeme().GetType() != Lexeme::LexemeType::RCurl) {
        blockNode->AddStatement(ParseStatement());
        ConsumeSemicolons();
    }

    ConsumeLexeme(Lexeme::LexemeType::RCurl, *blockNode, "Expecting '}'");
    return blockNode;
}

Pointer<ISyntaxNode> Parser::ParseControlStructureBody(bool acceptSemicolons = false) {
    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::LCurl) {
        return ParseBlock();
    }
    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpSemicolon && acceptSemicolons) {
        myLexer.NextLexeme();
        return std::make_unique<EmptyStatement>();
    }

    return ParseStatement();
}

// 'for' '(' (variableDeclaration | multiVariableDeclaration) 'in' expression ')' controlStructureBody?
Pointer<ForNode> Parser::ParseForLoop() {
    myLexer.NextLexeme();
    Pointer<ForNode> forLoop = std::make_unique<ForNode>();

    ConsumeLexeme(Lexeme::LexemeType::LParen, *forLoop, "Expecting '(' to open a loop range");
    forLoop->SetVariable(ParseVariable());

    ConsumeLexeme(Lexeme::LexemeType::OpIn, *forLoop, "Expecting 'in'");
    forLoop->SetExpression(ParseExpression());

    ConsumeLexeme(Lexeme::LexemeType::RParen, *forLoop, "Expecting ')'");
    forLoop->SetBody(ParseControlStructureBody());

    return forLoop;
}

// 'while' '(' expression ')' (controlStructureBody | ';')
Pointer<WhileNode> Parser::ParseWhileLoop() {
    myLexer.NextLexeme();
    Pointer<WhileNode> whileLoop = std::make_unique<WhileNode>();

    ConsumeLexeme(Lexeme::LexemeType::LParen, *whileLoop, "Expecting '('");
    whileLoop->SetExpression(ParseExpression());
    ConsumeLexeme(Lexeme::LexemeType::RParen, *whileLoop, "Expecting ')'");
    whileLoop->SetBody(ParseControlStructureBody(true));

    return whileLoop;
}

// 'do' controlStructureBody 'while' '(' expression ')'
Pointer<DoWhileNode> Parser::ParseDoWhileLoop() {
    myLexer.NextLexeme();
    Pointer<DoWhileNode> doWhileLoop = std::make_unique<DoWhileNode>();

    doWhileLoop->SetBody(ParseControlStructureBody());
    ConsumeLexeme(Lexeme::LexemeType::Keyword, "while", *doWhileLoop, "Expecting 'while' followed by a post-condition");
    ConsumeLexeme(Lexeme::LexemeType::LParen, *doWhileLoop, "Expecting '('");

    doWhileLoop->SetExpression(ParseExpression());
    ConsumeLexeme(Lexeme::LexemeType::RParen, *doWhileLoop, "Expecting ')'");

    return doWhileLoop;
}

// (directlyAssignableExpression assignmentAndOperator)? expression
Pointer<ISyntaxNode> Parser::ParseAssignment() {
    std::unique_ptr<ISyntaxNode> assignable = ParseExpression();

    Lexeme curLexeme = myLexer.GetLexeme();
    if (ParserUtils::AssignmentOperations.count(curLexeme.GetType())) {
        Pointer<Assignment> assignment = std::make_unique<Assignment>(myLexer.NextLexeme());

        if (!ParserUtils::IsDirectlyAssignable(assignable.get())) {
            AddError(*assignment, curLexeme, "Variable expected");
        }
        
        assignment->SetAssignable(std::move(assignable));
        assignment->SetExpression(ParseExpression());
        return assignment;
    }

    return assignable;
}

// ('val' | 'var') variableDeclaration ('=' expression)? ';'?
Pointer<IDeclaration> Parser::ParseProperty() {
    Lexeme keyword = myLexer.NextLexeme();
    Pointer<PropertyDeclaration> propertyDecl = std::make_unique<PropertyDeclaration>(keyword);
    propertyDecl->SetIdentifier(ParseIdentifier("Expecting property name"));

    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpColon) {
        myLexer.NextLexeme();
        propertyDecl->SetType(ParseType());
    }

    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpAssign) {
        myLexer.NextLexeme();
        propertyDecl->SetInitialization(ParseExpression());
    }

    return propertyDecl;
}

// simpleIdentifier (':' type)?
Pointer<VariableNode> Parser::ParseVariable() {
    Pointer<VariableNode> variable = std::make_unique<VariableNode>();

    variable->SetIdentifier(ParseIdentifier("Expecting a variable name"));
    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpColon) {
        variable->SetTypeNode(ParseType());
    }

    return variable;
}

Pointer<IdentifierNode> Parser::ParseIdentifier(const std::string& errorMessage) {
    Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() != Lexeme::LexemeType::Identifier) {
        curLexeme = curLexeme.CopyEmptyOfType(Lexeme::LexemeType::Identifier);
    }

    Pointer<IdentifierNode> identifier = std::make_unique<IdentifierNode>(curLexeme);
    ConsumeLexeme(Lexeme::LexemeType::Identifier, *identifier, errorMessage);
    return identifier;
}

// LeftAssociative(0)
Pointer<ILexemeNode> Parser::ParseExpression() {
    return ParseLeftAssociative(0);
}

// LeftAssociative(n+1) | LeftAssociative(n+1) operator(n) LeftAssociative(n+1)
Pointer<ILexemeNode> Parser::ParseLeftAssociative(size_t priority) {
    if (priority == ParserUtils::OperationsPriority.size()) {
        return ParsePrefix();
    }

    Pointer<ILexemeNode> leftOperand = ParseLeftAssociative(priority + 1);
    Lexeme operation = myLexer.GetLexeme();

    while (ParserUtils::OperationsPriority.at(priority).count(operation.GetType())
           // infixFunctionCall 
           || (priority == 5 && operation.GetType() == Lexeme::LexemeType::Identifier)) {
        int lastRow = myLexer.PrevLexeme().GetRow();
        if (priority == 5 && operation.GetRow() != lastRow) {
            break;
        }
        myLexer.NextLexeme();
        Pointer<ILexemeNode> rightOperand = ParseLeftAssociative(priority + 1);

        leftOperand = std::make_unique<BinOperationNode>(operation, std::move(leftOperand), std::move(rightOperand));
        operation = myLexer.GetLexeme();
    }

    return leftOperand;
}

// prefixUnaryOperator* postfixUnaryExpression
Pointer<ILexemeNode> Parser::ParsePrefix() {
    const Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() == Lexeme::LexemeType::OpSub || curLexeme.GetType() == Lexeme::LexemeType::OpAdd
        || curLexeme.GetType() == Lexeme::LexemeType::OpInc || curLexeme.GetType() == Lexeme::LexemeType::OpDec
        || curLexeme.GetType() == Lexeme::LexemeType::OpExclMark) {
        myLexer.NextLexeme();
        return std::make_unique<UnaryPrefixOperationNode>(curLexeme, ParsePrefix());
    }

    return ParsePostfix();
}

// primaryExpression postfixUnarySuffix*
Pointer<ILexemeNode> Parser::ParsePostfix() {
    Pointer<ILexemeNode> operand = ParsePrimary();
    Lexeme curLexeme = myLexer.GetLexeme();

    while (ParserUtils::PostfixOperations.count(curLexeme.GetType())) {
        myLexer.NextLexeme();
        if (curLexeme.GetType() == Lexeme::LexemeType::OpInc || curLexeme.GetType() == Lexeme::LexemeType::OpDec) {
            operand = std::make_unique<UnaryPostfixOperationNode>(curLexeme, std::move(operand));

        } else if (curLexeme.GetType() == Lexeme::LexemeType::OpDot) {
            std::unique_ptr<MemberAccessNode> memberAccess = std::make_unique<MemberAccessNode>(curLexeme, std::move(operand));
            curLexeme = myLexer.GetLexeme();

            // TODO: refactor
            if (curLexeme.GetType() != Lexeme::LexemeType::Identifier) {
                memberAccess->SetMember(std::make_unique<ErrorNode>(curLexeme, "Name expected"));
            } else {
                memberAccess->SetMember(std::make_unique<IdentifierNode>(curLexeme));
                myLexer.NextLexeme();
            }
            operand = std::move(memberAccess);

        } else if (curLexeme.GetType() == Lexeme::LexemeType::LParen) {
            Pointer<CallSuffixNode> callNode = std::make_unique<CallSuffixNode>(curLexeme, std::move(operand));
            Pointer<CallArgumentsNode> args = ParseArguments(Lexeme::LexemeType::RParen);
            callNode->SetArguments(std::move(args));

            ConsumeLexeme(Lexeme::LexemeType::RParen, *callNode, "Expecting ')'");
            operand = std::move(callNode);

        } else if (curLexeme.GetType() == Lexeme::LexemeType::LSquare) {
            Pointer<IndexSuffixNode> indexNode = std::make_unique<IndexSuffixNode>(curLexeme, std::move(operand));
            Pointer<CallArgumentsNode> args = ParseArguments(Lexeme::LexemeType::RSquare);

            if (args->GetArguments().empty()) {
                AddError(*indexNode, curLexeme, "Expecting an index");
            }
            indexNode->SetArguments(std::move(args));

            ConsumeLexeme(Lexeme::LexemeType::RSquare, *indexNode, "Expecting ']'");
            operand = std::move(indexNode);
        }

        curLexeme = myLexer.GetLexeme();
    }

    return operand;
}

Pointer<CallArgumentsNode> Parser::ParseArguments(Lexeme::LexemeType rParen) {
    Pointer<CallArgumentsNode> arguments = std::make_unique<CallArgumentsNode>();
    bool isLastArg = false;
    while (myLexer.GetLexeme().GetType() != rParen && myLexer.GetLexeme().GetType() != Lexeme::LexemeType::EndOfFile) {
        if (isLastArg) {
            AddError(*arguments, myLexer.GetLexeme(), "Expecting ','");
        }

        arguments->AddArgument(ParseExpression());

        isLastArg = true;
        if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpComma) {
            myLexer.NextLexeme();
            isLastArg = false;
        }
    }

    return arguments;
}

// Identifier | Number | String | '(' LeftAssociative(0) ')' | ifExpression | jumpExpression
Pointer<ILexemeNode> Parser::ParsePrimary() {
    const Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() == Lexeme::LexemeType::Keyword) {
        std::string keyword = curLexeme.GetValue<std::string>();
        if (keyword == "true" || keyword == "false") {
            return std::make_unique<BooleanNode>(myLexer.NextLexeme());
        }
        if (keyword == "break") {
            return std::make_unique<BreakNode>(myLexer.NextLexeme());
        }
        if (keyword == "continue") {
            return std::make_unique<ContinueNode>(myLexer.NextLexeme());
        }
        if (keyword == "return") {
            Pointer<ReturnNode> returnNode = std::make_unique<ReturnNode>(myLexer.NextLexeme());
            Lexeme nextLexeme = myLexer.GetLexeme();
            if (nextLexeme.GetRow() == curLexeme.GetRow() && nextLexeme.GetType() != Lexeme::LexemeType::RCurl && nextLexeme.GetType() != Lexeme::LexemeType::EndOfFile) {
                returnNode->SetExpression(ParseExpression());
            }
            
            return returnNode;
        }
        if (keyword == "if") {
            return ParseIfExpression();
        }
    }

    myLexer.NextLexeme();
    if (curLexeme.GetType() == Lexeme::LexemeType::LParen) {
        Pointer<ILexemeNode> expr = ParseExpression();
        ConsumeLexeme(Lexeme::LexemeType::RParen, *expr, "Expecting ')'");
        return expr;
    }
    if (curLexeme.GetType() == Lexeme::LexemeType::Identifier) {
        return std::make_unique<IdentifierNode>(curLexeme);
    }
    if (LexerUtils::IsIntegerType(curLexeme.GetType())) {
        return CreateNodeOfType<IntegerNode>(curLexeme);
    }
    if (LexerUtils::IsRealType(curLexeme.GetType())) {
        return CreateNodeOfType<RealNode>(curLexeme);
    }
    if (curLexeme.GetType() == Lexeme::LexemeType::String || curLexeme.GetType() == Lexeme::LexemeType::RawString) {
        return std::make_unique<StringNode>(curLexeme);
    }

    return std::make_unique<ErrorNode>(ErrorNode(curLexeme));
}

// 'if' '(' expression ')' (controlStructureBody | (controlStructureBody? ';'? 'else' (controlStructureBody | ';')) | ';')
Pointer<ILexemeNode> Parser::ParseIfExpression() {
    Pointer<IfExpression> ifExpr = std::make_unique<IfExpression>(myLexer.NextLexeme());

    ConsumeLexeme(Lexeme::LexemeType::LParen, *ifExpr, "Expecting '('");
    ifExpr->SetExpression(ParseExpression());
    ConsumeLexeme(Lexeme::LexemeType::RParen, *ifExpr, "Expecting ')'");

    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::Keyword && myLexer.GetLexeme().GetValue<std::string>() == "else") {
        ifExpr->SetIfBody(std::make_unique<EmptyStatement>());
        myLexer.NextLexeme();
        ifExpr->SetElseBody(ParseControlStructureBody(true));
        return ifExpr;
    }

    ifExpr->SetIfBody(ParseControlStructureBody(true));

    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::Keyword && myLexer.GetLexeme().GetValue<std::string>() == "else") {
        myLexer.NextLexeme();
        ifExpr->SetElseBody(ParseControlStructureBody(true));
    }

    return ifExpr;
}

void Parser::AddError(ISyntaxNode& root, const Lexeme& location, const std::string& error) const {
    root.AddError(std::make_unique<ErrorNode>(location, error));
}

bool Parser::ConsumeLexeme(Lexeme::LexemeType lexemeType, ISyntaxNode& host, const std::string& error) {
    if (myLexer.GetLexeme().GetType() != lexemeType) {
        AddError(host, myLexer.GetLexeme(), error);
        return false;
    }

    myLexer.NextLexeme();
    return true;
}

bool Parser::ConsumeLexeme(Lexeme::LexemeType lexemeType, const std::string& text, ISyntaxNode& host,
        const std::string& error) {
    if (myLexer.GetLexeme().GetText() != text) {
        AddError(host, myLexer.GetLexeme(), error);
        return false;
    }

    return ConsumeLexeme(lexemeType, host, error);
}

void Parser::ConsumeSemicolons() {
    while (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpSemicolon) {
        myLexer.NextLexeme();
    }
}
