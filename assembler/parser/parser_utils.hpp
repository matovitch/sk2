#ifndef __PARSER_UTILS_H__
#define __PARSER_UTILS_H__

#include <unordered_map>
#include <cstddef>

#include "assembler/lexer/assembler_lexer.hpp"
#include "utils/instruction.hpp"
#include "utils/mnemonic.hpp"

static const std::unordered_map<uint32_t, uint32_t> TOKEN_TO_MNEMONIC =
{
    { ASM_TOKEN_IADD,   mvm::Mnemonic::IADD  },
    { ASM_TOKEN_ISUB,   mvm::Mnemonic::ISUB  },
    { ASM_TOKEN_IMUL,   mvm::Mnemonic::IMUL  },
    { ASM_TOKEN_IDIV,   mvm::Mnemonic::IDIV  },
    { ASM_TOKEN_IMOD,   mvm::Mnemonic::IMOD  },
    { ASM_TOKEN_INEG,   mvm::Mnemonic::INEG  },
    { ASM_TOKEN_INOT,   mvm::Mnemonic::INOT  },
    { ASM_TOKEN_IAND,   mvm::Mnemonic::IAND  },
    { ASM_TOKEN_IXOR,   mvm::Mnemonic::IXOR  },
    { ASM_TOKEN_IOR,    mvm::Mnemonic::IOR   },
    { ASM_TOKEN_ILSH,   mvm::Mnemonic::ILSH  },
    { ASM_TOKEN_IRSH,   mvm::Mnemonic::IRSH  },
    { ASM_TOKEN_FADD,   mvm::Mnemonic::FADD  },
    { ASM_TOKEN_FSUB,   mvm::Mnemonic::FSUB  },
    { ASM_TOKEN_FMUL,   mvm::Mnemonic::FMUL  },
    { ASM_TOKEN_FDIV,   mvm::Mnemonic::FDIV  },
    { ASM_TOKEN_FNEG,   mvm::Mnemonic::FNEG  },
    { ASM_TOKEN_FISN,   mvm::Mnemonic::FISN  },
    { ASM_TOKEN_FASN,   mvm::Mnemonic::FASN  },
    { ASM_TOKEN_FEXP,   mvm::Mnemonic::FEXP  },
    { ASM_TOKEN_FLOG,   mvm::Mnemonic::FLOG  },
    { ASM_TOKEN_JUMP,   mvm::Mnemonic::JUMP  },
    { ASM_TOKEN_BEQL,   mvm::Mnemonic::BEQL  },
    { ASM_TOKEN_BILT,   mvm::Mnemonic::BILT  },
    { ASM_TOKEN_BIGT,   mvm::Mnemonic::BIGT  },
    { ASM_TOKEN_BILE,   mvm::Mnemonic::BILE  },
    { ASM_TOKEN_BIGE,   mvm::Mnemonic::BIGE  },
    { ASM_TOKEN_BFLT,   mvm::Mnemonic::BFLT  },
    { ASM_TOKEN_BFGT,   mvm::Mnemonic::BFGT  },
    { ASM_TOKEN_BFLE,   mvm::Mnemonic::BFLE  },
    { ASM_TOKEN_BFGE,   mvm::Mnemonic::BFGE  },
    { ASM_TOKEN_BAND,   mvm::Mnemonic::BAND  },
    { ASM_TOKEN_BNOT,   mvm::Mnemonic::BNOT  },
    { ASM_TOKEN_BXOR,   mvm::Mnemonic::BXOR  },
    { ASM_TOKEN_BOR,    mvm::Mnemonic::BOR   },
    { ASM_TOKEN_BPOP,   mvm::Mnemonic::BPOP  },
    { ASM_TOKEN_BPSH,   mvm::Mnemonic::BPSH  },
    { ASM_TOKEN_OPOP,   mvm::Mnemonic::OPOP  },
    { ASM_TOKEN_OPSH,   mvm::Mnemonic::OPSH  },
    { ASM_TOKEN_CGSK,   mvm::Mnemonic::CGSK  },
    { ASM_TOKEN_CPSK,   mvm::Mnemonic::CPSK  },
    { ASM_TOKEN_CALL,   mvm::Mnemonic::CALL  },
    { ASM_TOKEN_RET,    mvm::Mnemonic::RET   },
    { ASM_TOKEN_CSYS,   mvm::Mnemonic::CSYS  },
    { ASM_TOKEN_WRITE,  mvm::Mnemonic::WRITE },
    { ASM_TOKEN_READ,   mvm::Mnemonic::READ  },
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

static mvm::Mnemonic* mnemonicFromToken(quex::Token* token)
{
    return new mvm::Mnemonic(TOKEN_TO_MNEMONIC.at(token->type_id()));
}

static void addInstruction(mvm::AssemblerProgram* program, mvm::Instruction* instruction)
{
    program->addInstruction(*instruction);
    delete instruction;
}

static void addLabel(mvm::AssemblerProgram* program, std::string* label)
{
    program->addLabel(*label);
    delete label;
}

template <typename T>
static mvm::Instruction* instructionFromMnemonic(mvm::Mnemonic* mnemonic, T arg, uint32_t note)
{
    mvm::Instruction* instruction = new mvm::Instruction(*mnemonic | note, arg);

    delete mnemonic;

    return instruction;
}

template <>
mvm::Instruction* instructionFromMnemonic(mvm::Mnemonic* mnemonic, std::string* arg, uint32_t note)
{
    std::shared_ptr<std::string> ptr(arg);
    return instructionFromMnemonic(mnemonic, *arg, note);
}


#endif // end __PARSER_UTILS_H__