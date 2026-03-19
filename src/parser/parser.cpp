#include "parser.h"
#include "token.h"
#include "ast.h"
#include <iostream>
#include <vector>

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

        if (match(TokenType::If))
            return parseIfStatement();

        if (match(TokenType::While))
            return parseWhileStatement();

        // If it's not a specific keyword, it might be an expression statement
        ast::ExprPtr expr = parseExpression();
        if (!isAtEnd() && !check(TokenType::EndProgram))
        {
            consume(TokenType::EndOfStatement, "Expected newline after expression.");
        }
        else
        {
            syntaxError(peek(), "FAAAAHHH!, Unknown statement. Did you mean 'bhadwa', 'bolna', 'agar', or 'jabtak'?");
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

    // --- IF STATEMENT LOGIC ---
    ast::StmtPtr Parser::parseIfHelper()
    {
        ast::ExprPtr condition = parseExpression();
        consume(TokenType::Then, "Expected 'tab' after condition");
        skipNewlines();

        std::vector<ast::StmtPtr> consequence;
        while (!isAtEnd() && !check(TokenType::ElseIf) && !check(TokenType::Else))
        {
            if (match(TokenType::EndOfStatement))
                continue;
            consequence.push_back(parseStatement());
        }

        std::vector<ast::StmtPtr> alternative;
        if (match(TokenType::ElseIf))
        {
            alternative.push_back(parseIfHelper()); // --> Recursively chains if-else.
        }

        else if (match(TokenType::Else))
        {
            skipNewlines();
            while (!isAtEnd() && !check(TokenType::EndIf))
            {
                if (match(TokenType::EndOfStatement))
                    continue;
                alternative.push_back(parseStatement());
            }
        }

        return std::make_unique<ast::IfStmt>(std::move(condition), std::move(consequence), std::move(alternative));
    }

    ast::StmtPtr Parser::parseIfStatement()
    {
        auto ifStmt = parseIfHelper();
        consume(TokenType::EndIf, "Abe Laude! Expected `khtm` to close the if statement.");
        return ifStmt;
    }

    // --- While Implementation Jabtak --> Tabtak  --> hogya
    ast::StmtPtr Parser::parseWhileStatement()
    {
        ast::ExprPtr condition = parseExpression();
        consume(TokenType::Do, "Abbey Laude! Expected `TabTak` after while condition.");

        skipNewlines();

        std::vector<ast::StmtPtr> body;
        while (!isAtEnd() && !check(TokenType::EndWhile))
        {
            if (match(TokenType::EndOfStatement))
                continue;
            body.push_back(parseStatement());
        }

        consume(TokenType::EndWhile, "Abbey Laude!, Expected `hogya` to end the while loop.");
        return std::make_unique<ast::WhileStmt>(std::move(condition), std::move(body));
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
    /**
     *
     * Expression parsing  (recursive-descent, lowest → highest precedence)
     * parseExpression  →  parseOr
     * parseOr          →  parseAnd  ( "||" parseAnd )*
     * parseAnd         →  parseEquality  ( "&&" parseEquality )*
     * parseEquality    →  parseComparison  ( ("==" | "!=") parseComparison )*
     * parseComparison  →  parseAddSub  ( ("<"|">"|"<="|">=") parseAddSub )*
     * parseAddSub      →  parseMulDiv  ( ("+"|"-") parseMulDiv )*
     * parseMulDiv      →  parseUnary   ( ("*"|"/"|"%") parseUnary )*
     * parseUnary       →  ("-"|"!") parseUnary  |  parsePrimary
     * parsePrimary     →  NUMBER | STRING | BOOL | IDENTIFIER | "(" expr ")"
     * ───────────────────────────────────────────────────────────────────────────
     */
    ast::ExprPtr Parser::parseExpression() { return parseOr(); }

    ast::ExprPtr Parser::parseOr()
    {
        ast::ExprPtr expr = parseAnd();
        while (match(TokenType::LogicalOr))
        {
            ast::InfixOp op = ast::InfixOp::Or;
            ast::ExprPtr right = parseAnd();
            expr = std::make_unique<ast::InfixExpr>(std::move(expr), op, std::move(right));
        }
        return expr;
    }

    ast::ExprPtr Parser::parseAnd()
    {
        ast::ExprPtr expr = parseEquality();
        while (match(TokenType::LogicalAnd))
        {
            ast::InfixOp op = ast::InfixOp::And;
            ast::ExprPtr right = parseEquality();
            expr = std::make_unique<ast::InfixExpr>(std::move(expr), op, std::move(right));
        }
        return expr;
    }

    ast::ExprPtr Parser::parseEquality()
    {
        ast::ExprPtr expr = parseComparison();
        while (match(TokenType::Equal) || match(TokenType::NotEqual))
        {
            ast::InfixOp op = previous().type == TokenType::Equal ? ast::InfixOp::Eq : ast::InfixOp::NotEq;
            ast::ExprPtr right = parseComparison();
            expr = std::make_unique<ast::InfixExpr>(std::move(expr), op, std::move(right));
        }
        return expr;
    }

    ast::ExprPtr Parser::parseComparison()
    {
        ast::ExprPtr expr = parseAddSub();
        while (match(TokenType::LessThan) || match(TokenType::LessThanEqual) ||
               match(TokenType::GreaterThan) || match(TokenType::GreaterThanEqual))
        {
            ast::InfixOp op;
            switch (previous().type)
            {
            case TokenType::LessThan:
                op = ast::InfixOp::Lt;
                break;
            case TokenType::LessThanEqual:
                op = ast::InfixOp::LtEq;
                break;
            case TokenType::GreaterThan:
                op = ast::InfixOp::Gt;
                break;
            case TokenType::GreaterThanEqual:
                op = ast::InfixOp::GtEq;
                break;
            default:
                op = ast::InfixOp::Lt;
                break; // unreachable
            }
            ast::ExprPtr right = parseAddSub();
            expr = std::make_unique<ast::InfixExpr>(std::move(expr), op, std::move(right));
        }
        return expr;
    }

    ast::ExprPtr Parser::parseAddSub()
    {
        ast::ExprPtr expr = parseMulDiv();
        while (match(TokenType::Plus) || match(TokenType::Minus))
        {
            ast::InfixOp op = previous().type == TokenType::Plus ? ast::InfixOp::Add : ast::InfixOp::Sub;
            ast::ExprPtr right = parseMulDiv();
            expr = std::make_unique<ast::InfixExpr>(std::move(expr), op, std::move(right));
        }
        return expr;
    }

    ast::ExprPtr Parser::parseMulDiv()
    {
        ast::ExprPtr expr = parseUnary();
        while (match(TokenType::Multiply) || match(TokenType::Divide) || match(TokenType::Modulo))
        {
            ast::InfixOp op;
            if (previous().type == TokenType::Multiply)
                op = ast::InfixOp::Mul;
            else if (previous().type == TokenType::Divide)
                op = ast::InfixOp::Div;
            else
                op = ast::InfixOp::Mod;

            ast::ExprPtr right = parseUnary();
            expr = std::make_unique<ast::InfixExpr>(std::move(expr), op, std::move(right));
        }
        return expr;
    }

    ast::ExprPtr Parser::parseUnary()
    {
        if (match(TokenType::Not) || match(TokenType::Minus))
        {
            ast::PrefixOp op = previous().type == TokenType::Not ? ast::PrefixOp::Not : ast::PrefixOp::Negate;
            ast::ExprPtr right = parseUnary();
            return std::make_unique<ast::PrefixExpr>(op, std::move(right));
        }
        return parsePrimary();
    }

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