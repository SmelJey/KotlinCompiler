#include "ConfigurationBuilder.h"
#include "Configuration.h"
#include "Lexer/Lexer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "Parser/Parser.h"
#include "Parser/NodeVisitor.h"

namespace prog_opt = boost::program_options;

const char* FILES_KEY = "source-files";
const char* LEXER_DEBUG_KEY = "lexer-debug";
const char* PARSER_DEBUG_KEY = "parser-debug";

Configuration ParseCommandLineArgs(int argc, char** argv) {
    prog_opt::options_description optionsDesc("Allowed options");
    optionsDesc.add_options()
        ("help", "show help message")
        ("lexer-debug,l", "debug lexical analyser")
        ("source-files,f", prog_opt::value<std::vector<std::string>>(), "source files")
        ("parser-debug,p", "debug syntax analyser");

    prog_opt::positional_options_description positionalOptions;
    positionalOptions.add(FILES_KEY, -1);

    prog_opt::variables_map optionsMap;

    store(prog_opt::command_line_parser(argc, argv).options(optionsDesc).positional(positionalOptions).run(), optionsMap);

    ConfigurationBuilder builder;

    if (optionsMap.count(FILES_KEY) == 0) {
        // TODO: make custom exception
        throw "No files to compile";
    }

    builder.AddPaths(optionsMap[FILES_KEY].as<std::vector<std::string>>());
    if (optionsMap.count(LEXER_DEBUG_KEY)) {
        builder.SetLexerDebug();
    }
    if (optionsMap.count(PARSER_DEBUG_KEY)) {
        builder.SetParserDebug();
    }

    return builder.Build();
}

int main(int argc, char** argv) {
    const Configuration configuration = ParseCommandLineArgs(argc, argv);

    std::ifstream ifs(configuration.GetPaths()[0]);

    if (configuration.GetLexerDebug()) {
        Lexer lexer(configuration.GetPaths()[0]);
        lexer.NextLexeme();

        while (lexer.GetLexeme().GetType() != Lexeme::LexemeType::EndOfFile) {
            std::cout << lexer.NextLexeme() << std::endl;
        }
    }

    Lexer lexer(configuration.GetPaths()[0]);
    Parser parser(lexer);
    Pointer<ISyntaxNode> syntaxTree = parser.Parse();

    if (configuration.GetParserDebug()) {
        PrintVisitor visitor;
        syntaxTree->RunVisitor(visitor);
    }

    return 0;
}
