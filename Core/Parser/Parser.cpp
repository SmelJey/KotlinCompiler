#include "Parser.h"

#include "ExpressionNodes.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"
#include "StatementNodes.h"
#include "Semantics/ClassSymbol.h"
#include "Semantics/FunctionSymbol.h"
#include "ParserError.h"
#include "Semantics/SymbolsFrame.h"

Parser::Parser(Lexer& lexer, SymbolTable* symbolTable) : myLexer(lexer), myRootTable(symbolTable), myTable(symbolTable) {
    myLexer.NextLexeme();
}

const Lexer& Parser::GetLexer() const {
    return myLexer;
}

const SymbolTable& Parser::GetSymbolTable() const {
    return *myRootTable;
}

Pointer<DeclarationBlock> Parser::Parse() {
    return ParseDeclarations(false);
}

const std::vector<ParserError>& Parser::GetParsingErrors() const {
    return myParsingErrors;
}

const std::vector<ParserError>& Parser::GetSemanticsErrors() const {
    return mySemanticsErrors;
}

// (classDeclaration | functionDeclaration | propertyDeclaration semis?)*
Pointer<DeclarationBlock> Parser::ParseDeclarations(bool isClass) {
    ConsumeSemicolons();
    Lexeme curLexeme = myLexer.GetLexeme();
    Pointer<DeclarationBlock> declarations = std::make_unique<DeclarationBlock>(curLexeme);

    while (curLexeme.GetType() != LexemeType::EndOfFile && !(curLexeme.GetType() == LexemeType::RCurl && isClass)) {
        if (RequireLexeme(LexemeType::Keyword,
                          (isClass ? "Expecting member declaration" : "Expecting a top level declaration"))
        ) {
            std::string keyword = curLexeme.GetValue<std::string>();

            if (keyword == "class") {
                if (isClass) {
                    AddParsingError(myLexer.GetLexeme(), "Class is not allowed here");
                } else {
                    declarations->AddDeclaration(ParseClass());
                }
            } else if (keyword == "fun") {
                declarations->AddDeclaration(ParseFunction());
            } else if (keyword == "var" || keyword == "val") {
                declarations->AddDeclaration(ParseProperty(curLexeme));
            } else {
                AddParsingError(curLexeme, "Unsupported keyword");
            }
        } else {
            myLexer.NextLexeme();
        }

        ConsumeSemicolons();
        curLexeme = myLexer.GetLexeme();
    }

    return declarations;
}

// 'class' simpleIdentifier classBody?
Pointer<ClassDeclaration> Parser::ParseClass() {
    Pointer<ClassDeclaration> classDecl = std::make_unique<ClassDeclaration>(
        ParseIdentifier("Class declaration must have a name"), myRootTable->GetUnitSymbol());
    SymbolsFrame frame(&myTable);

    if (AcceptLexeme(LexemeType::LCurl)) {
        classDecl->SetBody(ParseDeclarations(true));
        RequireLexeme(LexemeType::RCurl, "Expecting '}'");
    }

    Pointer<ClassSymbol> classSym = std::make_unique<ClassSymbol>(classDecl->GetIdentifierName(), frame.Dispose());
    const ISymbol* sym = myTable->Add(std::move(classSym));
    if (*sym == UnresolvedSymbol(myRootTable)) {
        AddSemanticsError(classDecl->GetIdentifier().GetLexeme(), "Conflicting declarations: " + classDecl->GetIdentifierName());
    } else {
        Pointer<FunctionSymbol> constructor = std::make_unique<FunctionSymbol>(classDecl->GetIdentifierName(),
                                                                               dynamic_cast<const ClassSymbol*>(sym), std::vector<const AbstractType*>(), std::make_unique<SymbolTable>(myTable), classDecl.get());
        myTable->Add(std::move(constructor));
    }

    classDecl->SetSymbol(sym);
    return classDecl;
}

