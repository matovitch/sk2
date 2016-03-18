#include <iostream>
#include <string>
#include <map>

#include "docopt.cpp/docopt.h"

#include "assembler/assembler.hpp"

static const char USAGE[] =
R"(SK2 Assembler.

    Usage:
        sk2asm [-hv -o <output> -t <traces>] <input>

    Options:
        -o, --output   Name of the output file.
        -t, --traces   Lemon parsing traces.
        -h, --help     Show this screen.
        -v, --version  Show version number.
)";

int main(int argc, const char** argv)
{
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
                                                               { argv + 1, argv + argc },
                                                               true,
                                                               "SK2 Assembler 0.1");

    mvm::AssemblerParser assemblerParser;

    std::shared_ptr<mvm::AssemblerProgram> program = 
        assemblerParser.parse(args["<input>"].asString(),
                              args["<traces>"] ? args["<traces>"].asString() : "");

    if (program->hasLexerError() || program->hasParserError())
    {
        mvm::AssemblerLexer::token_type* lastValidToken = program->getLastValidToken();

        if (lastValidToken != NULL)
        {
            std::cout << "ERROR : last valid token is " << lastValidToken->type_id_name()  << ":"
                                                        << lastValidToken->line_number()   << ","
                                                        << lastValidToken->column_number() << "."
                                                        << std::endl;
        }
    }

    program->write((args["--output"]) ? args["<output>"].asString() : "a.sk2bin");

    return 0;
}