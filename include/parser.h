#pragma once
#include <vector>
#include <stdexcept>
#include "token.h"
#include "ast.h"

namespace parser
{
    using Tokens = std::vector<Token>;

    class Parser
    {
    public:
        explicit Parser(Tokens tokens);
        ast::Program parse();

    private:
        Tokens tokens_;
        size_t current_;

        // helpers
        const Token &peek() const;
        const Token &previous() const;
        bool isAtEnd() const;
        const Token &advance();
        bool check(TokenType type) const;
        bool match(TokenType type);
        const Token &consume(TokenType type, const std::string &msg);

        // ── error helpers ────────────────────────────────────────
        // Syntax error  → "FAAAAHHH"
        [[noreturn]] void syntaxError(const Token &tok, const std::string &detail);
        // General error → "CHUDDI"  (runtime/other — reserved for later)
        [[noreturn]] void generalError(const Token &tok, const std::string &detail);

        // ── statement parsers ────────────────────────────────────
        ast::StmtPtr parseStatement();
        ast::StmtPtr parseVarDeclStatement();
        ast::StmtPtr parsePrintStatement();

        ast::StmtPtr parseIfStatement();
        ast::StmtPtr parseIfHelper(); // Secret sauce for else-if chains
        ast::StmtPtr parseWhileStatement();

        // ── expression parsers ───────────────────────────────────
        // Each level handles a different precedence layer
        ast::ExprPtr parseExpression();
        ast::ExprPtr parseOr();
        ast::ExprPtr parseAnd();
        ast::ExprPtr parseEquality();
        ast::ExprPtr parseComparison();
        ast::ExprPtr parseAddSub();
        ast::ExprPtr parseMulDiv();
        ast::ExprPtr parseUnary();
        ast::ExprPtr parsePrimary();

        // ── utility ──────────────────────────────────────────────
        void skipNewlines();
    };

} // namespace parser
