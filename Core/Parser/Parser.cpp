#include "Parser.h"

#include "ExpressionNodes.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"
#include "StatementNodes.h"
#include "Semantics/ClassSymbol.h"
#include "Semantics/FunctionSymbol.h"
#include "ParserError.h"
#include "Semantics/SemanticsChecker.h"

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

    Pointer<ClassDeclaration> classDecl = std::make_unique<ClassDeclaration>(ParseIdentifier("Class declaration must have a name"));

    if (myLexer.GetLexeme().GetType() != LexemeType::LCurl) {
        return classDecl;
    }

    myLexer.NextLexeme();

    classDecl->SetBody(ParseDeclarations(true));
    ConsumeLexeme(LexemeType::RCurl, "Expecting '}'");
    return classDecl;
}

// 'fun' simpleIdentifier functionValueParameters (':' type)? functionBody
Pointer<FunctionDeclaration> Parser::ParseFunction() {
    myLexer.NextLexeme();

    Pointer<IdentifierNode> identifier = ParseIdentifier("Function declaration must have a name");
    Pointer<SymbolTable> functionSymbols = std::make_unique<SymbolTable>(myTable);
    myTable = functionSymbols.get();

    Pointer<ParameterList> paramsNode = ParseParameters();
    Pointer<TypeNode> returnNode;
    
    if (myLexer.GetLexeme().GetType() == LexemeType::OpColon) {
        myLexer.NextLexeme();
        returnNode = ParseType();
    }

    Pointer<IAnnotatedNode> body;

    if (myLexer.GetLexeme().GetType() == LexemeType::OpAssign) {
        myLexer.NextLexeme();
        body = ParseExpression();
    } else {
        body = ParseBlock();
    }

    Pointer<FunctionDeclaration> functionDecl = std::make_unique<FunctionDeclaration>(std::move(identifier), std::move(paramsNode), std::move(body));
    functionDecl->SetReturn(std::move(returnNode));

    myTable = functionSymbols->GetParent();

    const ITypeSymbol* returnType = functionDecl->GetBody().GetType();
    if (functionDecl->HasReturnNode()) {
        const ITypeSymbol* functionReturn = functionDecl->GetReturn().GetType();
        if (*functionReturn != *returnType) {
            AddSemanticsError(myLexer.GetLexeme(), returnType->GetName() + " does not conform to the expected type " + functionReturn->GetName());
        }

        returnType = functionReturn;
    }

    std::vector<const ITypeSymbol*> paramsTypes;
    for (auto& it : functionDecl->GetParameters().GetParameters()) {
        paramsTypes.push_back(it->GetTypeNode().GetType());
    }

    Pointer<FunctionSymbol> funcSym = std::make_unique<FunctionSymbol>(functionDecl->GetIdentifierName(), returnType,
        paramsTypes, std::move(functionSymbols));

    const ISymbol* sym = myTable->Add(std::move(funcSym));
    if (*sym == UnresolvedSymbol()) {
        AddSemanticsError(myLexer.GetLexeme(), "Conflicting function overloads: " + functionDecl->GetIdentifierName());
    }

    functionDecl->SetSymbol(sym);
    functionDecl->GetIdentifier().Resolve(sym);
    return functionDecl;
}

// '(' (functionValueParameter (',' functionValueParameter)* ','?)? ')'
Pointer<ParameterList> Parser::ParseParameters() {
    Pointer<ParameterList> params = std::make_unique<ParameterList>(myLexer.GetLexeme());
    ConsumeLexeme(LexemeType::LParen, "Expecting '('");

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

    ConsumeLexeme(LexemeType::RParen, "Expecting ')'");
    return params;
}

