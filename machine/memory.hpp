#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <cstddef>
#include <unordered_set>
#include <stack>

#include "byte_code.hpp"


namespace mvm
{

template <std::size_t N>
struct StackHead : public Stack
{
    StackHead(uint32_t bstack) : _top(&(_data[0])), _bstack(bstack) {}

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

template<std::size_t M, std::size_t N>
struct Memory
{
    Memory(std::size_t sizeB, std::sizeM, std::sizeC1, std::sizeC2) :
        _sizeB(sizeB),
        _sizeM(sizeM),
        _sizeC1(sizeC1),
        _sizeC2(sizeC2)
    {
        _stackC1.reserve(sizeC1);
        _stackC2.reserve(sizeC2);
        _memory.resize(sizeM);
    }

    uint32_t read(uint32_t address) 
    { 
        if (address < _sizeM)
        {
            return _memory[address];
        }
        else
        {
            return 0;
        }
    }

    void write(uint32_t address)
    {
        if (address < _sizeM)
        {
            _memory[address] = *(_cStackHeadPtr->_top);
        }
    }

    void cpush() 
    {
        if (_stackC1.size() == sizeC1)
        {
            return;
        }

        _stackC1.push_back(); 
    }

    void opush(uint32_t value)
    {
        if (_cStackHeadPtr->_top == _cStackHeadPtr->_top  + M || 
            _cStackHeadPtr->_top == &(_cStackBlockPtr->_data[0]))
        {

            if (_stackC2.size() == sizeC2)
            {
                return;
            }

            StackBlock* newStackBlock = new StackBlock(_cStackBlockPtr, value);
            _stackC2.insert(newStackBlock);
            _cStackBlockPtr = newStackBlock;
            _cStackBlockPtr->_data[0] = value;
            _cStackHeadPtr->_top = &(_cStackBlockPtr->_data[0]);
        }

        *((_cStackHeadPtr->_top)++) = value;
    }

    uint32_t cpop(uint32_t nToPop) 
    {
        if (nToPop < _stackC1.size())
        {
            uint32_t result = _stackC1[_stackC1.size() - nToPop]._data[0];
            _stackC1.resize(_stackC1.size() - nToPop);
        }
        return result;
    }

    uint32_t opop()
    {
        if (_cStackHeadPtr->_top == &(_cStackBlockPtr->_data[0]))
        {
            uint32_t result = _cStackBlockPtr->_data[0];
            StackBlock* newStackBlock = _cStackBlockPtr->previous;
            _stackC2.erase(_cStackBlockPtr);
            _cStackBlockPtr = newStackBlock;
            _cStackHeadPtr->top = &(_cStackBlockPtr->_data[M - 1]);
            return result;
        }

        if (_cStackHeadPtr->top != &(_cStackHeadPtr->data[0]))
        {
            return *((_cStackHeadPtr->top)--);
        }
        else
        {
            return 0;
        }
    }

    uint32_t* otop()
    {
        return _cStackHeadPtr->_top;
    }

    uint32_t    _bstack;
    StackHead* _cStackHeadPtr;
    StackBlock* _cStackBlockPtr;

    std::vector<StackHead<M>>           _stackC1;
    std::vector<uint32_t>               _memory;
    std::unordered_set<StackBlock<N>*>  _stackC2;

    const std::size_t _sizeB;
    const std::size_t _sizeM;
    const std::size_t _sizeC1;
    const std::size_t _sizeC2;
};

} 

#endif // end __MEMORY_H__
