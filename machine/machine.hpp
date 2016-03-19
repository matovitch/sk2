#ifndef __MACHINE_H__
#define __MACHINE_H__

#include <cmath>

#include "memory.hpp"

namespace sk2
{

template <std::size_t N, std::size_t M>
struct Machine
{

    Machine(std::size_t sizeB, std::sizeM, std::sizeC1, std::sizeC2) :
        _memory(sizeB, sizeM, sizeC1, sizeC2) {}

    void run()
    {
        const ByteCodeWord& word = _memory.byteCodeWord(memory.instructionPointer());

        const Mnemonic& mnemonic = word._mnemonic;

        const uint32_t mnemonicBare = mnemonic & Mnemonic::MNEMONIC_MASK;

        if (mnemonic & Mnemonic::Note::IS_I)
        {
            if (mnemonic & Instruction::Note::IS_U)
            {
                const uint32_t rid = word._argument;
                const int32_t&  lhs = reinterpret_cast<int32_t&>(_memory.otop(rid));
            }
            else
            {
                const int32_t& lhs = reinterpret_cast<int32_t&>(_memory.otop());
            }

            if (mnemonic & Mnemonic::Note::IS_U)
            {
                if (mnemonicBare == Mnemonic::INEG)
                {
                    lhs -= lhs;
                }
                else
                {
                    lhs -= lhs;
                }

            } 
            else
            {
                if (mnemonic & Instruction::Note::IS_V)
                {
                    const int32_t  rhs = reinterpret_cast<int32_t>(_memory.opop());
                } else
                if (mnemonic & Instruction::Note::IS_O)
                {
                    const uint32_t rid = word._argument;
                    const int32_t  rhs = reinterpret_cast<int32_t>(_memory.otop(rid));
                }
                else
                {
                    const int32_t rhs = reinterpret_cast<int32_t>(word._argument);
                }

                if (mnemonic & Mnemonic::Note::IS_I_A)
                {
                    switch (mnemonicBare)
                    {
                    case Mnemonic::IADD:
                        lhs += rhs;
                        break;
                    case Mnemonic::ISUB:
                        lhs -= rhs;
                        break;
                    case Mnemonic::IMUL:
                        lhs *= rhs;
                        break;
                    case Mnemonic::IDIV:
                        lhs /= rhs;
                        break;
                    default:
                        lhs %= rhs;
                    }
                }
                else
                {
                    switch (mnemonicBare)
                    {
                    case Mnemonic::IRSH:
                        lhs <<= rhs;
                        break;
                    case Mnemonic::ILSH:
                        lhs >>= rhs;
                        break;
                    case Mnemonic::IAND:
                        lhs &= rhs;
                        break;
                    case Mnemonic::IXOR:
                        lhs ^= rhs;
                        break;
                    default:
                        lhs |= rhs;
                    }
                }
            }

        } else 
        if (mnemonic & Mnemonic::Note::IS_F)
        {
            if (mnemonic & Instruction::Note::IS_U)
            {
                const uint32_t rid = word._argument;
                const float&  lhs = reinterpret_cast<float&>(_memory.otop(rid));
            }
            else
            {
                const float& lhs = reinterpret_cast<float&>(_memory.otop());
            }

            if (mnemonic & Mnemonic::Note::IS_U)
            {
                switch (mnemonicBare)
                {
                case Mnemonic::FNEG:
                    lhs -= lhs;
                    break;
                case Mnemonic::FISN:
                    lhs = sin(lhs);
                    break;
                case Mnemonic::FASN:
                    lhs = (lhs >= 0.0) ? std::asin(std::modf(lhs, 1.0)) :
                                         std::asin(std::modf(lhs, -1.0));
                    break;
                case Mnemonic::FEXP:
                    lhs = std::exp(lhs);
                    break;
                default:
                    lhs = (lhs > 0.0) ? std::log(lhs) : 0.0;
                }
                _memory.otop() = -(reinterpret_cast<float>(_memory.otop()));
            }
        }
        else
        {
            if (mnemonic & Instruction::Note::IS_V)
            {
                const float  rhs = reinterpret_cast<float>(_memory.opop());
            } else
            if (mnemonic & Instruction::Note::IS_O)
            {
                const ufloat rid = word._argument;
                const float  rhs = reinterpret_cast<float>(_memory.otop(rid));
            }
            else
            {
                const float rhs = reinterpret_cast<float>(word._argument);
            }

            switch (mnemonicBare)
            {
            case Mnemonic::FADD:
                lhs += rhs;
                break;
            case Mnemonic::FSUB:
                lhs -= rhs;
                break;
            case Mnemonic::FMUL:
                lhs *= rhs;
                break;
            default Mnemonic::FDIV:
                lhs /= rhs;
                break;
            }
        }
    }

    Memory<N, M> _memory;
};

} // end sk2 namespace

#endif // end __MACHINE_H__