// simpleIdentifier ':' type ('=' expression)? 
Pointer<ParameterNode> Parser::ParseParameter() {
    Lexeme curLexeme = myLexer.GetLexeme();
    Pointer<IdentifierNode> identifier = ParseIdentifier("Parameter name expected");
    bool isError = curLexeme.GetType() != LexemeType::Identifier;

    Pointer<TypeNode> typeNode = std::make_unique<TypeNode>(curLexeme.CopyEmptyOfType(LexemeType::Identifier), myRootTable->GetUnresolvedSymbol());
    curLexeme = myLexer.GetLexeme();

    if (curLexeme.GetType() != LexemeType::OpColon) {
        AddParsingError(curLexeme, "A type annotation is required on a value parameter");
        if (isError) {
            myLexer.NextLexeme();
        }
    } else {
        myLexer.NextLexeme();
        typeNode = ParseType();
    }

    Pointer<VariableSymbol> paramSymbol = std::make_unique<VariableSymbol>(identifier->GetIdentifier(), typeNode->GetType(), false);
    const ISymbol* resSym = myTable->Add(std::move(paramSymbol));
    if (*resSym == UnresolvedSymbol()) {
        AddSemanticsError(curLexeme, "Conflicting declarations: " + identifier->GetIdentifier());
    }

    identifier->Resolve(resSym);

    Pointer<ParameterNode> param = std::make_unique<ParameterNode>(std::move(identifier), std::move(typeNode));
    param->SetSymbol(resSym);

    if (myLexer.GetLexeme().GetType() == LexemeType::OpAssign) {
        myLexer.NextLexeme();
        param->SetDefault(ParseExpression());
    }

    return param;
}

// ( ('(' type ')') | (simpleIdentifier ('.' simpleIdentifier)*) )
Pointer<TypeNode> Parser::ParseType() {
    if (myLexer.GetLexeme().GetType() == LexemeType::LParen) {
        myLexer.NextLexeme();
        Pointer<TypeNode> typeNode = ParseType();
        ConsumeLexeme(LexemeType::RParen, "Expecting ')'");

        return typeNode;
    }

    Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() != LexemeType::Identifier) {
        AddParsingError(curLexeme, "Type expected");
        return std::make_unique<TypeNode>(curLexeme.CopyEmpty(), myRootTable->GetUnresolvedSymbol());
    }

    myLexer.NextLexeme();
    return std::make_unique<TypeNode>(curLexeme, GetType(curLexeme.GetValue<std::string>()));
}

Pointer<ISyntaxNode> Parser::ParseStatement() {
    Lexeme curLexeme = myLexer.GetLexeme();
    wasError = false;

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
            } else {
                returnNode->SetExpression(CreateEmptyStatement(myLexer.GetLexeme()));
            }

            return returnNode;
        }

        AddParsingError(myLexer.NextLexeme(), "Unsupported keyword");
        return CreateEmptyStatement(myLexer.GetLexeme());
    }

    return ParseAssignment();
}

// '{' (statement (semis statement)*)? semis? '}'
Pointer<BlockNode> Parser::ParseBlock() {
    Pointer<BlockNode> blockNode = std::make_unique<BlockNode>(myLexer.GetLexeme());
    ConsumeLexeme(LexemeType::LCurl, "Expecting '{'");
    ConsumeSemicolons();

    while (myLexer.GetLexeme().GetType() != LexemeType::EndOfFile && myLexer.GetLexeme().GetType() != LexemeType::RCurl) {
        Pointer<ISyntaxNode> stmt = ParseStatement();
        ReturnNode* returnNode = dynamic_cast<ReturnNode*>(stmt.get());

        if (returnNode != nullptr) {
            auto typeSym = returnNode->GetExpression()->GetType();
            if (blockNode->GetSymbol() == nullptr) {
                blockNode->SetSymbol(typeSym);
            } else if (*blockNode->GetSymbol() != *returnNode->GetExpression()->GetSymbol()) {
                AddSemanticsError(myLexer.GetLexeme(), typeSym->GetName() +  " does not conform to the expected type" + blockNode->GetSymbol()->GetName());
            }
        }

        blockNode->AddStatement(std::move(stmt));
        ConsumeSemicolons();
    }

    ConsumeLexeme(LexemeType::RCurl, "Expecting '}'");

    if (blockNode->GetSymbol() == nullptr) {
        blockNode->SetSymbol(myRootTable->GetUnitSymbol());
    }
    return blockNode;
}

