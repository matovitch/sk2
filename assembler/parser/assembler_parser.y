%include
{
    #include <string>
    #include <cassert>

    #include "assembler/lexer/assembler_lexer.hpp"
    #include "assembler/assembler_program.hpp"
    #include "utils/instruction.hpp"
    #include "utils/mnemonic.hpp"
    #include "parser_utils.hpp"
}

%name assembler_parser

%extra_argument { mvm::AssemblerProgram* state }

%token_prefix ASM_TOKEN_

%token_type             { quex::Token* }

%type dot               { std::string* }
%type sign              { std::string* }
%type label             { std::string* }
%type number            { std::string* }
%type exponent          { std::string* }
%type integer_s         { std::string* }
%type partial_float_s   { std::string* }
%type float_s           { std::string* }

%type integer           { int }
%type register_o        { int }
%type register_u        { int }

%type float             { float }

%type mnemonic_L        { mvm::Mnemonic* }
%type mnemonic_I        { mvm::Mnemonic* }
%type mnemonic_VIOU     { mvm::Mnemonic* }
%type mnemonic_VFOU     { mvm::Mnemonic* }
%type mnemonic_IFOU     { mvm::Mnemonic* }
%type mnemonic_VIFOU    { mvm::Mnemonic* }

%type instruction       { mvm::Instruction* }
%type instruction_L     { mvm::Instruction* }
%type instruction_I     { mvm::Instruction* }
%type instruction_VIOU  { mvm::Instruction* }
%type instruction_VFOU  { mvm::Instruction* }
%type instruction_IFOU  { mvm::Instruction* }
%type instruction_VIFOU { mvm::Instruction* }

axiom ::= program.

dot(s) ::= DOT. { s = new std::string("."); }

exponent(s) ::= EXPONENT(t). { s = toString(t); }

number(s) ::= NUMBER(t).     { s = toString(t); }

label(s) ::= LABEL(t).       { s = toString(t); }

sign(s) ::= SIGN(t).         { s = toString(t); }

register_o(i) ::= DOLLAR number(a).  { i = toInt(a); }

register_u(i) ::= PERCENT number(a). { i = toInt(a); }

integer_s(s) ::= number(a).         { s = a; }
integer_s(s) ::= sign(a) number(b). { s = concat(a, b); }

integer(i) ::= integer_s(a). { i = toInt(a); }

partial_float_s(s) ::= dot(a) number(b).              { s = concat(a, b); }
partial_float_s(s) ::= integer_s(a) dot(b).           { s = concat(a, b); }
partial_float_s(s) ::= sign(a) dot(b) number(c).      { s = concat(a, b, c); }
partial_float_s(s) ::= integer_s(a) dot(b) number(c). { s = concat(a, b, c); }

float_s(s) ::= partial_float_s(a).                          { s = a; }
float_s(s) ::= partial_float_s(a) exponent(b) integer_s(c). { s = concat(a, b, c); }

float(f) ::= float_s(a). { f = toFloat(a); }

mnemonic_L(m) ::= JUMP(a).  { m = mnemonicFromToken(a); }
mnemonic_L(m) ::= CALL(a).  { m = mnemonicFromToken(a); }

mnemonic_I(m) ::= WRITE(a). { m = mnemonicFromToken(a); }
mnemonic_I(m) ::= READ (a). { m = mnemonicFromToken(a); }
mnemonic_I(m) ::= CSYS (a). { m = mnemonicFromToken(a); }
mnemonic_I(m) ::= CGSK (a). { m = mnemonicFromToken(a); }
mnemonic_I(m) ::= CPSK (a). { m = mnemonicFromToken(a); }
mnemonic_I(m) ::= BPOP (a). { m = mnemonicFromToken(a); }
mnemonic_I(m) ::= OPOP (a). { m = mnemonicFromToken(a); }
mnemonic_I(m) ::= RET  (a). { m = mnemonicFromToken(a); }

