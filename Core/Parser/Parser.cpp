#include "Parser.h"

#include "ExpressionNodes.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"
#include "StatementNodes.h"
#include "Semantics/ClassSymbol.h"
#include "Semantics/FunctionSymbol.h"

Parser::Parser(Lexer& lexer, SymbolTable* symbolTable) : myLexer(lexer), myTable(symbolTable) {
    myLexer.NextLexeme();
}

const Lexer& Parser::GetLexer() const {
    return myLexer;
}

const SymbolTable& Parser::GetSymbolTable() const {
    return myTable;
}

Pointer<DeclarationBlock> Parser::Parse() {
    return ParseDeclarations(false);
}

const std::vector<ErrorNode>& Parser::GetParsingErrors() const {
    return myParsingErrors;
}

const std::vector<ErrorNode>& Parser::GetSemanticsErrors() const {
    return mySemanticsErrors;
}

// (classDeclaration | functionDeclaration | propertyDeclaration semis?)*
Pointer<DeclarationBlock> Parser::ParseDeclarations(bool isClass) {
    Pointer<DeclarationBlock> declarations = std::make_unique<DeclarationBlock>();

    ConsumeSemicolons();
    Lexeme curLexeme = myLexer.GetLexeme();

    while (curLexeme.GetType() != LexemeType::EndOfFile && !(curLexeme.GetType() == LexemeType::RCurl && isClass)) {
        if (curLexeme.GetType() != LexemeType::Keyword) {
            AddParsingError(curLexeme, (isClass ? "Expecting member declaration"
                : "Expecting a top level declaration"));

            myLexer.NextLexeme();
        } else {
            std::string keyword = curLexeme.GetValue<std::string>();

            if (keyword == "class") {
                if (isClass) {
                    myLexer.NextLexeme();
                    AddParsingError(curLexeme, "Class is not allowed here");
                } else {
                    declarations->AddDeclaration(ParseClass());
                }
            } else if (keyword == "fun") {
                declarations->AddDeclaration(ParseFunction());
            } else if (keyword == "var" || keyword == "val") {
                declarations->AddDeclaration(ParseProperty());
            } else {
                AddParsingError(curLexeme, "Unsupported keyword");
                myLexer.NextLexeme();
            }
        }

        ConsumeSemicolons();
        curLexeme = myLexer.GetLexeme();
    }

    return declarations;
}

// 'class' simpleIdentifier classBody?
Pointer<ClassDeclaration> Parser::ParseClass() {
    myLexer.NextLexeme();

    Pointer<ClassDeclaration> classDecl = std::make_unique<ClassDeclaration>();
    classDecl->SetIdentifier(ParseIdentifier("Class declaration must have a name"));

    if (myLexer.GetLexeme().GetType() != LexemeType::LCurl) {
        return classDecl;
    }

    myLexer.NextLexeme();

    classDecl->SetBody(ParseDeclarations(true));
    ConsumeLexeme(LexemeType::RCurl, *classDecl, "Expecting '}'");
    return classDecl;
}

// 'fun' simpleIdentifier functionValueParameters (':' type)? functionBody
Pointer<FunctionDeclaration> Parser::ParseFunction() {
    myLexer.NextLexeme();

    Pointer<FunctionDeclaration> functionDecl = std::make_unique<FunctionDeclaration>();
    functionDecl->SetIdentifier(ParseIdentifier("Function declaration must have a name"));
    functionDecl->SetParameters(ParseParameters());

    if (myLexer.GetLexeme().GetType() == LexemeType::OpColon) {
        myLexer.NextLexeme();
        functionDecl->SetReturn(ParseType());
    }

    Pointer<SymbolTable> functionSymbols = std::make_unique<SymbolTable>(myTable);
    myTable = functionSymbols.get();

    if (myLexer.GetLexeme().GetType() == LexemeType::OpAssign) {
        myLexer.NextLexeme();
        functionDecl->SetBody(ParseExpression());
    } else {
        functionDecl->SetBody(ParseBlock());
    }

    myTable = functionSymbols->GetParent();

    const ITypeSymbol* returnType = myTable->GetUnitSymbol();
    if (functionDecl->HasReturnNode()) {
        returnType = functionDecl->GetReturn().GetType();
    }

    std::vector<const ITypeSymbol*> params;
    for (auto& it : functionDecl->GetParameters().GetParameters()) {
        params.push_back(it->GetType().GetType());
    }

    Pointer<FunctionSymbol> funcSym = std::make_unique<FunctionSymbol>(functionDecl->GetIdentifierName(), returnType,
        params, std::move(functionSymbols));

    myTable->Add(std::move(funcSym));
    functionDecl->SetSymbol(myTable->GetFunction(functionDecl->GetIdentifierName(), params));
    functionDecl->GetIdentifier().UpdateCandidates(myTable->GetSymbols(functionDecl->GetIdentifierName()));
    functionDecl->GetIdentifier().TryResolveFunc(params);

    return functionDecl;
}

