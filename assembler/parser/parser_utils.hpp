#ifndef __PARSER_UTILS_H__
#define __PARSER_UTILS_H__

#include <unordered_map>
#include <cstddef>

#include "assembler/lexer/assembler_lexer.hpp"
#include "utils/instruction.hpp"
#include "utils/mnemonic.hpp"

static const std::unordered_map<uint32_t, uint32_t> TOKEN_TO_MNEMONIC =
{
    { ASM_TOKEN_IADD,   sk2::Mnemonic::IADD  },
    { ASM_TOKEN_ISUB,   sk2::Mnemonic::ISUB  },
    { ASM_TOKEN_IMUL,   sk2::Mnemonic::IMUL  },
    { ASM_TOKEN_IDIV,   sk2::Mnemonic::IDIV  },
    { ASM_TOKEN_IMOD,   sk2::Mnemonic::IMOD  },
    { ASM_TOKEN_INEG,   sk2::Mnemonic::INEG  },
    { ASM_TOKEN_INOT,   sk2::Mnemonic::INOT  },
    { ASM_TOKEN_IAND,   sk2::Mnemonic::IAND  },
    { ASM_TOKEN_IXOR,   sk2::Mnemonic::IXOR  },
    { ASM_TOKEN_IOR,    sk2::Mnemonic::IOR   },
    { ASM_TOKEN_ILSH,   sk2::Mnemonic::ILSH  },
    { ASM_TOKEN_IRSH,   sk2::Mnemonic::IRSH  },
    { ASM_TOKEN_FADD,   sk2::Mnemonic::FADD  },
    { ASM_TOKEN_FSUB,   sk2::Mnemonic::FSUB  },
    { ASM_TOKEN_FMUL,   sk2::Mnemonic::FMUL  },
    { ASM_TOKEN_FDIV,   sk2::Mnemonic::FDIV  },
    { ASM_TOKEN_FNEG,   sk2::Mnemonic::FNEG  },
    { ASM_TOKEN_FISN,   sk2::Mnemonic::FISN  },
    { ASM_TOKEN_FASN,   sk2::Mnemonic::FASN  },
    { ASM_TOKEN_FEXP,   sk2::Mnemonic::FEXP  },
    { ASM_TOKEN_FLOG,   sk2::Mnemonic::FLOG  },
    { ASM_TOKEN_JUMP,   sk2::Mnemonic::JUMP  },
    { ASM_TOKEN_BEQL,   sk2::Mnemonic::BEQL  },
    { ASM_TOKEN_BILT,   sk2::Mnemonic::BILT  },
    { ASM_TOKEN_BIGT,   sk2::Mnemonic::BIGT  },
    { ASM_TOKEN_BILE,   sk2::Mnemonic::BILE  },
    { ASM_TOKEN_BIGE,   sk2::Mnemonic::BIGE  },
    { ASM_TOKEN_BFLT,   sk2::Mnemonic::BFLT  },
    { ASM_TOKEN_BFGT,   sk2::Mnemonic::BFGT  },
    { ASM_TOKEN_BFLE,   sk2::Mnemonic::BFLE  },
    { ASM_TOKEN_BFGE,   sk2::Mnemonic::BFGE  },
    { ASM_TOKEN_BAND,   sk2::Mnemonic::BAND  },
    { ASM_TOKEN_BNOT,   sk2::Mnemonic::BNOT  },
    { ASM_TOKEN_BXOR,   sk2::Mnemonic::BXOR  },
    { ASM_TOKEN_BOR,    sk2::Mnemonic::BOR   },
    { ASM_TOKEN_BPOP,   sk2::Mnemonic::BPOP  },
    { ASM_TOKEN_BPSH,   sk2::Mnemonic::BPSH  },
    { ASM_TOKEN_OPOP,   sk2::Mnemonic::OPOP  },
    { ASM_TOKEN_OPSH,   sk2::Mnemonic::OPSH  },
    { ASM_TOKEN_CGSK,   sk2::Mnemonic::CGSK  },
    { ASM_TOKEN_CPSK,   sk2::Mnemonic::CPSK  },
    { ASM_TOKEN_CALL,   sk2::Mnemonic::CALL  },
    { ASM_TOKEN_RET,    sk2::Mnemonic::RET   },
    { ASM_TOKEN_CSYS,   sk2::Mnemonic::CSYS  },
    { ASM_TOKEN_WRITE,  sk2::Mnemonic::WRITE },
    { ASM_TOKEN_READ,   sk2::Mnemonic::READ  },
    { ASM_TOKEN_HALT,   sk2::Mnemonic::HALT  },
};

static std::string* toString(quex::Token* token)
{
    return new std::string(quex::unicode_to_char(token->text));
}

static int toInt(std::string* string)
{
    int i = std::stoi(*string);
    delete string;
    return i;
}

static float toFloat(std::string* string)
{
    float f = std::stof(*string);
    delete string;
    return f;
}


template<typename T>
static T* concat(T* a) {
    return a;
}

template<typename T, typename... Args>
static T* concat(T* a, Args... args) {
    T* b = concat(args...);
    T* t = new T(*a + *b);
    delete a;
    delete b;
    return t;
}

static sk2::Mnemonic* mnemonicFromToken(quex::Token* token)
{
    return new sk2::Mnemonic(TOKEN_TO_MNEMONIC.at(token->type_id()));
}

static void addInstruction(sk2::AssemblerProgram* program, sk2::Instruction* instruction)
{
    program->addInstruction(*instruction);
    delete instruction;
}

static void addLabel(sk2::AssemblerProgram* program, std::string* label)
{
    program->addLabel(*label);
    delete label;
}

template <typename T>
static sk2::Instruction* instructionFromMnemonic(sk2::Mnemonic* mnemonic, T arg, uint32_t note)
{
    sk2::Instruction* instruction = new sk2::Instruction(*mnemonic | note, arg);

    delete mnemonic;

    return instruction;
}

template <>
sk2::Instruction* instructionFromMnemonic(sk2::Mnemonic* mnemonic, std::string* arg, uint32_t note)
{
    sk2::Instruction* instruction = instructionFromMnemonic(mnemonic, *arg, note);

    delete arg;

    return instruction;
}


#endif // end __PARSER_UTILS_H__