// 'fun' simpleIdentifier functionValueParameters (':' type)? functionBody
Pointer<FunctionDeclaration> Parser::ParseFunction() {
    Pointer<IdentifierNode> identifier = ParseIdentifier("Function declaration must have a name");
    SymbolsFrame tableFrame(&myTable);

    Pointer<ParameterList> paramsNode = ParseParameters();
    Pointer<TypeNode> returnNode;

    const AbstractType* returnType = myRootTable->GetUnitSymbol();
    myReturns.push(nullptr);
    if (AcceptLexeme(LexemeType::OpColon)) {
        returnNode = ParseType();
        returnType = returnNode->GetType();
        myReturns.top() = returnNode->GetType();
    }

    Pointer<FunctionDeclaration> functionDecl = std::make_unique<FunctionDeclaration>(std::move(identifier), myRootTable->GetUnitSymbol(), std::move(paramsNode));
    functionDecl->SetReturn(std::move(returnNode));

    std::vector<const AbstractType*> paramsTypes;
    for (auto& it : functionDecl->GetParameters().GetParameters()) {
        paramsTypes.push_back(it->GetTypeNode().GetType());
    }

    Pointer<FunctionSymbol> funcSym = std::make_unique<FunctionSymbol>(functionDecl->GetIdentifierName(), returnType,
                                                                       paramsTypes, functionDecl.get());

    ISymbol* sym = myTable->GetParent()->Add(std::move(funcSym));
    CheckUnresolvedType(sym, "Conflicting function overloads: " + functionDecl->GetIdentifierName(), functionDecl->GetIdentifier().GetLexeme());
    functionDecl->SetSymbol(sym);

    if (AcceptLexeme(LexemeType::OpAssign)) {
        functionDecl->SetBody(ParseExpression());
        myReturns.top() = functionDecl->GetBody().GetType();
    } else {
        functionDecl->SetBody(ParseBlock());
    }

    if (myReturns.top() == nullptr) {
        myReturns.top() = myRootTable->GetUnitSymbol();
    }

    if (*myReturns.top() != *returnType) {
        AddSemanticsError(functionDecl->GetBody().GetLexeme(), myReturns.top()->GetName() + " does not conform to the expected type " + returnType->GetName());
    }

    if (dynamic_cast<FunctionSymbol*>(sym)) {
        dynamic_cast<FunctionSymbol*>(sym)->SetTable(tableFrame.Dispose());
    } else {
        tableFrame.Dispose();
    }

    myReturns.pop();
    return functionDecl;
}

// '(' (functionValueParameter (',' functionValueParameter)* ','?)? ')'
Pointer<ParameterList> Parser::ParseParameters() {
    Pointer<ParameterList> params = std::make_unique<ParameterList>(myLexer.GetLexeme());
    RequireLexeme(LexemeType::LParen, "Expecting '('");

    while (myLexer.GetLexeme().GetType() != LexemeType::RParen && myLexer.GetLexeme().GetType() != LexemeType::EndOfFile) {
        params->AddParameter(std::move(ParseParameter()));

        if (!AcceptLexeme(LexemeType::OpComma)) {
            if (myLexer.GetLexeme().GetType() == LexemeType::RParen || myLexer.GetLexeme().GetType() == LexemeType::EndOfFile) {
                break;
            }

            AddParsingError(myLexer.GetLexeme(), "Expecting ','");
        }
    }

    RequireLexeme(LexemeType::RParen, "Expecting ')'");
    return params;
}

// simpleIdentifier ':' type ('=' expression)? 
Pointer<ParameterNode> Parser::ParseParameter() {
    Lexeme curLexeme = myLexer.GetLexeme();
    Pointer<IdentifierNode> identifier = ParseIdentifier("Parameter name expected");
    bool isError = curLexeme.GetType() != LexemeType::Identifier;

    Pointer<TypeNode> typeNode = std::make_unique<TypeNode>(curLexeme.CopyEmptyOfType(LexemeType::Identifier), myRootTable->GetUnresolvedSymbol());
    curLexeme = myLexer.GetLexeme();

    if (RequireLexeme(LexemeType::OpColon, "A type annotation is required on a value parameter")) {
        typeNode = ParseType();
    } else {
        if (isError) {
            myLexer.NextLexeme();
        }
    }

    Pointer<VariableSymbol> paramSymbol = std::make_unique<VariableSymbol>(identifier->GetIdentifier(), typeNode->GetType(), false);
    const ISymbol* resSym = myTable->Add(std::move(paramSymbol));
    CheckUnresolvedType(resSym, "Conflicting declarations: " + identifier->GetIdentifier(), curLexeme);

    Pointer<ParameterNode> param = std::make_unique<ParameterNode>(std::move(identifier), myRootTable->GetUnitSymbol(), std::move(typeNode));
    param->SetSymbol(resSym);

    if (AcceptLexeme(LexemeType::OpAssign)) {
        param->SetDefault(ParseExpression());
    }

    return param;
}

// ( ('(' type ')') | (simpleIdentifier ('.' simpleIdentifier)*) )
Pointer<TypeNode> Parser::ParseType() {
    if (AcceptLexeme(LexemeType::LParen)) {
        Pointer<TypeNode> typeNode = ParseType();
        RequireLexeme(LexemeType::RParen, "Expecting ')'");
        return typeNode;
    }

    Lexeme curLexeme = myLexer.GetLexeme();
    if (!RequireLexeme(LexemeType::Identifier, "Type expected")) {
        return std::make_unique<TypeNode>(curLexeme.CopyEmptyOfType(LexemeType::Identifier), myRootTable->GetUnresolvedSymbol());
    }

    std::string typeName = curLexeme.GetValue<std::string>();
    if (AcceptLexeme(LexemeType::OpLess)) {
        Pointer<TypeArgumentsNode> typeArgs = ParseTypeArguments();
        if (typeName != "Array") {
            AddSemanticsError(typeArgs->GetLexeme(), "Type arguments currently are only supported for arrays");
        }
        if (typeArgs->GetArguments().size() == 1) {
            const ArraySymbol* arrSym = ParserUtils::GetGenericArray(typeArgs->GetArguments()[0]->GetType(), myRootTable);
            Pointer<TypeNode> typeNode = std::make_unique<TypeNode>(curLexeme, arrSym);
            typeNode->SetTypeArgs(std::move(typeArgs));
            return typeNode;
        }

        typeName.append("<>");
    }

    return std::make_unique<TypeNode>(curLexeme, myTable->GetType(typeName));
}