// '(' (functionValueParameter (',' functionValueParameter)* ','?)? ')'
Pointer<ParameterList> Parser::ParseParameters() {
    Pointer<ParameterList> params = std::make_unique<ParameterList>();
    ConsumeLexeme(LexemeType::LParen, *params, "Expecting '('");

    while (myLexer.GetLexeme().GetType() != LexemeType::RParen && myLexer.GetLexeme().GetType() != LexemeType::EndOfFile) {
        params->AddParameter(std::move(ParseParameter()));

        if (myLexer.GetLexeme().GetType() != LexemeType::OpComma) {
            if (myLexer.GetLexeme().GetType() == LexemeType::RParen || myLexer.GetLexeme().GetType() == LexemeType::EndOfFile) {
                break;
            }

            AddParsingError(myLexer.GetLexeme(), "Expecting ','");
        } else {
            myLexer.NextLexeme();
        }
    }

    ConsumeLexeme(LexemeType::RParen, *params, "Expecting ')'");
    return params;
}

// simpleIdentifier ':' type ('=' expression)? 
Pointer<ParameterNode> Parser::ParseParameter() {
    Pointer<ParameterNode> param = std::make_unique<ParameterNode>();
    Lexeme curLexeme = myLexer.GetLexeme();
    param->SetIdentifier(ParseIdentifier("Parameter name expected"));

    bool isError = curLexeme.GetType() != LexemeType::Identifier;
    curLexeme = myLexer.GetLexeme();

    if (curLexeme.GetType() != LexemeType::OpColon) {
        AddParsingError(curLexeme, "A type annotation is required on a value parameter");
        param->SetType(CreateEmptyIdentifier(curLexeme));
        if (isError) {
            myLexer.NextLexeme();
        }
        return param;
    }
    myLexer.NextLexeme();
    param->SetType(std::move(ParseType()));

    if (myLexer.GetLexeme().GetType() == LexemeType::OpAssign) {
        myLexer.NextLexeme();
        param->SetDefault(ParseExpression());
    }

    return param;
}

// ( ('(' type ')') | (simpleIdentifier ('.' simpleIdentifier)*) )
Pointer<ITypedNode> Parser::ParseType() {
    if (myLexer.GetLexeme().GetType() == LexemeType::LParen) {
        myLexer.NextLexeme();
        Pointer<ITypedNode> typeNode = ParseType();
        ConsumeLexeme(LexemeType::RParen, *typeNode, "Expecting ')'");

        return typeNode;
    }

    Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() != LexemeType::Identifier) {
        AddParsingError(curLexeme, "Type expected");
        return std::make_unique<TypeNode>(curLexeme.CopyEmpty(), myTable->GetUnresolvedSymbol());
    }

    myLexer.NextLexeme();
    return std::make_unique<TypeNode>(curLexeme, myTable->GetType(curLexeme.GetValue<std::string>()));
}

