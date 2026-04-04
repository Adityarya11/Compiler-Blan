#include "evaluator.h"
#include <iostream>

Evaluator::Evaluator()
{
    env = std::make_shared<Environment>();
}

void Evaluator::evaluateProgram(const ast::Program &program)
{
    for (const auto &stmt : program.statements)
    {
        execute(stmt.get());
    }
}

bool Evaluator::isTruth(const Value &val)
{
    if (std::holds_alternative<bool>(val.v))
        return std::get<bool>(val.v);

    // if number != 0, --> "sach"
    if (std::holds_alternative<double>(val.v))
        return std::get<double>(val.v) != 0;

    // if string !empty --> "sach"
    if (std::holds_alternative<std::string>(val.v))
        return !std::get<std::string>(val.v).empty();

    return false; // null --> "jhooth"
}

bool Evaluator::isNumericLike(const Value &val)
{
    return std::holds_alternative<double>(val.v) || std::holds_alternative<bool>(val.v);
}

double Evaluator::asNumber(const Value &val)
{
    if (std::holds_alternative<double>(val.v))
        return std::get<double>(val.v);
    if (std::holds_alternative<bool>(val.v))
        return std::get<bool>(val.v) ? 1.0 : 0.0;

    std::cerr << "\nCHUDDI! Expected numeric value (number/sach/jhooth).\n";
    std::exit(1);
}

// -- Stmt Execution
void Evaluator::execute(const ast::Stmt *stmt)
{
    // ignoring start and end of the stmt at runtime

    if (dynamic_cast<const ast::ProgramStartStmt *>(stmt) || dynamic_cast<const ast::ProgramEndStmt *>(stmt))
        return;

    // handle assign stmt
    if (auto varDecl = dynamic_cast<const ast::VarDeclStmt *>(stmt))
    {
        Value val = evaluate(varDecl->initializer.get());
        env->define(varDecl->name.name, val);
        return;
    }

    // handle print stmt
    if (auto printstmt = dynamic_cast<const ast::PrintStmt *>(stmt))
    {
        Value val = evaluate(printstmt->expr.get());
        std::cout << val.toString() << "\n";
        return;
    }

    // handle normal stmts
    if (auto exprStmt = dynamic_cast<const ast::ExprStmt *>(stmt))
    {
        evaluate(exprStmt->expr.get());
        return;
    }

    // Handle if-else Statement.
    if (auto ifStmt = dynamic_cast<const ast::IfStmt *>(stmt))
    {
        Value conditionalVal = evaluate(ifStmt->condition.get());
        if (isTruth(conditionalVal))
        {
            for (const auto &s : ifStmt->consequences)
            {
                execute(s.get());
            }
        }
        else
        {
            for (const auto &s : ifStmt->alternative)
            {
                execute(s.get());
            }
        }
        return;
    }

    // Handle While Loop
    if (auto whileStmt = dynamic_cast<const ast::WhileStmt *>(stmt))
    {
        int iterCount = 0;
        const int Max_ITER_COUNT = 100000;

        while (isTruth(evaluate(whileStmt->condition.get())))
        {
            for (const auto &s : whileStmt->body)
            {
                execute(s.get());
            }
            iterCount++;

            if (iterCount > Max_ITER_COUNT)
            {
                std::cerr << "CHHUDDI!, Maximum loops exceeded, Infinite loop hai re laure.";
                std::exit(1);
            }
        }

        return;
    }

    std::cerr << "\nCHUDDI! Unknwn statement encountered during execution.\n";
    std::exit(1);
}

