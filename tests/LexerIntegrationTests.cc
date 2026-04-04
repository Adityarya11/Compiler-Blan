#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "lexer.h"
#include "token.h"

class LexerIntegrationTest : public ::testing::Test
{
protected:
    std::vector<Token> scan(const std::string &source)
    {
        Lexer lexer(source);
        return lexer.scanTokens();
    }

    void expectToken(const Token &token, TokenType type, const std::string &lexeme)
    {
        EXPECT_EQ(token.type, type);
        EXPECT_EQ(token.lexeme, lexeme);
    }
};

//-----
TEST_F(LexerIntegrationTest, TokenizesVarDeclAssignmentAndNumber)
{
    const auto tokens = scan("bhadwa age matlb 21");

    ASSERT_EQ(tokens.size(), 5);
    expectToken(tokens[0], TokenType::VarDecl, "bhadwa");
    expectToken(tokens[1], TokenType::Identifier, "age");
    expectToken(tokens[2], TokenType::Assign, "matlb");
    expectToken(tokens[3], TokenType::Number, "21");
    expectToken(tokens[4], TokenType::Eof, "");
}

//-----
TEST_F(LexerIntegrationTest, TokenizesProgramBoundaryKeywords)
{
    const auto tokens = scan("Haan Meri Jaan\nBhag Bsdk");

    ASSERT_EQ(tokens.size(), 4);
    expectToken(tokens[0], TokenType::StartProgram, "Haan Meri Jaan");
    expectToken(tokens[1], TokenType::EndOfStatement, "\\n");
    expectToken(tokens[2], TokenType::EndProgram, "Bhag Bsdk");
    expectToken(tokens[3], TokenType::Eof, "");
}

//-----
TEST_F(LexerIntegrationTest, IgnoresLineCommentAndKeepsFollowingTokens)
{
    const auto tokens = scan("// comment\n+");

    ASSERT_EQ(tokens.size(), 3);
    expectToken(tokens[0], TokenType::EndOfStatement, "\\n");
    expectToken(tokens[1], TokenType::Plus, "+");
    expectToken(tokens[2], TokenType::Eof, "");
}

//-----
TEST_F(LexerIntegrationTest, TokenizesStringLiteralWithoutQuotes)
{
    const auto tokens = scan("bolna \"hello compiler\"");

    ASSERT_EQ(tokens.size(), 3);
    expectToken(tokens[0], TokenType::Print, "bolna");
    expectToken(tokens[1], TokenType::StringLiteral, "hello compiler");
    expectToken(tokens[2], TokenType::Eof, "");
}

//-----
TEST_F(LexerIntegrationTest, ProducesIllegalForSingleAmpersand)
{
    const auto tokens = scan("&");

    ASSERT_EQ(tokens.size(), 2);
    expectToken(tokens[0], TokenType::Illegal, "&");
    expectToken(tokens[1], TokenType::Eof, "");
}