Pointer<ISyntaxNode> Parser::ParseStatement() {
    Lexeme curLexeme = myLexer.GetLexeme();

    if (curLexeme.GetType() == LexemeType::Keyword) {
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
        if (keyword == "if") {
            return ParseIfExpression();
        }
        if (keyword == "break") {
            return CreateLexemeNode<BreakNode>(myLexer.NextLexeme());
        }
        if (keyword == "continue") {
            return CreateLexemeNode<ContinueNode>(myLexer.NextLexeme());
        }
        if (keyword == "return") {
            Pointer<ReturnNode> returnNode = std::make_unique<ReturnNode>(myLexer.NextLexeme());
            Lexeme nextLexeme = myLexer.GetLexeme();
            if (nextLexeme.GetRow() == curLexeme.GetRow() && nextLexeme.GetType() != LexemeType::RCurl
                && nextLexeme.GetType() != LexemeType::EndOfFile) {
                returnNode->SetExpression(ParseExpression());
            }

            return returnNode;
        }

        AddParsingError(myLexer.NextLexeme(), "Unsupported keyword");
        return std::make_unique<EmptyStatement>(myTable->GetUnitSymbol());
    }

    return ParseAssignment();
}

// '{' (statement (semis statement)*)? semis? '}'
Pointer<BlockNode> Parser::ParseBlock() {
    Pointer<BlockNode> blockNode = std::make_unique<BlockNode>();
    ConsumeLexeme(LexemeType::LCurl, *blockNode, "Expecting '{'");
    ConsumeSemicolons();

    while (myLexer.GetLexeme().GetType() != LexemeType::EndOfFile && myLexer.GetLexeme().GetType() != LexemeType::RCurl) {
        blockNode->AddStatement(ParseStatement());
        ConsumeSemicolons();
    }

    ConsumeLexeme(LexemeType::RCurl, *blockNode, "Expecting '}'");
    return blockNode;
}

Pointer<ISyntaxNode> Parser::ParseControlStructureBody(bool acceptSemicolons = false) {
    if (myLexer.GetLexeme().GetType() == LexemeType::LCurl) {
        return ParseBlock();
    }
    if (myLexer.GetLexeme().GetType() == LexemeType::OpSemicolon && acceptSemicolons) {
        myLexer.NextLexeme();
        return std::make_unique<EmptyStatement>(myTable->GetUnitSymbol());
    }

    return ParseStatement();
}

// 'for' '(' (variableDeclaration | multiVariableDeclaration) 'in' expression ')' controlStructureBody?
Pointer<ForNode> Parser::ParseForLoop() {
    myLexer.NextLexeme();
    Pointer<ForNode> forLoop = std::make_unique<ForNode>();

    ConsumeLexeme(LexemeType::LParen, *forLoop, "Expecting '(' to open a loop range");
    forLoop->SetVariable(ParseVariable());

    ConsumeLexeme(LexemeType::OpIn, *forLoop, "Expecting 'in'");
    forLoop->SetExpression(ParseExpression());

    ConsumeLexeme(LexemeType::RParen, *forLoop, "Expecting ')'");
    forLoop->SetBody(ParseControlStructureBody());

    return forLoop;
}

// 'while' '(' expression ')' (controlStructureBody | ';')
Pointer<WhileNode> Parser::ParseWhileLoop() {
    myLexer.NextLexeme();
    Pointer<WhileNode> whileLoop = std::make_unique<WhileNode>();

    ConsumeLexeme(LexemeType::LParen, *whileLoop, "Expecting '('");
    whileLoop->SetExpression(ParseExpression());
    ConsumeLexeme(LexemeType::RParen, *whileLoop, "Expecting ')'");
    whileLoop->SetBody(ParseControlStructureBody(true));

    return whileLoop;
}

// 'do' controlStructureBody 'while' '(' expression ')'
Pointer<DoWhileNode> Parser::ParseDoWhileLoop() {
    myLexer.NextLexeme();
    Pointer<DoWhileNode> doWhileLoop = std::make_unique<DoWhileNode>();

    doWhileLoop->SetBody(ParseControlStructureBody());
    ConsumeLexeme(LexemeType::Keyword, "while", *doWhileLoop, "Expecting 'while' followed by a post-condition");
    ConsumeLexeme(LexemeType::LParen, *doWhileLoop, "Expecting '('");

    doWhileLoop->SetExpression(ParseExpression());
    ConsumeLexeme(LexemeType::RParen, *doWhileLoop, "Expecting ')'");

    return doWhileLoop;
}

