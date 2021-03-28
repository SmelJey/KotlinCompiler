#include "ConfigurationBuilder.h"
#include "Configuration.h"
#include "Lexer/IncrementalLexer.h"

#include <iostream>
#include <vector>
#include <string>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

namespace prog_opt = boost::program_options;

const char* FILES_KEY = "source-files";
const char* LEXER_DEBUG_KEY = "lexer-debug";

Configuration ParseCommandLineArgs(int argc, char** argv) {
    prog_opt::options_description optionsDesc("Allowed options");
    optionsDesc.add_options()
        ("help", "show help message")
        ("lexer-debug,l", "debug lexical analyser")
        ("source-files,f", prog_opt::value<std::vector<std::string>>(), "source files");

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
    return builder.Build();
}

int main(int argc, char** argv) {
    Configuration configuration = ParseCommandLineArgs(argc, argv);

    std::ifstream ifs(configuration.GetPaths()[0]);

    IncrementalLexer<std::ifstream> lexer(configuration.GetPaths()[0]);

    while (lexer.GetLexeme().GetType() != Lexeme::LexemeType::EndOfFile) {
        if (configuration.GetLexerDebug()) {
            std::cout << lexer.GetLexeme() << std::endl;
        }

        lexer.NextLexeme();
    }
    
    return 0;
}
