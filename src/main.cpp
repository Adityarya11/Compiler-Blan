#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "ast.h"
#include "evaluator.h"

void run(const std::string &sourceCode, bool isDebug)
{
    // ── Lexer
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.scanTokens();

    if (isDebug)
    {
        for (const Token &token : tokens)
        {
            if (token.type == TokenType::EndOfStatement)
            {
                std::cout << "[Line " << token.line << "] EndOfStatement '\\n'\n";
                continue;
            }
            std::cout << "[Line " << token.line << ", Col " << token.column << "] "
                      << tokenTypeToString(token.type)
                      << " : '" << token.lexeme << "'\n";
        }
    }

    // ── Parser
    parser::Parser p(tokens);
    ast::Program program = p.parse();

    if (isDebug)
    {
        std::cout << program.toString();
    }

    // -- Evaluator
    Evaluator evaluator;
    evaluator.evaluateProgram(program);
}

// execute .bl / .blan
void runFile(const std::string &path, bool isDebug)
{
    bool validExt = (path.length() >= 3 && path.substr(path.length() - 3) == ".bl") ||
                    (path.length() >= 5 && path.substr(path.length() - 5) == ".blan");
    if (!validExt)
    {
        std::cerr << "wrong extension\n";
        return;
    }

    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "cant open file: " << path << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();
    run(sourceCode, isDebug);
}

void runPrompt(bool isDebug)
{
    std::string line;
    while (true)
    {
        std::cout << "bl> ";
        if (!std::getline(std::cin, line))
            break; // Handle EOF (Ctrl+D / Ctrl+Z)
        if (line == "exit")
            break;

        run(line, isDebug);
    }
}

int main(int argc, char *argv[])
{
    bool isDebug = false;
    std::string filePath = "";

    // Parse arguments
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--debug")
        {
            isDebug = true;
        }
        else if (filePath.empty())
        {
            filePath = arg;
        }
        else
        {
            std::cerr << "Usage: badlang [script] [--debug]\n";
            return 64;
        }
    }

    if (!filePath.empty())
    {
        runFile(filePath, isDebug);
    }
    else
    {
        runPrompt(isDebug);
    }

    return 0;
}