// -- Expression Evaluation
Value Evaluator::evaluate(const ast::Expr *expr)
{
    // 1. Handles Literals
    if (auto literals = dynamic_cast<const ast::LiteralExpr *>(expr))
    {
        if (std::holds_alternative<double>(literals->lit.value))
        {
            return Value(std::get<double>(literals->lit.value));
        }
        if (std::holds_alternative<bool>(literals->lit.value))
        {
            return Value(std::get<bool>(literals->lit.value));
        }
        if (std::holds_alternative<std::string>(literals->lit.value))
        {
            return Value(std::get<std::string>(literals->lit.value));
        }
    }

    // 2. Handles Identifier
    if (auto identifier = dynamic_cast<const ast::IdentifierExpr *>(expr))
    {
        return env->get(identifier->id.name);
    }

    // 3. Handle prefix Expression
    if (auto prefix = dynamic_cast<const ast::PrefixExpr *>(expr))
    {
        Value right = evaluate(prefix->right.get());

        if (prefix->op == ast::PrefixOp::Negate)
        {
            if (!isNumericLike(right))
            {
                std::cerr << "\nCHUDDI! You can only negate numbers!\n";
                std::exit(1);
            }
            return Value(-asNumber(right));
        }

        if (prefix->op == ast::PrefixOp::Not)
        {
            if (!std::holds_alternative<bool>(right.v))
            {
                std::cerr << "\nCHUDDI! You can only use '!' on booleans!\n";
                std::exit(1);
            }
            return Value(!std::get<bool>(right.v));
        }
    }

    // 4. Handles infix (all binary things)
    if (auto infix = dynamic_cast<const ast::InfixExpr *>(expr))
    {
        Value left = evaluate(infix->left.get());

        // Short circuit implementation (AND / OR)
        if (infix->op == ast::InfixOp::And)
        {
            if (!isTruth(left))
                return Value(false);
        }

        if (infix->op == ast::InfixOp::Or)
        {
            if (isTruth(left))
                return Value(true);
        }

        Value right = evaluate(infix->right.get());

        // check for both numbers
        bool bothNumbers = isNumericLike(left) && isNumericLike(right);

        switch (infix->op)
        {

        // Logical
        case ast::InfixOp::Or:
        case ast::InfixOp::And:
            // At this point, right will decide the final answer to the shortcircuiting logic.
            return Value(isTruth(right));

        // airthmetic
        case ast::InfixOp::Add:
            if (bothNumbers)
                return Value(asNumber(left) + asNumber(right));

            // both string: concatanate
            if (std::holds_alternative<std::string>(left.v) && std::holds_alternative<std::string>(right.v))
            {
                return Value(std::get<std::string>(left.v) + std::get<std::string>(right.v));
            }
            std::cerr << "\nCHUDDI!, type mismatch in addition.\n";
            std::exit(1);

        case ast::InfixOp::Sub:
            if (!bothNumbers)
            {
                std::cerr << "\nCHUDDI!, Subtration number ka karte hai LALE";
                std::exit(1);
            }
            return Value(asNumber(left) - asNumber(right));

        case ast::InfixOp::Mul:
            if (!bothNumbers)
            {
                std::cerr << "\nCHUDDI!, Multiplication number ka karte hai LALE";
                std::exit(1);
            }
            return Value(asNumber(left) * asNumber(right));

        case ast::InfixOp::Div:
            if (!bothNumbers)
            {
                std::cerr << "\nCHUDDI!, Division numbers ka karte hai LALE";
                std::exit(1);
            }

            if (asNumber(right) == 0)
            {
                std::cerr << "\nCHUDDI!, Division by zero, Lund hai kya BSDK";
                std::exit(1);
            }

            return Value(asNumber(left) / asNumber(right));

        case ast::InfixOp::Mod:
            if (!bothNumbers)
            {
                std::cerr << "\nCHUDDI!, Modulo number ka karte hai LALE";
                std::exit(1);
            }
            if (asNumber(right) == 0)
            {
                std::cerr << "\nCHUDDI!, Modulo by zero, Lund hai kya BSDK";
                std::exit(1);
            }
            return Value((double)((int)asNumber(left) % (int)asNumber(right)));

        // Comparators
        case ast::InfixOp::Eq:
            if (bothNumbers)
                return Value(asNumber(left) == asNumber(right));
            return Value(left.v == right.v);
        case ast::InfixOp::NotEq:
            if (bothNumbers)
                return Value(asNumber(left) != asNumber(right));
            return Value(left.v != right.v);

        case ast::InfixOp::Gt:
            if (!bothNumbers)
            {
                std::cerr << "\nCHUDDI!, Comparison numbers ka karte hai LALE";
                std::exit(1);
            }
            return Value(asNumber(left) > asNumber(right));
        case ast::InfixOp::GtEq:
            if (!bothNumbers)
            {
                std::cerr << "\nCHUDDI!, Comparison numbers ka karte hai LALE";
                std::exit(1);
            }
            return Value(asNumber(left) >= asNumber(right));
        case ast::InfixOp::Lt:
            if (!bothNumbers)
            {
                std::cerr << "\nCHUDDI!, Comparison numbers ka karte hai LALE";
                std::exit(1);
            }
            return Value(asNumber(left) < asNumber(right));
        case ast::InfixOp::LtEq:
            if (!bothNumbers)
            {
                std::cerr << "\nCHUDDI!, Comparison numbers ka karte hai LALE";
                std::exit(1);
            }
            return Value(asNumber(left) <= asNumber(right));

        default:
            std::cerr << "\nCHUDDI! Unknown operator!\n";
            std::exit(1);
        }
    }

    std::cerr << "\nCHUDDI!, We havent implemented evaluating this expression yet.\n";
    std::exit(1);
}