Pointer<ISyntaxNode> Parser::ParseControlStructureBody(bool acceptSemicolons = false) {
    if (myLexer.GetLexeme().GetType() == LexemeType::LCurl) {
        return ParseBlock();
    }
    if (myLexer.GetLexeme().GetType() == LexemeType::OpSemicolon && acceptSemicolons) {
        myLexer.NextLexeme();
        return CreateEmptyStatement(myLexer.GetLexeme());
    }

    return ParseStatement();
}

// 'for' '(' (variableDeclaration | multiVariableDeclaration) 'in' expression ')' controlStructureBody?
Pointer<ForNode> Parser::ParseForLoop() {
    Lexeme curLexeme = myLexer.NextLexeme();

    ConsumeLexeme(LexemeType::LParen, "Expecting '(' to open a loop range");
    Pointer<VariableNode> variable = ParseVariable();

    ConsumeLexeme(LexemeType::OpIn, "Expecting 'in'");
    Pointer<IAnnotatedNode> expr = ParseExpression();

    ConsumeLexeme(LexemeType::RParen, "Expecting ')'");
    Pointer<ISyntaxNode> body = ParseControlStructureBody();

    Pointer<ForNode> forLoop = std::make_unique<ForNode>(curLexeme, std::move(expr), std::move(body), std::move(variable));

    return forLoop;
}

// 'while' '(' expression ')' (controlStructureBody | ';')
Pointer<WhileNode> Parser::ParseWhileLoop() {
    Lexeme curLexeme = myLexer.NextLexeme();

    ConsumeLexeme(LexemeType::LParen, "Expecting '('");
    Pointer<IAnnotatedNode> expr = ParseExpression();
    ConsumeLexeme(LexemeType::RParen, "Expecting ')'");
    Pointer<ISyntaxNode> body = ParseControlStructureBody(true);

    Pointer<WhileNode> whileLoop = std::make_unique<WhileNode>(curLexeme, std::move(expr), std::move(body));

    return whileLoop;
}

// 'do' controlStructureBody 'while' '(' expression ')'
Pointer<DoWhileNode> Parser::ParseDoWhileLoop() {
    Lexeme curLexeme = myLexer.NextLexeme();

    Pointer<ISyntaxNode> body = ParseControlStructureBody();
    ConsumeLexeme(LexemeType::Keyword, "while", "Expecting 'while' followed by a post-condition");
    ConsumeLexeme(LexemeType::LParen, "Expecting '('");

    Pointer<IAnnotatedNode> expr = ParseExpression();
    ConsumeLexeme(LexemeType::RParen, "Expecting ')'");

    Pointer<DoWhileNode> doWhileLoop = std::make_unique<DoWhileNode>(curLexeme, std::move(expr), std::move(body));

    return doWhileLoop;
}

// (directlyAssignableExpression assignmentAndOperator)? expression
Pointer<ISyntaxNode> Parser::ParseAssignment() {
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

        Pointer<Assignment> assignment = std::make_unique<Assignment>(curLexeme, std::move(assignable), std::move(expression));
        return assignment;
    }

    return assignable;
}