// (directlyAssignableExpression assignmentAndOperator)? expression
Pointer<ISyntaxNode> Parser::ParseAssignment() {
    Pointer<ITypedNode> assignable = ParseExpression();

    Lexeme curLexeme = myLexer.GetLexeme();
    if (ParserUtils::AssignmentOperations.count(curLexeme.GetType())) {
        Pointer<Assignment> assignment = std::make_unique<Assignment>(myLexer.NextLexeme());
        
        if (!ParserUtils::IsDirectlyAssignable(assignable.get())) {
            AddParsingError(curLexeme, "Variable expected");
        }
        
        assignment->SetAssignable(std::move(assignable));
        assignment->SetExpression(ParseExpression());
        return assignment;
    }

    return assignable;
}

// ('val' | 'var') variableDeclaration ('=' expression)? ';'?
Pointer<PropertyDeclaration> Parser::ParseProperty() {
    Lexeme keyword = myLexer.NextLexeme();
    Pointer<PropertyDeclaration> propertyDecl = std::make_unique<PropertyDeclaration>(keyword);
    propertyDecl->SetIdentifier(ParseIdentifier("Expecting property name"));

    if (myLexer.GetLexeme().GetType() == LexemeType::OpColon) {
        myLexer.NextLexeme();
        propertyDecl->SetType(ParseType());
    }

    bool isFailed = false;
    if (myLexer.GetLexeme().GetType() == LexemeType::OpAssign) {
        myLexer.NextLexeme();
        propertyDecl->SetInitialization(ParseExpression());
    } else {
        AddParsingError(myLexer.GetLexeme(), "Property must be initialized");
        isFailed = true;
        propertyDecl->SetInitialization(CreateEmptyIdentifier(myLexer.GetLexeme()));
    }

    if (!isFailed && propertyDecl->HasType() && propertyDecl->HasInitialization()
        && propertyDecl->GetType().GetType() != propertyDecl->GetInitialization().GetType()) {
        AddSemanticsError(myLexer.GetLexeme(), propertyDecl->GetInitialization().GetType()->GetName()
                          + " does not conform to the expected type " + propertyDecl->GetType().GetType()->GetName());
    }
    auto propertySym = std::make_unique<VariableSymbol>(propertyDecl->GetIdentifierName(),
        propertyDecl->GetInitialization().GetType(), propertyDecl->IsMutable());
    bool res = myTable->Add(std::move(propertySym));
    propertyDecl->SetSymbol(myTable->GetVariable(propertyDecl->GetIdentifierName()));
    propertyDecl->GetIdentifier().UpdateCandidates(myTable->GetSymbols(propertyDecl->GetIdentifierName()));
    propertyDecl->GetIdentifier().TryResolveVariable();

    if (!res) {
        AddSemanticsError(myLexer.GetLexeme(), "Conflicting declarations");
    }

    return propertyDecl;
}

// simpleIdentifier (':' type)?
Pointer<VariableNode> Parser::ParseVariable() {
    Pointer<VariableNode> variable = std::make_unique<VariableNode>();

    variable->SetIdentifier(ParseIdentifier("Expecting a variable name"));
    if (myLexer.GetLexeme().GetType() == LexemeType::OpColon) {
        myLexer.NextLexeme();
        variable->SetType(ParseType());
    }

    return variable;
}

Pointer<IdentifierNode> Parser::ParseIdentifier(const std::string& errorMessage) {
    Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() != LexemeType::Identifier) {
        curLexeme = curLexeme.CopyEmptyOfType(LexemeType::Identifier);
    }

    Pointer<IdentifierNode> identifier = CreateLexemeNode(curLexeme, myTable->GetUnresolvedSymbol(), myTable->GetSymbols(curLexeme.GetText()));
    ConsumeLexeme(LexemeType::Identifier, *identifier, errorMessage);
    return identifier;
}

// LeftAssociative(0)
Pointer<ITypedNode> Parser::ParseExpression() {
    return ParseLeftAssociative(0);
}

