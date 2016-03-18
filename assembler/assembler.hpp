#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include "parser/assembler_parser.c"
#include "parser/parser.hpp"

#include "lexer/assembler_lexer.hpp"
#include "lexer/lexer.hpp"

#include "assembler_program.hpp"

namespace mvm
{

typedef Lexer<quex::assembler_lexer,
              quex::Token,
              ASM_TOKEN_FAILURE,
              ASM_TOKEN_END_OF_FILE> AssemblerLexer;

typedef Parser<AssemblerLexer,
               AssemblerProgram,
               assembler_parserAlloc,
               assembler_parserFree,
               assembler_parser,
               assembler_parserTrace> AssemblerParser;

} //end mvm namespace

#endif // end __ASSEMBLER_H__