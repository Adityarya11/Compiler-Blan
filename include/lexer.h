#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "token.h"

class Lexer
{
    std::string source;
    std::vector<Token> tokens;

    int start = 0;
    int current = 0;
    int line = 1;
    int column = 1;

    std::unordered_map<std::string, TokenType> keywords;

    // helper functions
    bool isAtEnd() const;

    // Token Generation
    void scanToken();
    char advance();
    char peek() const;
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
    void addToken(TokenType type);
    void addToken(TokenType type, std::string text);

    void identifier();
    void number();

public:
    Lexer(std::string source);
    std::vector<Token> scanTokens();
};