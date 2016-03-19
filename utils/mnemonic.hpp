#ifndef __MNEMONIC_H__
#define __MNEMONIC_H__

//STL
#include <unordered_map>
#include <cstddef>

namespace sk2
{

static const uint32_t MNEMONIC_MASK = 0xff;

struct Mnemonic
{
    enum : uint32_t
    {
        // About integers
        IADD = 0,
        ISUB,
        IMUL,
        IDIV,
        IMOD,
        INEG,

        INOT,
        IAND,
        IXOR,
        IOR,

        ILSH,
        IRSH,

        // About floats
        FADD,
        FSUB,
        FMUL,
        FDIV,
        FNEG,

        FISN,
        FASN,
        FEXP,
        FLOG,

        // Unique and singular jump
        JUMP,

        // About bools
        BILT,
        BIGT,
        BILE,
        BIGE,
        BFLT,
        BFGT,
        BFLE,
        BFGE,
        BEQL,
        BAND,
        BNOT,
        BXOR,
        BOR,
        
        BPOP,
        BPSH,
        
        // About stack(s)
        OPOP,
        OPSH,

        CGSK,
        CPSK,

        CALL,
        RET,

        // Module system
        CSYS,

        // About memory
        WRITE,
        READ,

        HALT
    };

    static const uint32_t NOTE_SHIFT = 0x10;

    struct Note
    {
        enum
        {
            IS_I        = 0b1           << NOTE_SHIFT,
            IS_F        = 0b10          << NOTE_SHIFT,
            IS_B        = 0b100         << NOTE_SHIFT,
            IS_E        = 0b1000        << NOTE_SHIFT,
            IS_I_A      = 0b10000       << NOTE_SHIFT,
            IS_I_L      = 0b100000      << NOTE_SHIFT,
            IS_F_A      = 0b1000000     << NOTE_SHIFT,
            IS_F_M      = 0b10000000    << NOTE_SHIFT,
            IS_B_I      = 0b100000000   << NOTE_SHIFT,
            IS_B_F      = 0b1000000000  << NOTE_SHIFT,
            IS_B_E      = 0b10000000000 << NOTE_SHIFT
        };

        operator uint32_t() const { return _value; }

        uint32_t _value;
    };

    Mnemonic(uint32_t value) : _value(value)
    {
        uint32_t tmp = _value & MNEMONIC_MASK;

        if (tmp <= IRSH)
        {
            _value |= Note::IS_I;

            if (tmp <= INEG)
            {
                _value |= Note::IS_I_A;
            }
            else
            {
                _value |= Note::IS_I_L;
            }
        } 
        else if (tmp <= FLOG)
        {
            _value |= Note::IS_F;

            if (tmp <= FNEG)
            {
                _value |= Note::IS_F_A;
            }
            else
            {
                _value |= Note::IS_F_M;
            }
        } 
        else if (tmp <= BOR)
        {
            _value |= Note::IS_B;

            if (tmp <= BIGE)
            {
                _value |= Note::IS_B_I;
            }
            else if (tmp <= BFGE)
            {
                _value |= Note::IS_B_F;
            }
            else
            {
                _value |= Note::IS_B_E;
            }
        }
        else
        {
            _value |= Note::IS_E;
        }
    }

    operator uint32_t() const { return _value; }

    uint32_t _value;

};

} // end sk2 namespace

#endif // end __MNEMONIC_H__
