#ifndef __LEXER_H__
#define __LEXER_H__

#include <cstddef>

template <typename QUEX_LEXER,
          typename QUEX_TOKEN, 
          std::size_t QUEX_ERR, 
          std::size_t QUEX_EOF>
class Lexer
{

public:

    typedef QUEX_LEXER lexer_type;
    typedef QUEX_TOKEN token_type;

    enum
    {
        err_token_id = QUEX_ERR,
        eof_token_id = QUEX_EOF
    };

    Lexer(std::string filePath) : 
        _lexer(filePath), 
        _token(NULL), 
        _error(false) {}

    bool advance() 
    {
        _lexer.receive(&_token);

        if (_token->type_id() == QUEX_ERR)
        {
            _error = true;
            return false;
        }

        if (_token->type_id() == QUEX_EOF)
        {
            return false;
        }

        return true;
    }

    const std::size_t getTokenId() const { return _token->type_id(); }

    QUEX_TOKEN* getToken() const { return _token; }

    bool hasFailed() { return _error; }

private:

    QUEX_LEXER      _lexer;
    QUEX_TOKEN*     _token;
    bool            _error;
};


#endif // end  __LEXER_H__