#include "Parser.h"

#include "ExpressionNodes.h"
#include "ParserUtils.h"
#include "SimpleNodes.h"

Parser::Parser(Lexer& lexer) : myLexer(lexer) {
    myLexer.NextLexeme();
}

Lexer& Parser::GetLexer() {
    return myLexer;
}

// (classDeclaration | functionDeclaration | propertyDeclaration semis?)*
Pointer<DeclarationBlock> Parser::Parse() {
    Pointer<DeclarationBlock> declarations = std::make_unique<DeclarationBlock>();
    int curRow = -1;
    Lexeme curLexeme = myLexer.GetLexeme();
    while (curLexeme.GetType() != Lexeme::LexemeType::EndOfFile) {
        if (curLexeme.GetType() != Lexeme::LexemeType::Keyword) {
            AddError(*declarations, curLexeme, "Expecting a top level declaration");
            myLexer.NextLexeme();
        } else {
            std::string keyword = curLexeme.GetValue<std::string>();

            if (keyword == "class") {
                declarations->AddDeclaration(std::move(ParseClass()));
            } else if (keyword == "fun") {
                declarations->AddDeclaration(std::move(ParseFunction()));
            } else if (keyword == "var" || keyword == "val") {
                declarations->AddDeclaration(std::move(ParseProperty()));
            } else {
                AddError(*declarations, curLexeme, "Unsupported keyword");
                myLexer.NextLexeme();
            }
        }

        curLexeme = myLexer.GetLexeme();
    }

    return declarations;
}

// 'class' simpleIdentifier classBody?
Pointer<IDeclaration> Parser::ParseClass() {
    throw;
}

// 'fun' simpleIdentifier functionValueParameters (':' type)? functionBody
Pointer<IDeclaration> Parser::ParseFunction() {
    myLexer.NextLexeme();
    Lexeme curLexeme = myLexer.GetLexeme();
    Pointer<FunctionDeclaration> functionDecl;
    if (curLexeme.GetType() != Lexeme::LexemeType::Identifier) {
        functionDecl = std::make_unique<FunctionDeclaration>(Lexeme(curLexeme.GetColumn(), curLexeme.GetRow(), "", Lexeme::LexemeType::Identifier, "", false));
        AddError(*functionDecl, curLexeme, "Function declaration must have a name");
    } else {
        functionDecl = std::make_unique<FunctionDeclaration>(curLexeme);
        myLexer.NextLexeme();
    }

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
    if (myLexer.GetLexeme().GetType() != Lexeme::LexemeType::LParen) {
        AddError(*params, myLexer.GetLexeme(), "Expecting '('");
    } else {
        myLexer.NextLexeme();
    }

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

    if (myLexer.GetLexeme().GetType() != Lexeme::LexemeType::RParen) {
        AddError(*params, myLexer.GetLexeme(), "Expecting ')'");
    } else {
        myLexer.NextLexeme();
    }

    return params;
}

//  simpleIdentifier ':' type ('=' expression)? 
Pointer<Parameter> Parser::ParseParameter() {
    Lexeme curLexeme = myLexer.GetLexeme();
    Pointer<Parameter> param;
    if (curLexeme.GetType() != Lexeme::LexemeType::Identifier) {
        param = std::make_unique<Parameter>(Lexeme(curLexeme.GetColumn(), curLexeme.GetRow(), "", Lexeme::LexemeType::Identifier, ""));
        AddError(*param, curLexeme, "Parameter name expected");
    } else {
        param = std::make_unique<Parameter>(curLexeme);
        myLexer.NextLexeme();
    }

    curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() != Lexeme::LexemeType::OpColon) {
        param->SetTypeNode(std::make_unique<ErrorNode>(Lexeme(curLexeme.GetColumn(), curLexeme.GetRow(),
                        "", Lexeme::LexemeType::Identifier, ""), "A type annotation is required on a value parameter"));
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
        if (myLexer.GetLexeme().GetType() != Lexeme::LexemeType::RParen) {
            AddError(*typeNode, myLexer.GetLexeme(), "Expecting ')'");
        } else {
            myLexer.NextLexeme();
        }

        return typeNode;
    }

    Lexeme curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() != Lexeme::LexemeType::Identifier) {
        return std::make_unique<ErrorNode>(curLexeme, "Type expected");
    }

    myLexer.NextLexeme();
    return std::make_unique<TypeNode>(curLexeme);
}

