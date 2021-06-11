#include "CompilerTest.h"

#include "Parser/NodeVisitor.h"
#include "Parser/Parser.h"

LexerTest::LexerTest(const std::string& filepath) : IOTest(LexerDirectory + filepath) {
    myLexer = std::make_unique<Lexer>(Lexer(GetFilepath()));
    myLexer->NextLexeme();
}

std::string LexerTest::NextToken() {
    const Lexeme lexeme = myLexer->NextLexeme();
    if (lexeme.GetType() == Lexeme::LexemeType::EndOfFile) {
        return "";
    }
    return lexeme.ToString();
}

ParserTest::ParserTest(const std::string& filepath) : IOTest(ParserDirectory + filepath) {
    Lexer lexer(GetFilepath());
    Parser parser(lexer);

    Pointer<ISyntaxNode> tree = parser.Parse();
    CuteToStringVisitor visitor;
    tree->RunVisitor(visitor);

    myTokens = visitor.GetStringData();
}

std::string ParserTest::NextToken() {
    if (myIdx >= myTokens.size()) {
        return "";
    }

    return myTokens[myIdx++];
}
