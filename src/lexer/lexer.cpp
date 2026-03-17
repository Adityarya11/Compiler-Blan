#include "lexer.h"
#include <cctype>

Lexer::Lexer(std::string source) : source(source)
{
    keywords = {
        {"bhadwa", TokenType::VarDecl},
        {"matlb", TokenType::Assign},
        {"bolna", TokenType::Print}};
}

// if EOF reached
bool Lexer::isAtEnd() const
{
    return current >= static_cast<int>(source.length());
}

// Returns the current character and advances both cursors.
char Lexer::advance()
{
    char c = source[current];
    current++;
    column++;
    return c;
}

char Lexer::peek() const
{
    if (isAtEnd())
        return '\0';
    return source[current];
}

bool Lexer::isAlpha(char c) const
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Lexer::isDigit(char c) const
{
    return c >= '0' && c <= '9';
}

bool Lexer::isAlphaNumeric(char c) const
{
    return isAlpha(c) || isDigit(c);
}

void Lexer::addToken(TokenType type)
{
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, std::move(text), line, column);
}

void Lexer::addToken(TokenType type, std::string text)
{
    tokens.emplace_back(type, std::move(text), line, column);
}

void Lexer::number()
{
    while (isDigit(peek()))
        advance();
    addToken(TokenType::Number, source.substr(start, current - start));
}

void Lexer::identifier()
{
    // Consume the rest of the word (first char already consumed by scanToken).
    while (isAlphaNumeric(peek()))
        advance();
    std::string text = source.substr(start, current - start);

    // multi-keyword check

    if (text == "Haan")
    {
        const std::string expected = " Meri Jaan";
        if (current + static_cast<int>(expected.size()) <= static_cast<int>(source.size()) &&
            source.substr(current, expected.size()) == expected)
        {
            current += static_cast<int>(expected.size());
            column += static_cast<int>(expected.size());
            addToken(TokenType::StartProgram, "Haan Meri Jaan");
            return;
        }
    }

    // Multi-word keyword: "Bhag Bsdk"
    else if (text == "Bhag")
    {
        const std::string expected = " Bsdk";
        if (current + static_cast<int>(expected.size()) <= static_cast<int>(source.size()) &&
            source.substr(current, expected.size()) == expected)
        {
            current += static_cast<int>(expected.size());
            column += static_cast<int>(expected.size());
            addToken(TokenType::EndProgram, "Bhag Bsdk");
            return;
        }
    }

    // Look up keyword map; fall back to plain Identifier.
    auto it = keywords.find(text);
    if (it != keywords.end())
        addToken(it->second, text);
    else
        addToken(TokenType::Identifier, text);
}

void Lexer::scanToken()
{
    char c = advance();
    switch (c)
    {
    // Whitespace — skip
    case ' ':
    case '\r':
    case '\t':
        break;

    // Newline — acts as statement terminator
    case '\n':
        tokens.emplace_back(TokenType::EndOfStatement, "\\n", line, column);
        line++;
        column = 1;
        break;

    // Single-character operators
    case '+':
        addToken(TokenType::Plus);
        break;
    case '-':
        addToken(TokenType::Minus);
        break;
    case '*':
        addToken(TokenType::Multiply);
        break;
    case '/':
        if (peek() == '/')
        {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd())
                advance();
        }
        else
        {
            addToken(TokenType::Divide);
        }
        break;
    case '%':
        addToken(TokenType::Modulo);
        break;
    case '(':
        addToken(TokenType::LeftParen);
        break;
    case ')':
        addToken(TokenType::RightParen);
        break;
    case '=':
        addToken(TokenType::Equal);
        break;

    // Two-character operators
    case '<':
        if (peek() == '=')
        {
            advance();
            addToken(TokenType::LessThanEqual);
        }
        else
            addToken(TokenType::LessThan);
        break;
    case '>':
        if (peek() == '=')
        {
            advance();
            addToken(TokenType::GreaterThanEqual);
        }
        else
            addToken(TokenType::GreaterThan);
        break;
    case '!':
        if (peek() == '=')
        {
            advance();
            addToken(TokenType::NotEqual);
        }
        else
            addToken(TokenType::Not);
        break;

    // String literals
    case '"':
        while (peek() != '"' && !isAtEnd())
        {
            if (peek() == '\n')
            {
                line++;
                column = 1;
            }
            advance();
        }
        if (!isAtEnd())
            advance(); // consume closing "
        addToken(TokenType::StringLiteral, source.substr(start + 1, current - start - 2));
        break;

    default:
        if (isDigit(c))
            number();
        else if (isAlpha(c))
            identifier();
        else
            addToken(TokenType::Illegal);
        break;
    }
}

std::vector<Token> Lexer::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::Eof, "", line, column);
    return tokens;
}
