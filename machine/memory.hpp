#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <cstddef>
#include <unordered_set>
#include <stack>

#include "utils/mnemonic.hpp"

namespace sk2
{

template <std::size_t N>
struct StackHead : public Stack
{
    StackHead() : _top(&(_data[0])) {}

    uint32_t* _top;
    std::array<uint32_t, N> _data;
};

template <std::size_t N>
struct StackBlock : public Stack
{
    StackBlock(uint32_t* previous, uint32_t element) : 
        _previous(previous),
    {
        data[0] = element;
    }

    uint32_t* _previous;
    std::array<uint32_t, N> _data;
};

struct ByteCodeWord
{
    ByteCodeWord(uint64_t word) : 
        _mnemonic(word >> 0x20), 
        _argument(word  % 0x20) {}

    ByteCodeWord(Mnemonic mnemonic, uint32_t argument) : 
        _mnemonic(mnemonic), 
        _argument(argument) {}

    Mnemonic _mnemonic;
    uint32_t _argument;
};

template<std::size_t M, std::size_t N>
struct Memory
{

    enum Error
    {
        B_STACK_OVERFLOW = 1,
        C1_STACK_OVERFLOW,
        C1_STACK_UNDERFLOW,
        C2_STACK_OVERFLOW,
        C2_STACK_UNDERFLOW,
        RAM_OUT_OF_BOUND_READ,
        RAM_OUT_OF_BOUND_WRITE.
        PROGRAM_OUT_OF_BOUND_READ
    };

    Memory(std::size_t sizeB, std::sizeM, std::sizeC1, std::sizeC2) :
        _sizeB(sizeB),
        _sizeM(sizeM),
        _sizeC1(sizeC1),
        _sizeC2(sizeC2)
    {
        _stackC1.reserve(sizeC1);
        _stackC2.reserve(sizeC2);
        _memory.resize(sizeM);
        init();
    }

    void init()
    {
        _byteCode.push_back(ByteCodeWord(Mnemonic(Mnemonic::HALT), 0));

        _instrIdx = 0;

        _stackC1.clear();
        _stackC2.clear();

        _stackC1.push_back(StackHead());
        _stackC1 = &(_stackC1[0]);
        _stackC2 = static_cast<StackBlock*>(_sizeC1);
    }

    uint32_t& read(uint32_t address) 
    { 
        if (address < _sizeM)
        {
            return _memory[address];
        }
        else
        {
            _error = Error::RAM_OUT_OF_BOUND_READ;
            return 0;
        }
    }

    void write(uint32_t address)
    {
        if (address < _sizeM)
        {
            _memory[address] = *(_cStackHeadPtr->_top);
        }
        else
        {
            _error = Error::RAM_OUT_OF_BOUND_WRITE;
        }
    }

    void cpush() 
    {
        if (_stackC1.size() == sizeC1)
        {
            _error = Error::C1_STACK_OVERFLOW;
            return;
        }

        _stackC1.push_back(StackHead());

        _stackC1++;
        _stackC2 = static_cast<StackBlock*>(_sizeC1);
    }

    void opush(uint32_t value)
    {
        if (_cStackHeadPtr->_top == _cStackHeadPtr->_data + N - 1 || 
            _cStackHeadPtr->_top == _cStackBlockPtr->_data + M - 1)
        {

            if (_stackC2.size() == sizeC2)
            {
                _error = Error::C2_STACK_OVERFLOW;
                return;
            }

            StackBlock* newStackBlock = new StackBlock(_cStackBlockPtr, value);
            _stackC2.insert(newStackBlock);
            _cStackBlockPtr = newStackBlock;
            _cStackHeadPtr->_top = _cStackBlockPtr->_data;
        }

        *((_cStackHeadPtr->_top)++) = value;
    }

    uint32_t cpop(uint32_t nToPop) 
    {
        if (nToPop < _stackC1.size())
        {
            _stackC1.resize(_stackC1.size() - nToPop);
            return *(_stackC1[_stackC1.size() - nToPop].top);
        }
        else
        {
            _error = Error::C1_STACK_UNDERFLOW;
            return 0;
        }
    }

    uint32_t opop()
    {
        if (_cStackHeadPtr->_top == _cStackBlockPtr->_data))
        {
            uint32_t result = _cStackBlockPtr->_data[0];
            StackBlock* newStackBlock = _cStackBlockPtr->previous;
            _stackC2.erase(_cStackBlockPtr);
            _cStackBlockPtr = newStackBlock;
            _cStackHeadPtr->top = _cStackBlockPtr->_data + M - 1;
            return result;
        }

        if (_cStackHeadPtr->top != _cStackHeadPtr->data)
        {
            return *((_cStackHeadPtr->top)--);
        }
        else
        {
            _error = C2_STACK_UNDERFLOW;
            return 0;
        }
    }

    uint32_t& otop()
    {
        return _cStackHeadPtr->_top;
    }

    void bpush(uint32_t value)
    {
        _bstack = (_bstack << 1) | value;

        if (reinterpred_cast<int32_t>(_bstack) < 0)
        {
            _error = Error::B_STACK_OVERFLOW;
        }
    }

    uint32_t bpop()
    {
        uint32_t result = _bstack & 1;
        _bstack >>= 1;
        return result;
    }

    void loadByteCode(const std::string& filePath)
    {
        _byteCode.clear();

        std::ifstream ifstream(filePath.c_str(), std::ios::in | std::ios::binary);
        _byteCode = std::vector<ByteCodeWord>(std::istreambuf_iterator<uint64_t>(ifstream),
                                              std::istreambuf_iterator<uint64_t>());

        init();
    }

    uint32_t& instructionPointer()
    {
        return _instrIdx;
    }

    ByteCodeWord& byteCode(uint32_t index)
    {
        if (index < _byteCode.size())
        {
            return _byteCode[index];
        }
        else
        {
            _error = PROGRAM_OUT_OF_BOUND_READ;
            return _byteCode[0];
        }
    }

    uint32_t        _bstack;
    uint32_t        _instrIdx;
    StackHead*      _cStackHeadPtr;
    StackBlock*     _cStackBlockPtr;

    std::vector<ByteCodeWord>           _byteCode;
    std::vector<StackHead<N>>           _stackC1;
    std::vector<uint32_t>               _memory;
    std::unordered_set<StackBlock<M>*>  _stackC2;

    const std::size_t _sizeB;
    const std::size_t _sizeM;
    const std::size_t _sizeC1;
    const std::size_t _sizeC2;

    Error _error;
};

} 

#endif // end __MEMORY_H__
