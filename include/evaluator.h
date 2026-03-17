#pragma once

#include "ast.h"
#include "environment.h"
#include "value.h"

#include <memory>

class Evaluator
{
private:
    std::shared_ptr<Environment> env;
    bool isTruth(const Value &val); // for value is "true" or "false"

public:
    Evaluator();

    void evaluateProgram(const ast::Program &program);

    // core traversal functions
    void execute(const ast::Stmt *stmt);
    Value evaluate(const ast::Expr *expr);
};