// LeftAssociative(n+1) | LeftAssociative(n+1) operator(n) LeftAssociative(n+1)
Pointer<ITypedNode> Parser::ParseLeftAssociative(size_t priority) {
    if (priority == ParserUtils::OperationsPriority.size()) {
        return ParsePrefix();
    }

    Pointer<ITypedNode> leftOperand = ParseLeftAssociative(priority + 1);
    Lexeme operation = myLexer.GetLexeme();

    while (ParserUtils::OperationsPriority.at(priority).count(operation.GetType())
           // infixFunctionCall 
           || (priority == 5 && operation.GetType() == LexemeType::Identifier)) {
        int lastRow = myLexer.PrevLexeme().GetRow();
        if (operation.GetRow() != lastRow) {
            break;
        }

        myLexer.NextLexeme();
        Pointer<ITypedNode> rightOperand = ParseLeftAssociative(priority + 1);

        const ITypeSymbol* leftType = dynamic_cast<const ITypeSymbol*>(leftOperand->GetSymbol());
        const ITypeSymbol* rightType = dynamic_cast<const ITypeSymbol*>(rightOperand->GetSymbol());
        Pointer<ITypeSymbol> resultType = std::make_unique<UnresolvedSymbol>();

        if (leftType != nullptr && rightType != nullptr) {
            resultType = leftType->IsApplicable(operation.GetType(), rightType);
        }

        leftOperand = std::make_unique<BinOperationNode>(
            operation, std::move(leftOperand), std::move(rightOperand), myTable->GetType(resultType->GetName()));
        operation = myLexer.GetLexeme();
    }

    return leftOperand;
}

// prefixUnaryOperator* postfixUnaryExpression
Pointer<ITypedNode> Parser::ParsePrefix() {
    const Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() == LexemeType::OpSub || curLexeme.GetType() == LexemeType::OpAdd
        || curLexeme.GetType() == LexemeType::OpInc || curLexeme.GetType() == LexemeType::OpDec
        || curLexeme.GetType() == LexemeType::OpExclMark) {
        myLexer.NextLexeme();
        Pointer<ITypedNode> prefix = ParsePrefix();
        Pointer<ITypeSymbol> resultType = prefix->GetType()->IsApplicable(curLexeme.GetType());
        return std::make_unique<UnaryPrefixOperationNode>(curLexeme, std::move(prefix), myTable->GetType(resultType->GetName()));
    }

    return ParsePostfix();
}

