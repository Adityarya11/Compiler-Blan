#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "value.h"

class Environment
{
private:
    std::unordered_map<std::string, Value> vars; // this is the mimic of the zenLang map logic for the ast.
    std::shared_ptr<Environment> parent;         // this is useful for the functions

public:
    Environment(std::shared_ptr<Environment> parent = nullptr) : parent(parent) {}

    // for assigning bhadwa x matlb 2
    void define(const std::string &name, const Value &val)
    {
        vars[name] = val;
    }

    // read the variabls //-> bolna x
    Value get(const std::string &name)
    {
        if (vars.find(name) != vars.end())
        {
            return vars[name];
        }

        if (parent != nullptr)
            return parent->get(name);

        std::cerr << "\nCHUDDI! Undefined variable " << name << "laude value set kar warna hta"; // not defined variable
        std::exit(1);
    }
};