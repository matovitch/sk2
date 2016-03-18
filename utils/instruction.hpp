#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

//STL
#include <cstddef>
#include <string>
#include <memory>

//MVM
#include "mnemonic.hpp"

namespace mvm
{

struct Instruction
{

    static const uint32_t NOTE_SHIFT = 0x08;

    enum Note : uint32_t
    {
        IS_L        = 0b1                   << NOTE_SHIFT,
        IS_I        = 0b10                  << NOTE_SHIFT,
        IS_F        = 0b100                 << NOTE_SHIFT,
        IS_V        = 0b1000                << NOTE_SHIFT,
        IS_O        = 0b10000               << NOTE_SHIFT,
        IS_U        = 0b100000              << NOTE_SHIFT
    };


    Instruction(Mnemonic mnemonic) :
        _mnemonic(mnemonic),
        _argument(0) {}

    Instruction(Mnemonic mnemonic, int argument) :
        _mnemonic(mnemonic),
        _argument(reinterpret_cast<uint32_t&>(argument)) {}

    Instruction(Mnemonic mnemonic, float argument) :
        _mnemonic(mnemonic),
        _argument(reinterpret_cast<uint32_t&>(argument)) {}

    Instruction(Mnemonic mnemonic, std::string& label) :
        _mnemonic(mnemonic),
        _argument(0),
        _label(label) {}

    Instruction(const Instruction& rhs) :
        _mnemonic(rhs._mnemonic),
        _argument(rhs._argument),
        _label(rhs._label) {}

    Mnemonic _mnemonic;
    uint32_t _argument;
    std::string _label;
};

} // end mvm namespace


#endif // end __INSTRUCTION_H__
