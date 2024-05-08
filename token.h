#ifndef TOKEN_H_
#define TOKEN_H_
#include <string>

enum TokenType
{
    VALUE, //double value
    KEYWORD,
    STRING,
    ID
};

struct Token
{
    TokenType type;
    std::string content;
    Token()
    {

    }
    Token(TokenType t,const std::string& val)
    {
        type = t;
        content = val;
    }
};
#endif