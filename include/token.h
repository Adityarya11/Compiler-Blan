#pragma once
#include <string>
#include <iostream>

enum class TokenType
{
    // Special tokens
    Illegal,
    Eof,
    EndOfStatement,

    // Identifiers and literals
    Identifier,
    Number,
    StringLiteral,
    BooleanLiteral,

    // Operators
    Assign,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,

    // Relational Operators
    GreaterThan,
    GreaterThanEqual,
    LessThan,
    LessThanEqual,
    Equal,
    NotEqual,

    // Logical Operators
    Not,
    LogicalAnd,
    LogicalOr,

    // Keywords
    StartProgram,
    EndProgram,
    VarDecl,   // bhadwa — variable declaration
    If,
    Then,
    ElseIf,
    Else,
    EndIf,
    While,
    Do,
    EndWhile,
    Print,
    Input,

    // Punctuation
    LeftParen,
    RightParen,
};

std::string tokenTypeToString(TokenType type);

struct Token
{
    TokenType type;
    std::string lexeme;

    int line;
    int column;

    Token(TokenType type, std::string lexeme, int line = 1, int column = 1)
        : type(type), lexeme(std::move(lexeme)), line(line), column(column) {}
};