#include "parser.h"
#include "token.h"
#include "ast.h"
#include <iostream>

namespace parser
{

    Parser::Parser(Tokens tokens) : tokens_(std::move(tokens)), current_(0) {}

    ast::Program Parser::parse()
    {
        ast::Program program;

        skipNewlines();

        // for beginning of the code.
        consume(TokenType::StartProgram, "Expected 'Haan Meri Jaan' at the top of the program.");
        program.statements.push_back(std::make_unique<ast::ProgramStartStmt>());

        skipNewlines();

        // for EOF
        while (!isAtEnd() && !check(TokenType::EndProgram))
        {
            skipNewlines();
            if (isAtEnd() || check(TokenType::EndProgram))
                break;
            program.statements.push_back(parseStatement());
        }

        // check for EOF
        consume(TokenType::EndProgram, "Expected 'Bhag Bsdk' to close the Program.");
        program.statements.push_back(std::make_unique<ast::ProgramEndStmt>());

        // anything after bhaag bsdk is not acceptable
        skipNewlines();
        if (!isAtEnd())
        {
            syntaxError(peek(), "Unexpected tokens after 'Bhag Bsdk'. Nothing should exist past the end of the program.");
        }

        return program;
    }

    ast::StmtPtr Parser::parseStatement()
    {
        if (match(TokenType::VarDecl))
            return parseVarDeclStatement();
        if (match(TokenType::Print))
            return parsePrintStatement();

        // If it's not a specific keyword, it might be an expression statement
        ast::ExprPtr expr = parseExpression();
        if (!isAtEnd() && !check(TokenType::EndProgram))
        {
            consume(TokenType::EndOfStatement, "Expected newline after expression.");
        }
        return std::make_unique<ast::ExprStmt>(std::move(expr));
    }

    ast::StmtPtr Parser::parseVarDeclStatement()
    {
        const Token &nameToken = consume(TokenType::Identifier, "Expected name after 'bhadwa'. Bhadwa refers to the variable initialisation.");
        consume(TokenType::Assign, "Expected 'matlb' after variable name " + nameToken.lexeme + ".");

        ast::ExprPtr init = parseExpression();

        if (!isAtEnd() && !check(TokenType::EndProgram))
            consume(TokenType::EndOfStatement, "Expected newline after variable declaration.");

        return std::make_unique<ast::VarDeclStmt>(ast::Identifier{nameToken.lexeme}, std::move(init));
    }

    // bolna
    ast::StmtPtr Parser::parsePrintStatement()
    {
        ast::ExprPtr val = parseExpression();

        if (!isAtEnd() && !check(TokenType::EndProgram))
            consume(TokenType::EndOfStatement, "Expected newline after 'bolna' statement.");

        return std::make_unique<ast::PrintStmt>(std::move(val));
    }

    // token stream ─── HELPER METHODS ───

    const Token &Parser::peek() const { return tokens_[current_]; }
    const Token &Parser::previous() const { return tokens_[current_ - 1]; }
    bool Parser::isAtEnd() const { return peek().type == TokenType::Eof; }
    bool Parser::check(TokenType type) const { return (!isAtEnd() && peek().type == type); }

    const Token &Parser::advance()
    {
        if (!isAtEnd())
            current_++;
        return previous();
    }

    const Token &Parser::consume(TokenType type, const std::string &msg)
    {
        if (check(type))
            return advance();
        syntaxError(peek(), msg);
    }

    void Parser::skipNewlines()
    {
        while (check(TokenType::EndOfStatement))
            advance();
    }

    bool Parser::match(TokenType type)
    {
        if (check(type))
        {
            advance();
            return true;
        }
        return false;
    }

    // Error Reporters

    void Parser::syntaxError(const Token &tok, const std::string &detail)
    {
        std::cerr << "\nBehenChod! [Line " << tok.line << ", Col " << tok.column
                  << "] Syntax Error near '" << tok.lexeme << "':\n"
                  << "  --> " << detail << "\n\n";
        std::exit(1);
    }

    void Parser::generalError(const Token &tok, const std::string &detail)
    {
        std::cerr << "\nCHUDDI! [Line " << tok.line << ", Col " << tok.column
                  << "] Error near '" << tok.lexeme << "':\n"
                  << "  --> " << detail << "\n\n";
        std::exit(1);
    }

    // ── expression parse stubs (precedence chain) ────────────────
    // Each level will be filled in as the language grows.
    // For now every level falls straight through to parsePrimary.
    ast::ExprPtr Parser::parseExpression() { return parsePrimary(); }
    ast::ExprPtr Parser::parseOr() { return parsePrimary(); }
    ast::ExprPtr Parser::parseAnd() { return parsePrimary(); }
    ast::ExprPtr Parser::parseEquality() { return parsePrimary(); }
    ast::ExprPtr Parser::parseComparison() { return parsePrimary(); }
    ast::ExprPtr Parser::parseAddSub() { return parsePrimary(); }
    ast::ExprPtr Parser::parseMulDiv() { return parsePrimary(); }
    ast::ExprPtr Parser::parseUnary() { return parsePrimary(); }

    ast::ExprPtr Parser::parsePrimary()
    {
        // Number literal
        if (match(TokenType::Number))
        {
            double val = std::stod(previous().lexeme);
            return std::make_unique<ast::LiteralExpr>(ast::Literal{val});
        }

        // String literal
        if (match(TokenType::StringLiteral))
            return std::make_unique<ast::LiteralExpr>(ast::Literal{previous().lexeme});

        // Boolean literals  (true / false keywords can be added later)
        if (match(TokenType::BooleanLiteral))
        {
            bool val = (previous().lexeme == "sach"); // language's true word
            return std::make_unique<ast::LiteralExpr>(ast::Literal{val});
        }

        // Identifier
        if (match(TokenType::Identifier))
            return std::make_unique<ast::IdentifierExpr>(ast::Identifier{previous().lexeme});

        // Grouped expression  ( expr )
        if (match(TokenType::LeftParen))
        {
            ast::ExprPtr inner = parseExpression();
            consume(TokenType::RightParen, "Expected ')' after expression.");
            return inner;
        }

        syntaxError(peek(), "Expected an expression (number, string, or identifier).");
    }

} // namespace parser