Pointer<IAnnotatedNode> Parser::ParseStatement() {
    Lexeme curLexeme = myLexer.GetLexeme();
    wasError = false;

    if (AcceptLexeme(LexemeType::Keyword, "val") || AcceptLexeme(LexemeType::Keyword, "var")) {
        return ParseProperty(curLexeme);
    }
    if (AcceptLexeme(LexemeType::Keyword, "fun")) {
        return ParseFunction();
    }
    if (AcceptLexeme(LexemeType::Keyword, "class")) {
        return ParseClass();
    }
    if (AcceptLexeme(LexemeType::Keyword, "for")) {
        return ParseForLoop(curLexeme);
    } 
    if (AcceptLexeme(LexemeType::Keyword, "while")) {
        return ParseWhileLoop(curLexeme);
    } 
    if (AcceptLexeme(LexemeType::Keyword, "do")) {
        return ParseDoWhileLoop(curLexeme);
    }
    if (AcceptLexeme(LexemeType::Keyword, "if")) {
        return ParseIfExpression(curLexeme);
    }
    if (AcceptLexeme(LexemeType::Keyword, "break")) {
        if (inLoop == 0) {
            AddParsingError(curLexeme, "'break' and 'continue' are only allowed inside loop");
        }
        return CreateLexemeNode<BreakNode, UnitTypeSymbol>(curLexeme, myRootTable->GetUnitSymbol());
    }
    if (AcceptLexeme(LexemeType::Keyword, "continue")) {
        if (inLoop == 0) {
            AddParsingError(curLexeme, "'break' and 'continue' are only allowed inside loop");
        }
        return CreateLexemeNode<ContinueNode, UnitTypeSymbol>(curLexeme, myRootTable->GetUnitSymbol());
    }
    if (AcceptLexeme(LexemeType::Keyword, "return")) {
        Pointer<ReturnNode> returnNode = CreateLexemeNode<ReturnNode, UnitTypeSymbol>(curLexeme, myRootTable->GetUnitSymbol());
        Lexeme nextLexeme = myLexer.GetLexeme();
        if (nextLexeme.GetRow() == curLexeme.GetRow() && nextLexeme.GetType() != LexemeType::RCurl
            && nextLexeme.GetType() != LexemeType::EndOfFile && nextLexeme.GetType() != LexemeType::OpSemicolon) {
            returnNode->SetExpression(ParseExpression());
        } else {
            returnNode->SetExpression(CreateEmptyStatement(myLexer.GetLexeme()));
        }

        if (myReturns.empty()) {
            AddParsingError(returnNode->GetLexeme(), "'return' is not allowed here");
        } else if (myReturns.top() != nullptr && *myReturns.top() != *returnNode->GetType()) {
            AddSemanticsError(returnNode->GetLexeme(),
                returnNode->GetType()->GetName() + " does not conform to the expected type " + myReturns.top()->GetName());
        } else {
            myReturns.top() = returnNode->GetType();
        }
        
        return returnNode;
    }
    if (AcceptLexeme(LexemeType::Keyword)) {
        AddParsingError(curLexeme, "Unsupported keyword");
        return CreateEmptyStatement(curLexeme);
    }

    Pointer<IAnnotatedNode> res = ParseAssignment();
    CheckUnresolvedType(res->GetType(), "Unresolved symbol", curLexeme);
    return res;
}

// '{' (statement (semis statement)*)? semis? '}'
Pointer<BlockNode> Parser::ParseBlock() {
    Pointer<BlockNode> blockNode = std::make_unique<BlockNode>(myLexer.GetLexeme(), myRootTable->GetUnitSymbol());
    RequireLexeme(LexemeType::LCurl, "Expecting '{'");
    ConsumeSemicolons();

    while (myLexer.GetLexeme().GetType() != LexemeType::EndOfFile && myLexer.GetLexeme().GetType() != LexemeType::RCurl) {
        blockNode->AddStatement(ParseStatement());
        ConsumeSemicolons();
    }

    RequireLexeme(LexemeType::RCurl, "Expecting '}'");
    return blockNode;
}

Pointer<IAnnotatedNode> Parser::ParseControlStructureBody(bool acceptSemicolons = false) {
    if (myLexer.GetLexeme().GetType() == LexemeType::LCurl) {
        return ParseBlock();
    }
    if (acceptSemicolons && AcceptLexeme(LexemeType::OpSemicolon)) {
        return CreateEmptyStatement(myLexer.GetLexeme());
    }

    return ParseStatement();
}