// ('val' | 'var') variableDeclaration ('=' expression)? ';'?
Pointer<PropertyDeclaration> Parser::ParseProperty() {
    Lexeme keyword = myLexer.NextLexeme();
    Pointer<IdentifierNode> identifier = ParseIdentifier("Expecting property name");
    Pointer<TypeNode> typeNode;

    if (myLexer.GetLexeme().GetType() == LexemeType::OpColon) {
        myLexer.NextLexeme();
        typeNode = ParseType();
    }

    Pointer<IAnnotatedNode> initExpr;
    bool isFailed = false;
    if (myLexer.GetLexeme().GetType() == LexemeType::OpAssign) {
        myLexer.NextLexeme();
        initExpr = ParseExpression();
    } else {
        AddParsingError(myLexer.GetLexeme(), "Property must be initialized");
        isFailed = true;
        initExpr = CreateEmptyIdentifier(myLexer.GetLexeme());
    }

    if (!isFailed && typeNode != nullptr && initExpr != nullptr
        && *typeNode->GetSymbol() != *initExpr->GetSymbol()) {
        AddSemanticsError(myLexer.GetLexeme(), initExpr->GetSymbol()->GetName()
                          + " does not conform to the expected type " + typeNode->GetSymbol()->GetName());
    }

    Pointer<PropertyDeclaration> propertyDecl = std::make_unique<PropertyDeclaration>(std::move(identifier), keyword);
    propertyDecl->SetTypeNode(std::move(typeNode));
    propertyDecl->SetInitialization(std::move(initExpr));

    auto propertySym = std::make_unique<VariableSymbol>(propertyDecl->GetIdentifierName(),
       propertyDecl->GetInitialization().GetType(), propertyDecl->IsMutable());

    const ISymbol* sym = myTable->Add(std::move(propertySym));
    if (*sym == UnresolvedSymbol()) {
        AddSemanticsError(myLexer.GetLexeme(), "Conflicting declarations: " + propertyDecl->GetIdentifierName());
    }

    propertyDecl->SetSymbol(sym);
    propertyDecl->GetIdentifier().Resolve(sym);
    return propertyDecl;
}

// simpleIdentifier (':' type)?
Pointer<VariableNode> Parser::ParseVariable() {
    Pointer<VariableNode> variable = std::make_unique<VariableNode>(ParseIdentifier("Expecting a variable name"));
    if (myLexer.GetLexeme().GetType() == LexemeType::OpColon) {
        myLexer.NextLexeme();
        variable->SetTypeNode(ParseType());
    }

    return variable;
}

Pointer<IdentifierNode> Parser::ParseIdentifier(const std::string& errorMessage) {
    Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() != LexemeType::Identifier) {
        curLexeme = curLexeme.CopyEmptyOfType(LexemeType::Identifier);
    }

    Pointer<IdentifierNode> identifier = CreateLexemeNode(curLexeme, myRootTable->GetUnresolvedSymbol(), myTable->GetSymbols(curLexeme.GetText()));
    ConsumeLexeme(LexemeType::Identifier, errorMessage);
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

        const ITypeSymbol* leftType = leftOperand->GetType();
        const ITypeSymbol* rightType = rightOperand->GetType();
        Pointer<ITypeSymbol> resultType = std::make_unique<UnresolvedSymbol>();

        if (leftType != nullptr && rightType != nullptr) {
            resultType = IsApplicable(operation.GetType(), leftType, rightType);
        }

        std::string typeName = resultType->GetName();

        if (dynamic_cast<ArraySymbol*>(resultType.get()) != nullptr || dynamic_cast<RangeSymbol*>(resultType.get()) != nullptr) {
            myRootTable->Add(std::move(resultType));    
        }

        leftOperand = std::make_unique<BinOperationNode>(
            operation, std::move(leftOperand), std::move(rightOperand), GetType(typeName));
        operation = myLexer.GetLexeme();
    }

    return leftOperand;
}

// prefixUnaryOperator* postfixUnaryExpression
Pointer<IAnnotatedNode> Parser::ParsePrefix() {
    const Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() == LexemeType::OpSub || curLexeme.GetType() == LexemeType::OpAdd
        || curLexeme.GetType() == LexemeType::OpInc || curLexeme.GetType() == LexemeType::OpDec
        || curLexeme.GetType() == LexemeType::OpExclMark) {
        myLexer.NextLexeme();
        Pointer<IAnnotatedNode> prefix = ParsePrefix();
        Pointer<ITypeSymbol> resultType = IsApplicable(curLexeme.GetType(), prefix->GetType());
        return std::make_unique<UnaryPrefixOperationNode>(curLexeme, std::move(prefix), GetType(resultType->GetName()));
    }

    return ParsePostfix();
}