Pointer<BlockNode> Parser::ParseBlock() {
    Pointer<BlockNode> blockNode = std::make_unique<BlockNode>();
    if (myLexer.GetLexeme().GetType() != Lexeme::LexemeType::LCurl) {
        AddError(*blockNode, myLexer.GetLexeme(), "Expecting '{'");
    } else {
        myLexer.NextLexeme();
    }

    while (myLexer.GetLexeme().GetType() != Lexeme::LexemeType::EndOfFile && myLexer.GetLexeme().GetType() != Lexeme::LexemeType::RCurl) {
        // TODO: Change to statement
        blockNode->AddStatement(ParseExpression());
    }

    if (myLexer.GetLexeme().GetType() != Lexeme::LexemeType::RCurl) {
        AddError(*blockNode, myLexer.GetLexeme(), "Expecting '}'");
    } else {
        myLexer.NextLexeme();
    }

    return blockNode;
}

// propertyDeclaration (used by declaration) : ('val' | 'var') variableDeclaration ('=' expression) ? ';' ?
Pointer<IDeclaration> Parser::ParseProperty() {
    throw;
}

// LeftAssociative(0)
Pointer<ISyntaxNode> Parser::ParseExpression() {
    return ParseLeftAssociative(0);
}

// LeftAssociative(n+1) | LeftAssociative(n+1) operator(n) LeftAssociative(n+1)
Pointer<ISyntaxNode> Parser::ParseLeftAssociative(size_t priority) {
    if (priority == ParserUtils::OperationsPriority.size()) {
        return ParsePrefix();
    }

    Pointer<ISyntaxNode> leftOperand = ParseLeftAssociative(priority + 1);
    Lexeme operation = myLexer.GetLexeme();

    while (ParserUtils::OperationsPriority.at(priority).count(operation.GetType())
           // infixFunctionCall 
           || (priority == 5 && operation.GetType() == Lexeme::LexemeType::Identifier)) {
        myLexer.NextLexeme();
        Pointer<ISyntaxNode> rightOperand = ParseLeftAssociative(priority + 1);

        leftOperand = std::make_unique<BinOperationNode>(operation, std::move(leftOperand), std::move(rightOperand));
        operation = myLexer.GetLexeme();
    }

    return leftOperand;
}