// 'for' '(' (variableDeclaration | multiVariableDeclaration) 'in' expression ')' controlStructureBody?
Pointer<ForNode> Parser::ParseForLoop(const Lexeme& lexeme) {
    inLoop++;
    SymbolsFrame forFrame(&myTable);

    RequireLexeme(LexemeType::LParen, "Expecting '(' to open a loop range");
    Pointer<VariableNode> variable = ParseVariable();

    RequireLexeme(LexemeType::OpIn, "Expecting 'in'");
    Pointer<IAnnotatedNode> expr = ParseExpression();
    Pointer<ISyntaxNode> body;

    const IterableSymbol* iterable = dynamic_cast<const IterableSymbol*>(expr->GetType());
    if (iterable == nullptr) {
        AddSemanticsError(expr->GetLexeme(), "Array or range expected in for loop");
    }

    const AbstractType* underlyingType = (iterable == nullptr ? myRootTable->GetUnresolvedSymbol() : iterable->GetType());
    variable->SetSymbol(myTable->Add(std::make_unique<VariableSymbol>(variable->GetIdentifierName(), underlyingType, false)));

    {
        SymbolsFrame tableFrame(&myTable);

        RequireLexeme(LexemeType::RParen, "Expecting ')'");
        body = ParseControlStructureBody();
    }

    inLoop--;
    return std::make_unique<ForNode>(lexeme, std::move(expr), std::move(body), std::move(variable), myRootTable->GetUnitSymbol());
}

// 'while' '(' expression ')' (controlStructureBody | ';')
Pointer<WhileNode> Parser::ParseWhileLoop(const Lexeme& lexeme) {
    inLoop++;
    SymbolsFrame tableFrame(&myTable);

    RequireLexeme(LexemeType::LParen, "Expecting '('");
    Pointer<IAnnotatedNode> expr = ParseExpression();
    CheckType<BooleanSymbol>(expr->GetType(), expr->GetLexeme());
    RequireLexeme(LexemeType::RParen, "Expecting ')'");
    Pointer<ISyntaxNode> body = ParseControlStructureBody(true);

    inLoop--;
    return std::make_unique<WhileNode>(lexeme, std::move(expr), std::move(body), myRootTable->GetUnitSymbol());
}

// 'do' controlStructureBody 'while' '(' expression ')'
Pointer<DoWhileNode> Parser::ParseDoWhileLoop(const Lexeme& lexeme) {
    inLoop++;
    SymbolsFrame tableFrame(&myTable);

    Pointer<ISyntaxNode> body = ParseControlStructureBody();
    RequireLexeme(LexemeType::Keyword, "while", "Expecting 'while' followed by a post-condition");
    RequireLexeme(LexemeType::LParen, "Expecting '('");

    Pointer<IAnnotatedNode> expr = ParseExpression();
    CheckType<BooleanSymbol>(expr->GetType(), expr->GetLexeme());
    RequireLexeme(LexemeType::RParen, "Expecting ')'");

    inLoop--;
    return std::make_unique<DoWhileNode>(lexeme, std::move(expr), std::move(body), myRootTable->GetUnitSymbol());
}

// (directlyAssignableExpression assignmentAndOperator)? expression
Pointer<IAnnotatedNode> Parser::ParseAssignment() {
    Pointer<IAnnotatedNode> assignable = ParseExpression();

    Lexeme curLexeme = myLexer.GetLexeme();
    if (ParserUtils::AssignmentOperations.count(curLexeme.GetType())) {
        curLexeme = myLexer.NextLexeme();
        Pointer<IAnnotatedNode> expression = ParseExpression();
        
        if (!ParserUtils::IsDirectlyAssignable(assignable.get())) {
            AddParsingError(curLexeme, "Variable expected");
        } else if (!assignable->IsAssignable()) {
            AddSemanticsError(curLexeme, "Val cannot be reassigned");
        } else {
            auto rightType = expression->GetType();
            auto leftType = assignable->GetType();
            if (!leftType->IsAssignable(curLexeme.GetType(), rightType)) {
                AddSemanticsError(curLexeme, rightType->GetName() + " does not conform to the exprected type " + leftType->GetName());
            }
        }

        Pointer<Assignment> assignment = std::make_unique<Assignment>(curLexeme, std::move(assignable), std::move(expression), myRootTable->GetUnitSymbol());
        return assignment;
    }

    return assignable;
}

