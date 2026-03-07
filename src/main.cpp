#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "ast.h"

void run(const std::string &sourceCode)
{
    // ── Lexer
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.scanTokens();

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

    // ── Parser
    parser::Parser p(tokens);
    ast::Program program = p.parse();
    std::cout << program.toString();
}

// execute .bl / .blan
void runFile(const std::string &path)
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
    run(sourceCode);
}

void runPrompt()
{
    std::string line;
    while (true)
    {
        std::cout << "bl> ";
        if (!std::getline(std::cin, line))
            break; // Handle EOF (Ctrl+D / Ctrl+Z)
        if (line == "exit")
            break;

        run(line);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 2)
        return 64;
    else if (argc == 2)
        runFile(argv[1]);
    else
        runPrompt();

    return 0;
}