// TODO: REFACTOR
// primaryExpression postfixUnarySuffix*
Pointer<IAnnotatedNode> Parser::ParsePostfix() {
    Pointer<IAnnotatedNode> operand = ParsePrimary();
    Lexeme curLexeme = myLexer.GetLexeme();

    while (ParserUtils::PostfixOperations.count(curLexeme.GetType()) || curLexeme.GetType() == LexemeType::OpLess && dynamic_cast<IdentifierNode*>(operand.get()) != nullptr) {
        myLexer.NextLexeme();
        if (curLexeme.GetType() == LexemeType::OpInc || curLexeme.GetType() == LexemeType::OpDec) {
            Pointer<ITypeSymbol> resultType = IsApplicable(curLexeme.GetType(), operand->GetType());
            operand = std::make_unique<UnaryPostfixOperationNode>(curLexeme, std::move(operand), GetType(resultType->GetName()));

        } else if (curLexeme.GetType() == LexemeType::OpDot) {
            Pointer<IAnnotatedNode> member = CreateEmptyIdentifier(curLexeme);
            if (myLexer.GetLexeme().GetType() == LexemeType::Identifier) {
                IdentifierNode* identifier = dynamic_cast<IdentifierNode*>(operand.get());
                if (identifier != nullptr) {
                    identifier->TryResolveVariable();
                }
                auto classSym = dynamic_cast<const ClassSymbol*>(operand->GetSymbol());
                std::vector<const ISymbol*> candidates;
                if (classSym != nullptr) {
                    candidates = classSym->GetTable().GetSymbols(myLexer.GetLexeme().GetValue<std::string>());
                }

                Pointer<IdentifierNode> memberIdentifier = CreateLexemeNode(myLexer.GetLexeme(), myRootTable->GetUnresolvedSymbol(), candidates);
                memberIdentifier->TryResolveVariable();
                member = std::move(memberIdentifier);
                
                myLexer.NextLexeme();
            } else {
                AddParsingError(curLexeme, "Name expected");
            }

            std::unique_ptr<MemberAccessNode> memberAccess = std::make_unique<MemberAccessNode>(curLexeme, std::move(operand), std::move(member));
            operand = std::move(memberAccess);

        } else if (curLexeme.GetType() == LexemeType::LParen || curLexeme.GetType() == LexemeType::OpLess) {
            Pointer<TypeArgumentsNode> typeArgs;
            if (curLexeme.GetType() == LexemeType::OpLess) {
                typeArgs = ParseTypeArguments();
                myLexer.NextLexeme();

                if (myLexer.GetLexeme().GetType() != LexemeType::LParen) {
                    AddParsingError(myLexer.GetLexeme(), "Type arguments currently are only supported for function calls");
                } else {
                    myLexer.NextLexeme();
                }
            }

            Pointer<CallArgumentsNode> args = ParseArguments(LexemeType::RParen);
            const ITypeSymbol* resType = myRootTable->GetUnresolvedSymbol();

            IdentifierNode* identifier = dynamic_cast<IdentifierNode*>(operand.get());
            if (identifier != nullptr) {
                std::vector<const ITypeSymbol*> argsTypes;
                for (auto& arg : args->GetArguments()) {
                    argsTypes.push_back(arg->GetType());
                }

                identifier->TryResolveFunc(argsTypes);

                if (typeArgs != nullptr && typeArgs->GetArguments().size() == 1 && identifier->GetLexeme().GetText() == "arrayOf") {
                    Pointer<ArraySymbol> arrayType = std::make_unique<ArraySymbol>(typeArgs->GetArguments()[0]->GetType());

                    bool isFailed = false;

                    for (auto& arg : args->GetArguments()) {
                        if (*arg->GetSymbol() != *typeArgs->GetArguments()[0]->GetSymbol()) {
                            AddSemanticsError(myLexer.GetLexeme(), arg->GetSymbol()->GetName() + " does not conform to the expected type " + typeArgs->GetArguments()[0]->GetSymbol()->GetName());
                            isFailed = true;
                            break;
                        }
                    }

                    if (!isFailed) {
                        std::string arrayTypeName = arrayType->GetName();
                        myRootTable->Add(std::move(arrayType));

                        Pointer<FunctionSymbol> arrayInitFunc = std::make_unique<FunctionSymbol>("arrayOf", myRootTable->GetType(arrayTypeName), argsTypes, std::make_unique<SymbolTable>(myRootTable));
                        myRootTable->Add(std::move(arrayInitFunc));
                        identifier->Resolve(myRootTable->GetFunction("arrayOf", argsTypes));
                    }
                }

                resType = identifier->GetType();
            }

            Pointer<CallSuffixNode> callNode = std::make_unique<CallSuffixNode>(std::move(operand), std::move(args), resType);

            ConsumeLexeme(LexemeType::RParen, "Expecting ')'");
            operand = std::move(callNode);

        } else if (curLexeme.GetType() == LexemeType::LSquare) {
            Pointer<CallArgumentsNode> args = ParseArguments(LexemeType::RSquare);
            const ITypeSymbol* resType = myRootTable->GetUnresolvedSymbol();

            if (args->GetArguments().size() != 1) {
                AddParsingError(curLexeme, "Expecting an index");
            } else {
                auto arrSym = dynamic_cast<const ArraySymbol*>(operand->GetType());
                if (arrSym != nullptr) {
                    std::vector<const ITypeSymbol*> argsTypes;
                    for (auto& arg : args->GetArguments()) {
                        argsTypes.push_back(arg->GetType());
                    }

                    if (*argsTypes[0] == IntegerSymbol()) {
                        resType = arrSym->GetType();
                    } else {
                        AddSemanticsError(curLexeme, argsTypes[0]->GetName() + " does not conform to the expected type " + IntegerSymbol().GetName());
                    }
                }
            }

            Pointer<IndexSuffixNode> indexNode = std::make_unique<IndexSuffixNode>(std::move(operand), std::move(args), resType);

            ConsumeLexeme(LexemeType::RSquare, "Expecting ']'");
            operand = std::move(indexNode);
        }

        curLexeme = myLexer.GetLexeme();
    }

    return operand;
}