// ('val' | 'var') variableDeclaration ('=' expression)? ';'?
Pointer<PropertyDeclaration> Parser::ParseProperty(const Lexeme& keyword) {
    Pointer<IdentifierNode> identifier = ParseIdentifier("Expecting property name");
    Pointer<TypeNode> typeNode;

    if (AcceptLexeme(LexemeType::OpColon)) {
        typeNode = ParseType();
    }

    Pointer<IAnnotatedNode> initExpr;
    bool isFailed = false;
    if (!RequireLexeme(LexemeType::OpAssign, "Property must be initialized")) {
        isFailed = true;
        initExpr = CreateEmptyIdentifier(myLexer.GetLexeme());
    } else {
        initExpr = ParseExpression();
    }

    if (!isFailed && typeNode != nullptr && initExpr != nullptr && *typeNode->GetType() != *initExpr->GetType()) {
        AddSemanticsError(myLexer.GetLexeme(), initExpr->GetSymbol()->GetName()
                          + " does not conform to the expected type " + typeNode->GetSymbol()->GetName());
    }

    Pointer<PropertyDeclaration> propertyDecl = std::make_unique<PropertyDeclaration>(std::move(identifier), myRootTable->GetUnitSymbol(), keyword);
    propertyDecl->SetTypeNode(std::move(typeNode));
    propertyDecl->SetInitialization(std::move(initExpr));

    auto propertySym = std::make_unique<VariableSymbol>(propertyDecl->GetIdentifierName(),
        myTable->GetType(propertyDecl->GetInitialization().GetType()->GetName()), propertyDecl->IsMutable());

    const ISymbol* sym = myTable->Add(std::move(propertySym));
    CheckUnresolvedType(sym, "Conflicting declarations: " + propertyDecl->GetIdentifierName(), propertyDecl->GetIdentifier().GetLexeme());

    propertyDecl->SetSymbol(sym);
    return propertyDecl;
}

// simpleIdentifier (':' type)?
Pointer<VariableNode> Parser::ParseVariable() {
    Pointer<VariableNode> variable = std::make_unique<VariableNode>(ParseIdentifier("Expecting a variable name"), myRootTable->GetUnitSymbol());
    if (AcceptLexeme(LexemeType::OpColon)) {;
        variable->SetTypeNode(ParseType());
        if (*variable->GetTypeNode().GetType() != IntegerSymbol(myRootTable)) {
            AddParsingError(myLexer.GetLexeme(), "The loop iterates over values of type Int but the parameter is declared to be " + variable->GetTypeNode().GetType()->GetName());
        }
    }

    return variable;
}

Pointer<IdentifierNode> Parser::ParseIdentifier(const std::string& errorMessage) {
    Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() != LexemeType::Identifier) {
        curLexeme = curLexeme.CopyEmptyOfType(LexemeType::Identifier);
    }

    Pointer<IdentifierNode> identifier = CreateLexemeNode(curLexeme, myRootTable->GetUnresolvedSymbol(), myTable->GetSymbols(curLexeme.GetText()));
    RequireLexeme(LexemeType::Identifier, errorMessage);
    return identifier;
}

// LeftAssociative(0)
Pointer<IAnnotatedNode> Parser::ParseExpression() {
    return ParseLeftAssociative(0);
}

// LeftAssociative(n+1) | LeftAssociative(n+1) operator(n) LeftAssociative(n+1)
Pointer<IAnnotatedNode> Parser::ParseLeftAssociative(size_t priority) {
    if (priority == ParserUtils::OperationsPriority.size()) {
        return ParsePrefix();
    }

    Pointer<IAnnotatedNode> leftOperand = ParseLeftAssociative(priority + 1);
    Lexeme operation = myLexer.GetLexeme();

    while (ParserUtils::OperationsPriority.at(priority).count(operation.GetType())
           // infixFunctionCall 
           || (priority == 5 && operation.GetType() == LexemeType::Identifier)) {
        int lastRow = myLexer.PrevLexeme().GetRow();
        if (operation.GetRow() != lastRow) {
            break;
        }

        myLexer.NextLexeme();
        Pointer<IAnnotatedNode> rightOperand = ParseLeftAssociative(priority + 1);

        const AbstractType* leftType = leftOperand->GetType();
        const AbstractType* rightType = rightOperand->GetType();
        Pointer<AbstractType> resultType = std::make_unique<UnresolvedSymbol>(myRootTable);

        if (leftType != nullptr && rightType != nullptr) {
            resultType = IsApplicable(operation.GetType(), leftType, rightType);
        }

        std::string typeName = resultType->GetName();

        if (dynamic_cast<ArraySymbol*>(resultType.get()) != nullptr || dynamic_cast<RangeSymbol*>(resultType.get()) != nullptr) {
            myRootTable->Add(std::move(resultType));    
        }

        leftOperand = std::make_unique<BinOperationNode>(
            operation, std::move(leftOperand), std::move(rightOperand), myTable->GetType(typeName));
        operation = myLexer.GetLexeme();
    }

    return leftOperand;
}

