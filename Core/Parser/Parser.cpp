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
    Lexeme curLexeme = myLexer.NextLexeme();
    Pointer<FunctionDeclaration> functionDecl = std::make_unique<FunctionDeclaration>(curLexeme);
    if (curLexeme.GetType() != Lexeme::LexemeType::Identifier) {
        AddError(*functionDecl, curLexeme, "Function declaration must have a name");
    }

    functionDecl->SetParameters(ParseParameters(true));

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
Pointer<ParameterList> Parser::ParseParameters(bool isDeclaration) {
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

        params->AddParameter(std::move(ParseParameter(isDeclaration)));

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
Pointer<Parameter> Parser::ParseParameter(bool isDeclaration) {
    Lexeme curLexeme = myLexer.NextLexeme();
    Pointer<Parameter> param = std::make_unique<Parameter>(curLexeme);
    if (curLexeme.GetType() != Lexeme::LexemeType::Identifier) {
        AddError(*param, curLexeme, "Parameter name expected");
    }

    if (!isDeclaration) {
        return param;
    }

    curLexeme = myLexer.GetLexeme();
    if (curLexeme.GetType() != Lexeme::LexemeType::OpColon) {
        AddError(*param, curLexeme, "A type annotation is required on a value parameter");
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
Pointer<TypeNode> Parser::ParseType() {
    if (myLexer.GetLexeme().GetType() == Lexeme::LexemeType::LParen) {
        myLexer.NextLexeme();
        Pointer<TypeNode> typeNode = ParseType();
        if (myLexer.GetLexeme().GetType() != Lexeme::LexemeType::RParen) {
            AddError(*typeNode, myLexer.GetLexeme(), "Expecting ')'");
        } else {
            myLexer.NextLexeme();
        }

        return typeNode;
    }

    Lexeme curLexeme = myLexer.NextLexeme();
    Pointer<TypeNode> typeNode = std::make_unique<TypeNode>(curLexeme);
    if (curLexeme.GetType() != Lexeme::LexemeType::Identifier) {
        AddError(*typeNode, curLexeme, "Type expected");
    }

    return typeNode;
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
        return ParseFactor();
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

// TODO: if expression
// (Identifier | Number | String | UnaryOp Factor | (LeftAssociative(0))) (postfixUnarySuffix)*
Pointer<ISyntaxNode> Parser::ParseFactor() {
    const Lexeme curLexeme = myLexer.NextLexeme();
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

    if (curLexeme.GetType() == Lexeme::LexemeType::OpSub || curLexeme.GetType() == Lexeme::LexemeType::OpAdd
        || curLexeme.GetType() == Lexeme::LexemeType::OpInc || curLexeme.GetType() == Lexeme::LexemeType::OpDec
        || curLexeme.GetType() == Lexeme::LexemeType::OpExclMark) {
        Pointer<ISyntaxNode> operand = ParseFactor();
        return std::make_unique<UnaryOperationNode>(curLexeme, std::move(operand));
    }

    return std::make_unique<ErrorNode>(ErrorNode(curLexeme));
}

void Parser::AddError(ISyntaxNode& root, const Lexeme& location, const std::string& error) const {
    root.AddError(std::make_unique<ErrorNode>(location, error));
}
