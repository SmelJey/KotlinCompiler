#include "CompilerTest.h"

#include "PrintVisitors.h"
#include "Parser/Parser.h"
#include "Parser/ParserError.h"

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
    SymbolTable table;
    Parser parser(lexer, &table);

    Pointer<AbstractNode> tree = parser.Parse();
    CuteToStringVisitor visitor;
    tree->RunVisitor(visitor);

    myTokens = visitor.GetStringData();
    for (auto& err : parser.GetParsingErrors()) {
        myTokens.push_back(err.ToString());
    }
}

std::string ParserTest::NextToken() {
    if (myIdx >= myTokens.size()) {
        return "";
    }

    return myTokens[myIdx++];
}

std::string ParserSemanticTest::CreateTestPath(const std::string& path) {
    return TestDirectory + SemanticsDirectory + path;
}

ParserSemanticTest::ParserSemanticTest(const std::string& filepath) : IOTest(SemanticsDirectory + filepath) {
    Lexer lexer(GetFilepath());
    SymbolTable table;
    Parser parser(lexer, &table);

    Pointer<AbstractNode> tree = parser.Parse();
    CuteToStringVisitor parserVisitor;
    parserVisitor.ShowSemanticsAnnotations();
    tree->RunVisitor(parserVisitor);

    myTokens = parserVisitor.GetStringData();

    CuteToStringVisitor semanticsVisitor;
    table.RunVisitor(semanticsVisitor);

    myTokens.push_back("====");
    for (auto it : semanticsVisitor.GetStringData()) {
        myTokens.push_back(it);
    }

    for (auto& err : parser.GetParsingErrors()) {
        myTokens.push_back(err.ToString());
    }
    for (auto& err : parser.GetSemanticsErrors()) {
        myTokens.push_back(err.ToString());
    }
}

std::string ParserSemanticTest::NextToken() {
    if (myIdx >= myTokens.size()) {
        return "";
    }

    return myTokens[myIdx++];
}