// primaryExpression postfixUnarySuffix*
Pointer<ITypedNode> Parser::ParsePostfix() {
    Pointer<ITypedNode> operand = ParsePrimary();
    Lexeme curLexeme = myLexer.GetLexeme();

    // TODO: Fix it
    while (ParserUtils::PostfixOperations.count(curLexeme.GetType())) {
        myLexer.NextLexeme();
        if (curLexeme.GetType() == LexemeType::OpInc || curLexeme.GetType() == LexemeType::OpDec) {
            Pointer<ITypeSymbol> resultType = operand->GetType()->IsApplicable(curLexeme.GetType());
            operand = std::make_unique<UnaryPostfixOperationNode>(curLexeme, std::move(operand), myTable->GetType(resultType->GetName()));

        } else if (curLexeme.GetType() == LexemeType::OpDot) {
            Pointer<ITypedNode> member = CreateEmptyIdentifier(curLexeme);
            if (myLexer.GetLexeme().GetType() == LexemeType::Identifier) {
                IdentifierNode* identifier = dynamic_cast<IdentifierNode*>(operand.get());
                if (identifier != nullptr) {
                    identifier->TryResolveVariable();
                }
                auto classSym = dynamic_cast<const ClassSymbol*>(operand->GetType());
                std::vector<const ISymbol*> candidates;
                if (classSym != nullptr) {
                    candidates = classSym->GetTable().GetSymbols(myLexer.GetLexeme().GetValue<std::string>());
                }
                member = CreateLexemeNode(myLexer.GetLexeme(), myTable->GetUnresolvedSymbol(), candidates);
                myLexer.NextLexeme();
            } else {
                AddParsingError(curLexeme, "Name expected");
            }

            std::unique_ptr<MemberAccessNode> memberAccess = std::make_unique<MemberAccessNode>(curLexeme, std::move(operand), std::move(member));
            operand = std::move(memberAccess);

        } else if (curLexeme.GetType() == LexemeType::LParen) {
            Pointer<CallArgumentsNode> args = ParseArguments(LexemeType::RParen);
            const ITypeSymbol* resType = myTable->GetUnresolvedSymbol();

            IdentifierNode* identifier = dynamic_cast<IdentifierNode*>(operand.get());
            if (identifier != nullptr) {
                std::vector<const ITypeSymbol*> argsTypes;
                for (auto& arg : args->GetArguments()) {
                    argsTypes.push_back(arg->GetType());
                }

                identifier->TryResolveFunc(argsTypes);
                resType = identifier->GetType();
            }

            Pointer<CallSuffixNode> callNode = std::make_unique<CallSuffixNode>(std::move(operand), resType);
            callNode->SetArguments(std::move(args));

            ConsumeLexeme(LexemeType::RParen, *callNode, "Expecting ')'");
            operand = std::move(callNode);

        } else if (curLexeme.GetType() == LexemeType::LSquare) {
            Pointer<CallArgumentsNode> args = ParseArguments(LexemeType::RSquare);
            const ITypeSymbol* resType = myTable->GetUnresolvedSymbol();

            IdentifierNode* identifier = dynamic_cast<IdentifierNode*>(operand.get());
            if (identifier != nullptr) {
                std::vector<const ITypeSymbol*> argsTypes;
                for (auto& arg : args->GetArguments()) {
                    argsTypes.push_back(arg->GetType());
                }

                identifier->TryResolveArray(argsTypes);
                resType = identifier->GetType();
            }

            Pointer<IndexSuffixNode> indexNode = std::make_unique<IndexSuffixNode>(std::move(operand), resType);

            if (args->GetArguments().empty()) {
                AddParsingError(curLexeme, "Expecting an index");
            }
            indexNode->SetArguments(std::move(args));

            ConsumeLexeme(LexemeType::RSquare, *indexNode, "Expecting ']'");
            operand = std::move(indexNode);
        }

        curLexeme = myLexer.GetLexeme();
    }

    return operand;
}

Pointer<CallArgumentsNode> Parser::ParseArguments(LexemeType rParen) {
    Pointer<CallArgumentsNode> arguments = std::make_unique<CallArgumentsNode>();
    while (myLexer.GetLexeme().GetType() != rParen && myLexer.GetLexeme().GetType() != LexemeType::EndOfFile) {
        arguments->AddArgument(ParseExpression());

        if (myLexer.GetLexeme().GetType() != LexemeType::OpComma) {
            if (myLexer.GetLexeme().GetType() == rParen || myLexer.GetLexeme().GetType() == LexemeType::EndOfFile) {
                break;
            }

            AddParsingError(myLexer.GetLexeme(), "Expecting ','");
        } else {
            myLexer.NextLexeme();
        }
    }

    return arguments;
}

// Identifier | Number | String | '(' LeftAssociative(0) ')' | ifExpression | jumpExpression
Pointer<ITypedNode> Parser::ParsePrimary() {
    const Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() == LexemeType::Keyword) {
        std::string keyword = curLexeme.GetValue<std::string>();
        if (keyword == "true" || keyword == "false") {
            Pointer<BooleanNode> node = CreateLexemeNode<BooleanNode>(myLexer.NextLexeme(), myTable->GetType(BooleanSymbol().GetName()));
            return node;
        }
        if (keyword == "if") {
            Pointer<IfExpression> ifExpr = ParseIfExpression();
            if (ifExpr != nullptr) {
                if (!ifExpr->HasIfBody() || !ifExpr->HasElseBody()) {
                    AddParsingError(myLexer.GetLexeme(), "'if' must have both main and 'else' branches if used as an expression");
                }
            }

            return ifExpr;
        }
    }

    myLexer.NextLexeme();
    if (curLexeme.GetType() == LexemeType::LParen) {
        Pointer<ITypedNode> expr = ParseExpression();
        ConsumeLexeme(LexemeType::RParen, *expr, "Expecting ')'");
        return expr;
    }
    if (curLexeme.GetType() == LexemeType::Identifier) {
        Pointer<IdentifierNode> node = CreateLexemeNode(curLexeme,
            myTable->GetUnresolvedSymbol(), myTable->GetSymbols(curLexeme.GetValue<std::string>()));
        return node;
    }
    if (LexerUtils::IsIntegerType(curLexeme.GetType())) {
        Pointer<IntegerNode> node = CreateLexemeNode<IntegerNode>(curLexeme, myTable->GetType(IntegerSymbol().GetName()));
        return node;
    }
    if (LexerUtils::IsRealType(curLexeme.GetType())) {
        Pointer<DoubleNode> node = CreateLexemeNode<DoubleNode>(curLexeme, myTable->GetType(DoubleSymbol().GetName()));
        return node;
    }
    if (curLexeme.GetType() == LexemeType::String || curLexeme.GetType() == LexemeType::RawString) {
        Pointer<StringNode> node = CreateLexemeNode<StringNode>(curLexeme, myTable->GetType(StringSymbol().GetName()));
        return node;
    }

    AddParsingError(curLexeme, "Unexpected lexeme");
    return CreateEmptyIdentifier(curLexeme);
}