// prefixUnaryOperator* postfixUnaryExpression
Pointer<IAnnotatedNode> Parser::ParsePrefix() {
    const Lexeme curLexeme = myLexer.GetLexeme();

    if (AcceptLexeme({ LexemeType::OpSub, LexemeType::OpAdd, LexemeType::OpInc, LexemeType::OpDec, LexemeType::OpExclMark })) {
        Pointer<IAnnotatedNode> prefix = ParsePrefix();
        Pointer<AbstractType> resultType = IsApplicable(curLexeme.GetType(), prefix->GetType());
        if ((curLexeme.GetType() == LexemeType::OpInc || curLexeme.GetType() == LexemeType::OpDec) && !prefix->IsAssignable()) {
            AddSemanticsError(curLexeme, "Val cannot be reassigned");
        }

        return std::make_unique<UnaryPrefixOperationNode>(curLexeme, std::move(prefix), myTable->GetType(resultType->GetName()));
    }

    return ParsePostfix();
}

// TODO: REFACTOR
// primaryExpression postfixUnarySuffix*
Pointer<IAnnotatedNode> Parser::ParsePostfix() {
    Pointer<IAnnotatedNode> operand = ParsePrimary();
    Lexeme curLexeme = myLexer.GetLexeme();

    while (ParserUtils::PostfixOperations.count(curLexeme.GetType()) || curLexeme.GetType() == LexemeType::OpLess && ParserUtils::IsGenericIdentifier(operand.get())) {
        // Postfix operation
        if (AcceptLexeme(LexemeType::OpInc) || AcceptLexeme(LexemeType::OpDec)) {
            Pointer<AbstractType> resultType = IsApplicable(curLexeme.GetType(), operand->GetType());
            if (!operand->IsAssignable()) {
                AddSemanticsError(curLexeme, "Val cannot be reassigned");
            }

            operand = std::make_unique<UnaryPostfixOperationNode>(curLexeme, std::move(operand), myTable->GetType(resultType->GetName()));
        } else if (AcceptLexeme(LexemeType::OpDot)) {
            operand = ParseMemberAccess(curLexeme, std::move(operand));

        // Function call (with type args)
        } else if (AcceptLexeme(LexemeType::LParen) || AcceptLexeme(LexemeType::OpLess)) {
            Pointer<TypeArgumentsNode> typeArgs;
            if (curLexeme.GetType() == LexemeType::OpLess) {
                typeArgs = ParseTypeArguments();
                RequireLexeme(LexemeType::LParen, "Type arguments currently are only supported for arrays");
            }

            Pointer<CallArgumentsNode> args = ParseArguments(LexemeType::RParen);
            const AbstractType* resType = myRootTable->GetUnresolvedSymbol();

            IdentifierNode* identifier = dynamic_cast<IdentifierNode*>(operand.get());
            MemberAccessNode* memberAccess = dynamic_cast<MemberAccessNode*>(operand.get());

            if (memberAccess != nullptr) {
                identifier = memberAccess->GetMember();
            }

            if (identifier != nullptr) {
                std::vector<const AbstractType*> argsTypes = args->GetTypes();
                identifier->TryResolveFunc(argsTypes);

                if (typeArgs != nullptr && typeArgs->GetArguments().size() == 1 && identifier->GetLexeme().GetText() == "arrayOf") {
                    const ISymbol* res = ParserUtils::GetArrayBuilder(typeArgs->GetArguments()[0]->GetType(), argsTypes, myRootTable);
                    if (res == nullptr) {
                        AddSemanticsError(myLexer.GetLexeme(),"Arguments do not conform to the expected type " + typeArgs->GetArguments()[0]->GetSymbol()->GetName());
                    } else {
                        identifier->Resolve(res);
                    }
                }

                resType = identifier->GetType();
            }

            Pointer<CallSuffixNode> callNode = std::make_unique<CallSuffixNode>(std::move(operand), std::move(args), resType);
            if (typeArgs != nullptr) {
                callNode->SetTypeArguments(std::move(typeArgs));
            }

            CheckUnresolvedType(resType, "Unresolved function", curLexeme);
            RequireLexeme(LexemeType::RParen, "Expecting ')'");
            operand = std::move(callNode);

        // Index call
        } else if (AcceptLexeme(LexemeType::LSquare)) {
            Pointer<CallArgumentsNode> args = ParseArguments(LexemeType::RSquare);
            const AbstractType* resType = myRootTable->GetUnresolvedSymbol();

            if (args->GetArguments().size() != 1) {
                AddParsingError(curLexeme, "Expecting an index");
            } else {
                auto arrSym = dynamic_cast<const ArraySymbol*>(operand->GetType());
                if (arrSym != nullptr) {
                    std::vector<const AbstractType*> argsTypes = args->GetTypes();

                    if (CheckType<IntegerSymbol>(argsTypes[0], curLexeme)) {
                        resType = arrSym->GetType();
                    }
                }
            }

            Pointer<IndexSuffixNode> indexNode = std::make_unique<IndexSuffixNode>(std::move(operand), std::move(args), resType);
            RequireLexeme(LexemeType::RSquare, "Expecting ']'");
            operand = std::move(indexNode);
        }

        curLexeme = myLexer.GetLexeme();
    }

    return operand;
}