// prefixUnaryOperator* postfixUnaryExpression
Pointer<ISyntaxNode> Parser::ParsePrefix() {
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
Pointer<ISyntaxNode> Parser::ParsePostfix() {
    Pointer<ISyntaxNode> operand = ParsePrimary();
    Lexeme curLexeme = myLexer.GetLexeme();

    while (ParserUtils::PostfixOperations.count(curLexeme.GetType())) {
        myLexer.NextLexeme();
        if (curLexeme.GetType() == Lexeme::LexemeType::OpInc || curLexeme.GetType() == Lexeme::LexemeType::OpDec) {
            operand = std::make_unique<UnaryPostfixOperationNode>(curLexeme, std::move(operand));

        } else if (curLexeme.GetType() == Lexeme::LexemeType::OpDot) {
            std::unique_ptr<MemberAccessNode> memberAccess = std::make_unique<MemberAccessNode>(std::move(operand));
            curLexeme = myLexer.GetLexeme();
            if (curLexeme.GetType() != Lexeme::LexemeType::Identifier) {
                memberAccess->SetMember(std::make_unique<ErrorNode>(curLexeme, "Name expected"));
            } else {
                memberAccess->SetMember(ReturnNode<IdentifierNode>(curLexeme));
                myLexer.NextLexeme();
            }
            operand = std::move(memberAccess);

        } else if (curLexeme.GetType() == Lexeme::LexemeType::LParen) {
            Pointer<CallSuffixNode> callNode = std::make_unique<CallSuffixNode>(std::move(operand));
            std::vector<Pointer<ISyntaxNode>> args = ParseArguments(*callNode, Lexeme::LexemeType::RParen);
            callNode->SetArguments(std::move(args));

            Lexeme curLexeme = myLexer.GetLexeme();
            if (curLexeme.GetType() != Lexeme::LexemeType::RParen) {
                AddError(*callNode, curLexeme, "Expecting ')'");
            } else {
                myLexer.NextLexeme();
            }

            operand = std::move(callNode);

        } else if (curLexeme.GetType() == Lexeme::LexemeType::LSquare) {
            Pointer<IndexSuffixNode> indexNode = std::make_unique<IndexSuffixNode>(std::move(operand));
            std::vector<Pointer<ISyntaxNode>> args = ParseArguments(*indexNode, Lexeme::LexemeType::RSquare);

            if (args.empty()) {
                AddError(*indexNode, curLexeme, "Expecting an index");
            }
            indexNode->SetArguments(std::move(args));

            Lexeme curLexeme = myLexer.GetLexeme();
            if (curLexeme.GetType() != Lexeme::LexemeType::RSquare) {
                AddError(*indexNode, curLexeme, "Expecting ']'");
            } else {
                myLexer.NextLexeme();
            }

            operand = std::move(indexNode);
        }

        curLexeme = myLexer.GetLexeme();
    }

    return operand;
}

std::vector<Pointer<ISyntaxNode>> Parser::ParseArguments(ISyntaxNode& host, Lexeme::LexemeType rParen) {
    std::vector<Pointer<ISyntaxNode>> params;
    bool isLastArg = false;
    while (myLexer.GetLexeme().GetType() != rParen && myLexer.GetLexeme().GetType() != Lexeme::LexemeType::EndOfFile) {
        if (isLastArg) {
            AddError(host, myLexer.GetLexeme(), "Expecting ','");
        }

        params.push_back(ParseExpression());

        isLastArg = true;
        if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::OpComma) {
            myLexer.NextLexeme();
            isLastArg = false;
        }
    }

    return params;
}

// TODO: if expression
// Identifier | Number | String | '(' LeftAssociative(0) ')'
Pointer<ISyntaxNode> Parser::ParsePrimary() {
    const Lexeme curLexeme = myLexer.NextLexeme();
    if (curLexeme.GetType() == Lexeme::LexemeType::LParen) {
        Pointer<ISyntaxNode> expr = ParseExpression();
        const Lexeme rParen = myLexer.GetLexeme();
        if (rParen.GetType() != Lexeme::LexemeType::RParen) {
            AddError(*expr, rParen, "Expecting \")\"");
            return expr;
        }

        myLexer.NextLexeme();
        return expr;
    }
    if (curLexeme.GetType() == Lexeme::LexemeType::Identifier) {
        return ReturnNode<IdentifierNode>(curLexeme);
    }
    if (LexerUtils::IsIntegerType(curLexeme.GetType())) {
        return ReturnNode<IntegerNode>(curLexeme);
    }
    if (LexerUtils::IsRealType(curLexeme.GetType())) {
        return ReturnNode<RealNode>(curLexeme);
    }
    if (curLexeme.GetType() == Lexeme::LexemeType::String || curLexeme.GetType() == Lexeme::LexemeType::RawString) {
        return ReturnNode<StringNode>(curLexeme);
    }
    if (curLexeme.GetType() == Lexeme::LexemeType::Keyword && (curLexeme.GetValue<std::string>() == "true" || curLexeme.GetValue<std::string>() == "false")) {
        return ReturnNode<BooleanNode>(curLexeme);
    }

    return std::make_unique<ErrorNode>(ErrorNode(curLexeme));
}

void Parser::AddError(ISyntaxNode& root, const Lexeme& location, const std::string& error) const {
    root.AddError(std::make_unique<ErrorNode>(location, error));
}
