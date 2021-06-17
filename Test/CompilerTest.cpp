#include "CompilerTest.h"

#include "Parser/NodeVisitor.h"
#include "Parser/Parser.h"

std::string LexerTest::CreateTestPath(const std::string& path) {
    return TestDirectory + LexerDirectory + path;
}

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

std::string ParserTest::CreateTestPath(const std::string& path) {
    return TestDirectory + ParserDirectory + path;
}

ParserTest::ParserTest(const std::string& filepath) : IOTest(ParserDirectory + filepath) {
    Lexer lexer(GetFilepath());
    Parser parser(lexer);

    Pointer<AbstractNode> tree = parser.Parse();
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
