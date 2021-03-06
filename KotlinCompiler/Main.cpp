#include "ConfigurationBuilder.h"
#include "Configuration.h"
#include "Lexer/Lexer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "PrintVisitors.h"
#include "Interpreter/Interpreter.h"

#include "Parser/Parser.h"
#include "Parser/ParserError.h"

namespace prog_opt = boost::program_options;

const char* FILES_KEY = "source-files";
const char* LEXER_DEBUG_KEY = "lexer-debug";
const char* PARSER_DEBUG_KEY = "parser-debug";
const char* SEMANTICS_DEBUG_KEY = "semantics-debug";

Configuration ParseCommandLineArgs(int argc, char** argv) {
    prog_opt::options_description optionsDesc("Allowed options");
    optionsDesc.add_options()
        ("help", "show help message")
        ("lexer-debug,l", "debug lexical analyser")
        ("source-files,f", prog_opt::value<std::vector<std::string>>(), "source files")
        ("parser-debug,p", "debug syntax analyser")
        ("semantics-debug,s", "debug semantics");

    prog_opt::positional_options_description positionalOptions;
    positionalOptions.add(FILES_KEY, -1);

    prog_opt::variables_map optionsMap;

    store(prog_opt::command_line_parser(argc, argv).options(optionsDesc).positional(positionalOptions).run(), optionsMap);

    ConfigurationBuilder builder;

    if (optionsMap.count(FILES_KEY) == 0) {
        std::cout << "No files to compile" << std::endl;
        return builder.Build();
    }

    builder.AddPaths(optionsMap[FILES_KEY].as<std::vector<std::string>>());
    if (optionsMap.count(LEXER_DEBUG_KEY)) {
        builder.SetLexerDebug();
    }
    if (optionsMap.count(PARSER_DEBUG_KEY)) {
        builder.SetParserDebug();
    }
    if (optionsMap.count(SEMANTICS_DEBUG_KEY)) {
        builder.SetSemanticsDebug();
    }

    return builder.Build();
}

int main(int argc, char** argv) {
    const Configuration configuration = ParseCommandLineArgs(argc, argv);
    if (configuration.GetPaths().empty()) {
        return 0;
    }

    std::ifstream ifs(configuration.GetPaths()[0]);

    if (configuration.GetLexerDebug()) {
        Lexer lexer(configuration.GetPaths()[0]);
        lexer.NextLexeme();

        while (lexer.GetLexeme().GetType() != Lexeme::LexemeType::EndOfFile) {
            std::cout << lexer.NextLexeme() << std::endl;
        }
    }

    Lexer lexer(configuration.GetPaths()[0]);
    SymbolTable symTable;
    Parser parser(lexer, &symTable);
    Pointer<DeclarationBlock> syntaxTree = parser.Parse();

    if (configuration.GetParserDebug()) {
        std::cout << std::endl;
        CuteToStringVisitor visitor;

        if (configuration.GetSemanticsDebug()) {
            visitor.ShowSemanticsAnnotations();
        }

        syntaxTree->RunVisitor(visitor);

        for (auto& str : visitor.GetStringData()) {
            std::cout << str << std::endl;
        }
    }

    if (configuration.GetSemanticsDebug()) {
        std::cout << std::endl;
        CuteToStringVisitor visitor;
        symTable.RunVisitor(visitor);
        for (auto& str : visitor.GetStringData()) {
            std::cout << str << std::endl;
        }

        std::cout << std::endl;
    }

    for (auto& error : parser.GetParsingErrors()) {
        std::cout << error << std::endl;
    }

    for (auto& error : parser.GetSemanticsErrors()) {
        std::cout << error << std::endl;
    }
    

    if (!parser.GetParsingErrors().empty() || !parser.GetSemanticsErrors().empty()) {
        return 0;
    }

    Interpreter interpreter(syntaxTree.get(), &symTable);
    interpreter.RunMain();
    return 0;
}