Pointer<MemberAccessNode> Parser::ParseMemberAccess(const Lexeme& operationLexeme, Pointer<IAnnotatedNode> operand) {
    Pointer<IdentifierNode> member = CreateEmptyIdentifier(operationLexeme);
    Lexeme curLexeme = myLexer.GetLexeme();

    if (RequireLexeme(LexemeType::Identifier, "Name expected")) {
        IdentifierNode* identifier = dynamic_cast<IdentifierNode*>(operand.get());
        if (identifier != nullptr) {
            identifier->TryResolveVariable();
        }
        std::vector<const ISymbol*> candidates = operand->GetType()->GetTable()->GetSymbols(curLexeme.GetValue<std::string>());

        Pointer<IdentifierNode> memberIdentifier = CreateLexemeNode(curLexeme, myRootTable->GetUnresolvedSymbol(), candidates);
        memberIdentifier->TryResolveVariable();
        member = std::move(memberIdentifier);
    }

    return std::make_unique<MemberAccessNode>(operationLexeme, std::move(operand), std::move(member));
}

Pointer<TypeArgumentsNode> Parser::ParseTypeArguments() {
    Pointer<TypeArgumentsNode> arguments = std::make_unique<TypeArgumentsNode>(myLexer.GetLexeme());
    while (myLexer.GetLexeme().GetType() != LexemeType::OpGreater && myLexer.GetLexeme().GetType() != LexemeType::EndOfFile) {
        arguments->AddArgument(ParseType());

        if (!AcceptLexeme(LexemeType::OpComma)) {
            if (myLexer.GetLexeme().GetType() == LexemeType::OpGreater || myLexer.GetLexeme().GetType() == LexemeType::EndOfFile) {
                break;
            }

            AddParsingError(myLexer.GetLexeme(), "Expecting ','");
        }
    }

    RequireLexeme(LexemeType::OpGreater, "Expecting '>'");
    return arguments;
}

Pointer<CallArgumentsNode> Parser::ParseArguments(LexemeType rParen) {
    Pointer<CallArgumentsNode> arguments = std::make_unique<CallArgumentsNode>(myLexer.GetLexeme());
    while (myLexer.GetLexeme().GetType() != rParen && myLexer.GetLexeme().GetType() != LexemeType::EndOfFile) {
        arguments->AddArgument(ParseExpression());

        if (!AcceptLexeme(LexemeType::OpComma)) {
            if (myLexer.GetLexeme().GetType() == rParen || myLexer.GetLexeme().GetType() == LexemeType::EndOfFile) {
                break;
            }

            AddParsingError(myLexer.GetLexeme(), "Expecting ','");
        }
    }

    return arguments;
}

// Identifier | Number | String | '(' LeftAssociative(0) ')' | ifExpression | jumpExpression
Pointer<IAnnotatedNode> Parser::ParsePrimary() {
    const Lexeme curLexeme = myLexer.GetLexeme();

    if (AcceptLexeme(LexemeType::Keyword, "true") || AcceptLexeme(LexemeType::Keyword, "false")) {
        Pointer<BooleanNode> node = CreateLexemeNode<BooleanNode>(curLexeme, myTable->GetType(BooleanSymbol(myRootTable).GetName()));
        return node;
    }
    if (AcceptLexeme(LexemeType::Keyword, "if")) {
        Pointer<IfExpression> ifExpr = ParseIfExpression(curLexeme);

        if (*ifExpr->GetType() == *myRootTable->GetUnitSymbol()) {
            AddParsingError(myLexer.GetLexeme(), "'if' must have both main and 'else' branches with the same type if used as an expression");
        }

        return ifExpr;
    }
    if (AcceptLexeme(LexemeType::LParen)) {
        Pointer<IAnnotatedNode> expr = ParseExpression();
        RequireLexeme(LexemeType::RParen, "Expecting ')'");
        return expr;
    }
    if (AcceptLexeme(LexemeType::Identifier)) {
        Pointer<IdentifierNode> node = CreateLexemeNode(curLexeme,
            myRootTable->GetUnresolvedSymbol(), myTable->GetSymbols(curLexeme.GetValue<std::string>()));
        node->TryResolveVariable();
        return node;
    }

    myLexer.NextLexeme();
    if (LexerUtils::IsIntegerType(curLexeme.GetType())) {
        return CreateLexemeNode<IntegerNode>(curLexeme, myTable->GetType(IntegerSymbol(myRootTable).GetName()));;
    }
    if (LexerUtils::IsRealType(curLexeme.GetType())) {
        return CreateLexemeNode<DoubleNode>(curLexeme, myTable->GetType(DoubleSymbol(myRootTable).GetName()));;
    }
    if (curLexeme.GetType() == LexemeType::String || curLexeme.GetType() == LexemeType::RawString) {
        return CreateLexemeNode<StringNode>(curLexeme, myTable->GetType(StringSymbol(myRootTable).GetName()));;
    }

    AddParsingError(curLexeme, "Unexpected lexeme");
    return CreateEmptyIdentifier(curLexeme);
}

