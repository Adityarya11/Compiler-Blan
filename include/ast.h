#pragma once
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <iostream>
#include <sstream>
#include "token.h"

namespace ast
{
    struct Stmt;
    struct Expr;

    using StmtPtr = std::unique_ptr<Stmt>;
    using ExprPtr = std::unique_ptr<Expr>;

    // Identifier
    struct Identifier
    {
        std::string name;
        Identifier() = default;
        explicit Identifier(std::string s) : name(std::move(s)) {}
    };

    // Literal - Name, Number, bool
    using LiteralValue = std::variant<double, bool, std::string>;
    struct Literal
    {
        LiteralValue value;
        Literal() = default;
        explicit Literal(LiteralValue v) : value(std::move(v)) {}
    };

    // Operator kinds
    enum class PrefixOp
    {
        Negate, // -
        Not,    // !
    };
    enum class InfixOp
    {
        Add,
        Sub,
        Mul,
        Div,
        Mod,
        Eq,
        NotEq,
        Lt,
        LtEq,
        Gt,
        GtEq,
        And,
        Or
    };

    // --- HELPER STRINGS FOR DEBUGGING ---
    inline std::string prefixOpStr(PrefixOp op)
    {
        if (op == PrefixOp::Negate)
            return "-";
        if (op == PrefixOp::Not)
            return "!";
        return "";
    }

    inline std::string infixOpStr(InfixOp op)
    {
        switch (op)
        {
        case InfixOp::Add:
            return "+";
        case InfixOp::Sub:
            return "-";
        case InfixOp::Mul:
            return "*";
        case InfixOp::Div:
            return "/";
        case InfixOp::Mod:
            return "%";
        case InfixOp::Eq:
            return "==";
        case InfixOp::NotEq:
            return "!=";
        case InfixOp::Lt:
            return "<";
        case InfixOp::LtEq:
            return "<=";
        case InfixOp::Gt:
            return ">";
        case InfixOp::GtEq:
            return ">=";
        case InfixOp::And:
            return "&&";
        case InfixOp::Or:
            return "||";
        default:
            return "";
        }
    }

    // Base expression
    /**
     * below struct is the base of the other expr as sometime we need our expression to be inside other
     * in case of the conditionals and loops. so the issue is solved using the polymorphism of the C++
     * the **VIRTUAL** enables runtime polymorphism.
     * "The function that runs depends on the actual object type, not the pointer type."
     */

    struct Expr
    {
        virtual ~Expr() = default;
        virtual std::string toString() const = 0;
    };

    // Expression variants
    struct IdentifierExpr : Expr
    {
        Identifier id;
        explicit IdentifierExpr(Identifier i) : id(std::move(i)) {}
        std::string toString() const override { return id.name; }
    };

    struct LiteralExpr : Expr
    {
        Literal lit;
        explicit LiteralExpr(Literal l) : lit(std::move(l)) {}
        std::string toString() const override
        {
            if (std::holds_alternative<double>(lit.value))
                return std::to_string(std::get<double>(lit.value));
            if (std::holds_alternative<bool>(lit.value))
                return std::get<bool>(lit.value) ? "true" : "false";
            return "\"" + std::get<std::string>(lit.value) + "\"";
        }
    };

    // -expr  /  !expr
    struct PrefixExpr : Expr
    {
        PrefixOp op;
        ExprPtr right;
        PrefixExpr(PrefixOp op, ExprPtr right) : op(op), right(std::move(right)) {}
        std::string toString() const override { return "(" + prefixOpStr(op) + right->toString() + ")"; }
    };

    // left op right
    struct InfixExpr : Expr
    {
        ExprPtr left;
        InfixOp op;
        ExprPtr right;
        InfixExpr(ExprPtr left, InfixOp op, ExprPtr right) : left(std::move(left)), op(op), right(std::move(right)) {}
        std::string toString() const override { return "(" + left->toString() + " " + infixOpStr(op) + " " + right->toString() + ")"; }
    };

    // input keyword
    struct InputExpr : Expr
    {
        std::string toString() const override { return "<input>"; }
    };

    // Base stmt
    struct Stmt
    {
        virtual ~Stmt() = default;
        virtual std::string toString(unsigned indent = 0) const = 0;
    };

    inline std::string indentStr(unsigned n) { return std::string(n * 2, ' '); }

    // Statement variants
    struct ProgramStartStmt : Stmt
    {
        std::string toString(unsigned indent = 0) const override { return indentStr(indent) + "<ProgramStart>"; }
    };

    struct ProgramEndStmt : Stmt
    {
        std::string toString(unsigned indent = 0) const override { return indentStr(indent) + "<ProgramEnd>"; }
    };

    struct VarDeclStmt : Stmt
    {
        Identifier name;
        ExprPtr initializer;
        VarDeclStmt(Identifier name, ExprPtr initializer) : name(std::move(name)), initializer(std::move(initializer)) {}
        std::string toString(unsigned indent = 0) const override
        {
            return indentStr(indent) + "VarDecl(" + name.name + " = " + (initializer ? initializer->toString() : "<null>") + ")";
        }
    };

    // bolna <expr>
    struct PrintStmt : Stmt
    {
        ExprPtr expr;
        explicit PrintStmt(ExprPtr e) : expr(std::move(e)) {}
        std::string toString(unsigned indent = 0) const override
        {
            return indentStr(indent) + "Print(" + (expr ? expr->toString() : "<null>") + ")";
        }
    };

    struct ExprStmt : Stmt
    {
        ExprPtr expr;
        explicit ExprStmt(ExprPtr e) : expr(std::move(e)) {}
        std::string toString(unsigned indent = 0) const override { return indentStr(indent) + "ExprStmt(" + (expr ? expr->toString() : "<null>") + ")"; }
    };

    // --- ROOT NODE ---
    struct Program
    {
        std::vector<StmtPtr> statements;
        std::string toString() const
        {
            std::ostringstream os;
            for (auto const &s : statements)
                os << s->toString() << "\n";
            return os.str();
        }
    };
} // namespace ast