// 'if' '(' expression ')' (controlStructureBody | (controlStructureBody? ';'? 'else' (controlStructureBody | ';')) | ';')
Pointer<IfExpression> Parser::ParseIfExpression() {
    myLexer.NextLexeme();
    // TODO: make it work
    Pointer<IfExpression> ifExpr = std::make_unique<IfExpression>(myTable->GetUnitSymbol());

    ConsumeLexeme(LexemeType::LParen, *ifExpr, "Expecting '('");
    ifExpr->SetExpression(ParseExpression());
    ConsumeLexeme(LexemeType::RParen, *ifExpr, "Expecting ')'");

    if (myLexer.GetLexeme().GetType() == LexemeType::Keyword && myLexer.GetLexeme().GetValue<std::string>() == "else") {
        ifExpr->SetIfBody(std::make_unique<EmptyStatement>(myTable->GetUnitSymbol()));
        myLexer.NextLexeme();
        ifExpr->SetElseBody(ParseControlStructureBody(true));
        return ifExpr;
    }

    ifExpr->SetIfBody(ParseControlStructureBody(true));

    if (myLexer.GetLexeme().GetType() == LexemeType::Keyword && myLexer.GetLexeme().GetValue<std::string>() == "else") {
        myLexer.NextLexeme();
        ifExpr->SetElseBody(ParseControlStructureBody(true));
    } else {
        ifExpr->SetElseBody(std::make_unique<EmptyStatement>(myTable->GetUnitSymbol()));
    }

    return ifExpr;
}

Pointer<IdentifierNode> Parser::CreateEmptyIdentifier(const Lexeme& lexeme) {
    return std::make_unique<IdentifierNode>(lexeme.CopyEmptyOfType(LexemeType::Error), myTable->GetUnresolvedSymbol(), std::vector<const ISymbol*>());
}

void Parser::AddParsingError(const Lexeme& location, const std::string& error) {
    myParsingErrors.emplace_back(location, myTable->GetUnresolvedSymbol(), error);
}

void Parser::AddSemanticsError(const Lexeme& location, const std::string& error) {
    mySemanticsErrors.emplace_back(location, myTable->GetUnresolvedSymbol(), error);
}

bool Parser::ConsumeLexeme(LexemeType lexemeType, ISyntaxNode& host, const std::string& error) {
    if (myLexer.GetLexeme().GetType() != lexemeType) {
        AddParsingError(myLexer.GetLexeme(), error);
        return false;
    }

    myLexer.NextLexeme();
    return true;
}

bool Parser::ConsumeLexeme(LexemeType lexemeType, const std::string& text, ISyntaxNode& host,
        const std::string& error) {
    if (myLexer.GetLexeme().GetText() != text) {
        AddParsingError(myLexer.GetLexeme(), error);
        return false;
    }

    return ConsumeLexeme(lexemeType, host, error);
}

void Parser::ConsumeSemicolons() {
    while (myLexer.GetLexeme().GetType() == LexemeType::OpSemicolon) {
        myLexer.NextLexeme();
    }
}
