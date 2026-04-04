#include <gtest/gtest.h>
#include <string>
#include <utility>
#include <vector>

#include "evaluator.h"
#include "lexer.h"
#include "parser.h"

class InterpreterBooleanIntegrationTest : public ::testing::Test
{
protected:
    std::string runProgram(const std::string &source)
    {
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.scanTokens();

        parser::Parser parser(std::move(tokens));
        ast::Program program = parser.parse();

        Evaluator evaluator;
        testing::internal::CaptureStdout();
        evaluator.evaluateProgram(program);
        return testing::internal::GetCapturedStdout();
    }
};

//-----
TEST_F(InterpreterBooleanIntegrationTest, RunsIfBlockForSach)
{
    const std::string output = runProgram(
        "Haan Meri Jaan\n"
        "bhadwa x matlb sach\n"
        "agar x tab\n"
        "    bolna \"bool works\"\n"
        "khtm\n"
        "Bhag Bsdk\n");

    EXPECT_EQ(output, "bool works\n");
}

//-----
TEST_F(InterpreterBooleanIntegrationTest, EvaluatesLogicalNot)
{
    const std::string output = runProgram(
        "Haan Meri Jaan\n"
        "bolna !sach\n"
        "bolna !jhooth\n"
        "Bhag Bsdk\n");

    EXPECT_EQ(output, "jhooth\nsach\n");
}

//-----
TEST_F(InterpreterBooleanIntegrationTest, EvaluatesLogicalAndOr)
{
    const std::string output = runProgram(
        "Haan Meri Jaan\n"
        "bolna sach && jhooth\n"
        "bolna jhooth || sach\n"
        "Bhag Bsdk\n");

    EXPECT_EQ(output, "jhooth\nsach\n");
}

//-----
TEST_F(InterpreterBooleanIntegrationTest, EvaluatesBooleanEquality)
{
    const std::string output = runProgram(
        "Haan Meri Jaan\n"
        "bolna sach == sach\n"
        "bolna sach != jhooth\n"
        "Bhag Bsdk\n");

    EXPECT_EQ(output, "sach\nsach\n");
}

//-----
TEST_F(InterpreterBooleanIntegrationTest, MapsSachJhoothToOneZeroInExpressions)
{
    const std::string output = runProgram(
        "Haan Meri Jaan\n"
        "bhadwa x matlb sach\n"
        "bhadwa y matlb jhooth\n"
        "agar x == 1 tab\n"
        "    bolna \"ok\"\n"
        "khtm\n"
        "bolna y == 0\n"
        "bolna x + 2\n"
        "Bhag Bsdk\n");

    EXPECT_EQ(output, "ok\nsach\n3\n");
}
