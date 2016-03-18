#ifndef __ASSEMBLER_PROGRAM_H__
#define __ASSEMBLER_PROGRAM_H__

//STL
#include <unordered_map>
#include <vector>
#include <cstddef>
#include <fstream>
#include <ios>

//MVM
#include "lexer/assembler_lexer.hpp"
#include "utils/instruction.hpp"

#include <iostream>

namespace mvm
{

class AssemblerProgram
{
public:

    AssemblerProgram() :
        _lexerError(false),
        _parserError(false),
        _lastValidToken (NULL) {}

    AssemblerProgram* addLabel(const std::string& label)
    {
        _labelToAddress[label] = _instructions.size();
        return this;
    }

    AssemblerProgram* addInstruction(const Instruction& instruction)
    {
        _instructions.push_back(instruction);
        return this;
    }

    void write(const std::string& filePath)
    {
        std::ofstream ofstream(filePath.c_str(), std::ios::out | std::ios::binary);

        std::vector<uint32_t> tmp;

        for (const Instruction& i : _instructions)
        {
            tmp.push_back(i._mnemonic);

            if (i._label.empty())
            {               
                tmp.push_back(i._argument);
            }
            else
            {
                tmp.push_back(_labelToAddress.at(i._label));
            }
        }

        ofstream.write(reinterpret_cast<const char*>(&tmp[0]), tmp.size() * sizeof(uint32_t));
    }

    bool hasLexerError() { return _lexerError; }

    bool hasParserError() { return _parserError; }

    void setParserError() { _parserError = true; }

    void saveLastValidToken(quex::Token* token) 
    {
        _lastValidToken = token;
        _lexerError = !_parserError;
    }

    quex::Token* getLastValidToken() { return _lastValidToken; }

private:

    bool _lexerError;
    bool _parserError;
    quex::Token* _lastValidToken;
    std::vector<Instruction> _instructions;
    std::unordered_map<std::string, uint32_t> _labelToAddress;
};


} // end mvm namespace



#endif // end __ASSEMBLER_PROGRAM_H__