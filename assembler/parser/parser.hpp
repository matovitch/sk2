#ifndef __PARSER_H__
#define __PARSER_H__

#include <memory>
#include <cstdlib>

typedef void* (*ParserAllocFuncType)(void* (*alloc)(size_t));
typedef void  (*ParserFreeFuncType)(void*, void (*free)(void*));
typedef void  (*ParserTraceFuncType)(FILE*, char*);

template <typename QUEX_TOKEN, typename STATE>
struct ParserFuncType
{
 typedef void (*value)(void*, int, QUEX_TOKEN*, STATE*);
};

template <typename LEXER,
          typename STATE,
          ParserAllocFuncType PARSER_ALLOC,
          ParserFreeFuncType PARSER_FREE,
          typename ParserFuncType<typename LEXER::token_type, STATE>::value PARSE,
          ParserTraceFuncType PARSER_TRACE>
class Parser
{

public:

    Parser() : 
        _state(new STATE),
        _parser(NULL)
    {
        _parser = PARSER_ALLOC(malloc);
    }

    std::shared_ptr<STATE> parse(std::string filePath, std::string traceFilePath = "")
    {
        if (!traceFilePath.empty())
        {
            FILE* traceFile = fopen(traceFilePath.c_str(), "w");
            PARSER_TRACE(traceFile, (char*)"");
        }

        LEXER lexer(filePath);

        while (!_state->hasParserError() && lexer.advance())
        {
            PARSE(_parser, lexer.getTokenId(), lexer.getToken(), &(*_state));
        }

        if (_state->hasParserError() || lexer.hasFailed())
        {
            _state->saveLastValidToken(lexer.getToken());
        }

        PARSE(_parser, 0, NULL, &(*_state));

        return _state;
    }

    ~Parser()
    {
        PARSER_FREE(_parser, free);
    }

private:

    std::shared_ptr<STATE> _state;
    void* _parser;
};

#endif // end __PARSER_H__