Pointer<TypeArgumentsNode> Parser::ParseTypeArguments() {
    Pointer<TypeArgumentsNode> arguments = std::make_unique<TypeArgumentsNode>(myLexer.GetLexeme());
    while (myLexer.GetLexeme().GetType() != LexemeType::OpGreater && myLexer.GetLexeme().GetType() != LexemeType::EndOfFile) {
        arguments->AddArgument(ParseType());

        if (myLexer.GetLexeme().GetType() != LexemeType::OpComma) {
            if (myLexer.GetLexeme().GetType() == LexemeType::OpGreater || myLexer.GetLexeme().GetType() == LexemeType::EndOfFile) {
                break;
            }

            AddParsingError(myLexer.GetLexeme(), "Expecting ','");
        } else {
            myLexer.NextLexeme();
        }
    }

    return arguments;
}

Pointer<CallArgumentsNode> Parser::ParseArguments(LexemeType rParen) {
    Pointer<CallArgumentsNode> arguments = std::make_unique<CallArgumentsNode>(myLexer.GetLexeme());
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
Pointer<IAnnotatedNode> Parser::ParsePrimary() {
    const Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() == LexemeType::Keyword) {
        std::string keyword = curLexeme.GetValue<std::string>();
        if (keyword == "true" || keyword == "false") {
            Pointer<BooleanNode> node = CreateLexemeNode<BooleanNode>(myLexer.NextLexeme(), GetType(BooleanSymbol().GetName()));
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
        Pointer<IAnnotatedNode> expr = ParseExpression();
        ConsumeLexeme(LexemeType::RParen, "Expecting ')'");
        return expr;
    }
    if (curLexeme.GetType() == LexemeType::Identifier) {
        Pointer<IdentifierNode> node = CreateLexemeNode(curLexeme,
            myRootTable->GetUnresolvedSymbol(), myTable->GetSymbols(curLexeme.GetValue<std::string>()));
        node->TryResolveVariable();
        return node;
    }
    if (LexerUtils::IsIntegerType(curLexeme.GetType())) {
        Pointer<IntegerNode> node = CreateLexemeNode<IntegerNode>(curLexeme, GetType(IntegerSymbol().GetName()));
        return node;
    }
    if (LexerUtils::IsRealType(curLexeme.GetType())) {
        Pointer<DoubleNode> node = CreateLexemeNode<DoubleNode>(curLexeme, GetType(DoubleSymbol().GetName()));
        return node;
    }
    if (curLexeme.GetType() == LexemeType::String || curLexeme.GetType() == LexemeType::RawString) {
        Pointer<StringNode> node = CreateLexemeNode<StringNode>(curLexeme, GetType(StringSymbol().GetName()));
        return node;
    }

    AddParsingError(curLexeme, "Unexpected lexeme");
    return CreateEmptyIdentifier(curLexeme);
}

// 'if' '(' expression ')' (controlStructureBody | (controlStructureBody? ';'? 'else' (controlStructureBody | ';')) | ';')
Pointer<IfExpression> Parser::ParseIfExpression() {
    Lexeme curLexeme = myLexer.NextLexeme();

    ConsumeLexeme(LexemeType::LParen, "Expecting '('");
    Pointer<IfExpression> ifExpr = std::make_unique<IfExpression>(curLexeme, myRootTable->GetUnitSymbol(), ParseExpression());
    ConsumeLexeme(LexemeType::RParen, "Expecting ')'");

    if (myLexer.GetLexeme().GetType() == LexemeType::Keyword && myLexer.GetLexeme().GetValue<std::string>() == "else") {
        ifExpr->SetIfBody(CreateEmptyStatement(myLexer.GetLexeme()));
        myLexer.NextLexeme();
        ifExpr->SetElseBody(ParseControlStructureBody(true));
        return ifExpr;
    }

    ifExpr->SetIfBody(ParseControlStructureBody(true));

    if (myLexer.GetLexeme().GetType() == LexemeType::Keyword && myLexer.GetLexeme().GetValue<std::string>() == "else") {
        myLexer.NextLexeme();
        ifExpr->SetElseBody(ParseControlStructureBody(true));
    } else {
        ifExpr->SetElseBody(CreateEmptyStatement(myLexer.GetLexeme()));
    }

    return ifExpr;
}

Pointer<IdentifierNode> Parser::CreateEmptyIdentifier(const Lexeme& lexeme) {
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

bool Parser::ConsumeLexeme(LexemeType lexemeType, const std::string& error) {
    if (myLexer.GetLexeme().GetType() != lexemeType) {
        AddParsingError(myLexer.GetLexeme(), error);
        return false;
    }

    myLexer.NextLexeme();
    return true;
}

bool Parser::ConsumeLexeme(LexemeType lexemeType, const std::string& text, const std::string& error) {
    if (myLexer.GetLexeme().GetText() != text) {
        AddParsingError(myLexer.GetLexeme(), error);
        return false;
    }

    return ConsumeLexeme(lexemeType, error);
}

void Parser::ConsumeSemicolons() {
    while (myLexer.GetLexeme().GetType() == LexemeType::OpSemicolon) {
        myLexer.NextLexeme();
    }
}

Pointer<EmptyStatement> Parser::CreateEmptyStatement(const Lexeme& lexeme) {
    return std::make_unique<EmptyStatement>(lexeme, myRootTable->GetUnitSymbol());
}

Pointer<ITypeSymbol> Parser::IsApplicable(LexemeType operation, const ITypeSymbol* left, const ITypeSymbol* right) {
    Pointer<ITypeSymbol> res = left->IsApplicable(operation, right);
    if (*res == UnresolvedSymbol()) {
        AddSemanticsError(myLexer.GetLexeme(), "Operation is not applicable to types "
            + left->GetName() + " and " + right->GetName());
    }
    return res;
}

Pointer<ITypeSymbol> Parser::IsApplicable(LexemeType operation, const ITypeSymbol* left) {
    Pointer<ITypeSymbol> res = left->IsApplicable(operation);
    if (*res == UnresolvedSymbol()) {
        AddSemanticsError(myLexer.GetLexeme(), "Operation is not applicable to type " + left->GetName());
    }
    return res;
}

const ITypeSymbol* Parser::GetType(const std::string& name) {
    const ITypeSymbol* type = myTable->GetType(name);
    if (*type == UnresolvedSymbol()) {
        AddSemanticsError(myLexer.GetLexeme(), "Unresolved symbol");
    }
    return type;
}