// 'if' '(' expression ')' (controlStructureBody | (controlStructureBody? ';'? 'else' (controlStructureBody | ';')) | ';')
Pointer<IfExpression> Parser::ParseIfExpression(const Lexeme& lexeme) {
    RequireLexeme(LexemeType::LParen, "Expecting '('");
    Pointer<IAnnotatedNode> expression = ParseExpression();
    CheckType<BooleanSymbol>(expression->GetType(), expression->GetLexeme());
    RequireLexeme(LexemeType::RParen, "Expecting ')'");

    Pointer<SymbolTable> currentTable = std::make_unique<SymbolTable>(myTable);
    myTable = currentTable.get();

    Pointer<IAnnotatedNode> ifBody = CreateEmptyStatement(myLexer.GetLexeme());
    Pointer<IAnnotatedNode> elseBody = CreateEmptyStatement(myLexer.GetLexeme());

    if (AcceptLexeme(LexemeType::Keyword, "else")) {
        elseBody = ParseControlStructureBody(true);
    } else {
        ifBody = ParseControlStructureBody(true);

        if (AcceptLexeme(LexemeType::Keyword, "else")) {
            elseBody = ParseControlStructureBody(true);
        }
    }

    myTable = myTable->GetParent();
    myTable->Add(std::move(currentTable));

    return std::make_unique<IfExpression>(lexeme, myRootTable->GetUnitSymbol(), std::move(expression), std::move(ifBody), std::move(elseBody));
}

Pointer<IdentifierNode> Parser::CreateEmptyIdentifier(const Lexeme& lexeme) const {
    return std::make_unique<IdentifierNode>(lexeme.CopyEmptyOfType(LexemeType::Error), myRootTable->GetUnresolvedSymbol(), std::vector<const ISymbol*>());
}

void Parser::AddParsingError(const Lexeme& location, const std::string& error) {
    wasError = true;
    myParsingErrors.emplace_back(location, error);
}

void Parser::AddSemanticsError(const Lexeme& location, const std::string& error) {
    if (!wasError) {
        wasError = true;
        mySemanticsErrors.emplace_back(location, error);
    }
}

bool Parser::RequireLexeme(LexemeType lexemeType, const std::string& error) {
    if (myLexer.GetLexeme().GetType() != lexemeType) {
        AddParsingError(myLexer.GetLexeme(), error);
        return false;
    }

    myLexer.NextLexeme();
    return true;
}

bool Parser::RequireLexeme(LexemeType lexemeType, const std::string& text, const std::string& error) {
    if (myLexer.GetLexeme().GetText() != text) {
        AddParsingError(myLexer.GetLexeme(), error);
        return false;
    }

    return RequireLexeme(lexemeType, error);
}

void Parser::ConsumeSemicolons() {
    while (myLexer.GetLexeme().GetType() == LexemeType::OpSemicolon) {
        myLexer.NextLexeme();
    }
}

Pointer<EmptyStatement> Parser::CreateEmptyStatement(const Lexeme& lexeme) {
    return std::make_unique<EmptyStatement>(lexeme, myRootTable->GetUnitSymbol());
}

Pointer<AbstractType> Parser::IsApplicable(LexemeType operation, const AbstractType* left, const AbstractType* right) {
    Pointer<AbstractType> res = left->IsApplicable(operation, right);
    CheckUnresolvedType(res.get(), "Operation is not applicable to types " + left->GetName() + " and " + right->GetName(), myLexer.GetLexeme());
    return res;
}

Pointer<AbstractType> Parser::IsApplicable(LexemeType operation, const AbstractType* left) {
    Pointer<AbstractType> res = left->IsApplicable(operation);
    CheckUnresolvedType(res.get(), "Operation is not applicable to type " + left->GetName(), myLexer.GetLexeme());
    return res;
}

const ISymbol* Parser::CheckUnresolvedType(const ISymbol* symbol, const std::string& error, const Lexeme& lexeme) {
    if (*symbol == UnresolvedSymbol(myRootTable)) {
        AddSemanticsError(lexeme, error);
    }
    return symbol;
}

bool Parser::AcceptLexeme(LexemeType lexType) {
    if (myLexer.GetLexeme().GetType() == lexType) {
        myLexer.NextLexeme();
        return true;
    }
    return false;
}

bool Parser::AcceptLexeme(LexemeType lexType, const std::string& text) {
    if (myLexer.GetLexeme().GetType() == lexType && myLexer.GetLexeme().GetText() == text) {
        myLexer.NextLexeme();
        return true;
    }
    return false;
}

bool Parser::AcceptLexeme(std::initializer_list<LexemeType> lexTypes) {
    if (std::find(lexTypes.begin(), lexTypes.end(), myLexer.GetLexeme().GetType()) != lexTypes.end()) {
        myLexer.NextLexeme();
        return true;
    }
    return false;
}