mnemonic_VIOU(m) ::= IADD(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= ISUB(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= IMUL(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= IDIV(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= IMOD(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= INEG(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= INOT(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= IAND(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= IXOR(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= IOR (a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= ILSH(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= IRSH(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= BILT(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= BIGT(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= BILE(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= BIGE(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= BAND(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= BNOT(a). { m = mnemonicFromToken(a); }
mnemonic_VIOU(m) ::= BOR (a). { m = mnemonicFromToken(a); }

mnemonic_VFOU(m) ::= FADD(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= FSUB(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= FMUL(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= FDIV(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= FNEG(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= FISN(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= FASN(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= FEXP(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= FLOG(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= BFLT(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= BFGT(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= BFLE(a). { m = mnemonicFromToken(a); }
mnemonic_VFOU(m) ::= BFGE(a). { m = mnemonicFromToken(a); }

mnemonic_IFOU(m) ::= BPSH(a). { m = mnemonicFromToken(a); }
mnemonic_IFOU(m) ::= OPSH(a). { m = mnemonicFromToken(a); }

mnemonic_VIFOU(m) ::= BEQL(a). { m = mnemonicFromToken(a); }
mnemonic_VIFOU(m) ::= BXOR(a). { m = mnemonicFromToken(a); }

instruction_L(i) ::= mnemonic_L(a) label(b).   { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_L); }

instruction_I(i) ::= mnemonic_I(a) integer(b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_I); }

instruction_VIOU(i) ::= mnemonic_VIOU(a)              . { i = instructionFromMnemonic(a, 0, mvm::Instruction::Note::IS_V); }
instruction_VIOU(i) ::= mnemonic_VIOU(a) integer   (b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_I); }
instruction_VIOU(i) ::= mnemonic_VIOU(a) register_o(b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_O); }
instruction_VIOU(i) ::= mnemonic_VIOU(a) register_u(b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_U); }

instruction_VFOU(i) ::= mnemonic_VFOU(a)              . { i = instructionFromMnemonic(a, 0, mvm::Instruction::Note::IS_V); }
instruction_VFOU(i) ::= mnemonic_VFOU(a) float     (b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_F); }
instruction_VFOU(i) ::= mnemonic_VFOU(a) register_o(b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_O); }
instruction_VFOU(i) ::= mnemonic_VFOU(a) register_u(b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_U); }

instruction_IFOU(i) ::= mnemonic_IFOU(a) integer   (b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_I); }
instruction_IFOU(i) ::= mnemonic_IFOU(a) float     (b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_F); }
instruction_IFOU(i) ::= mnemonic_IFOU(a) register_o(b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_O); }
instruction_IFOU(i) ::= mnemonic_IFOU(a) register_u(b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_U); }

instruction_VIFOU(i) ::= mnemonic_VIFOU(a)              . { i = instructionFromMnemonic(a, 0, mvm::Instruction::Note::IS_V); }
instruction_VIFOU(i) ::= mnemonic_VIFOU(a) integer   (b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_I); }
instruction_VIFOU(i) ::= mnemonic_VIFOU(a) float     (b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_F); }
instruction_VIFOU(i) ::= mnemonic_VIFOU(a) register_o(b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_O); }
instruction_VIFOU(i) ::= mnemonic_VIFOU(a) register_u(b). { i = instructionFromMnemonic(a, b, mvm::Instruction::Note::IS_U); }

instruction(i) ::=  instruction_I    (a). { i = a; }
instruction(i) ::=  instruction_L    (a). { i = a; }
instruction(i) ::=  instruction_VIOU (a). { i = a; }
instruction(i) ::=  instruction_VFOU (a). { i = a; }
instruction(i) ::=  instruction_IFOU (a). { i = a; }
instruction(i) ::=  instruction_VIFOU(a). { i = a; }

program ::= program instruction(a). { addInstruction(state, a); }
program ::= program label      (a). { addLabel(state, a); }
program ::= .

%nonassoc FAILURE END_OF_FILE UNINITIALIZED TERMINATION.

%syntax_error { state->setParserError(); }

%destructor dot               { delete $$; }
%destructor sign              { delete $$; }
%destructor label             { delete $$; }
%destructor number            { delete $$; }
%destructor exponent          { delete $$; }
%destructor integer_s         { delete $$; }
%destructor partial_float_s   { delete $$; }
%destructor float_s           { delete $$; }

%destructor mnemonic_L        { delete $$; }
%destructor mnemonic_I        { delete $$; }
%destructor mnemonic_VIOU     { delete $$; }
%destructor mnemonic_VFOU     { delete $$; }
%destructor mnemonic_IFOU     { delete $$; }
%destructor mnemonic_VIFOU    { delete $$; }

%destructor instruction       { delete $$; }
%destructor instruction_L     { delete $$; }
%destructor instruction_I     { delete $$; }
%destructor instruction_VIOU  { delete $$; }
%destructor instruction_VFOU  { delete $$; }
%destructor instruction_IFOU  { delete $$; }
%destructor instruction_VIFOU { delete $$; }