#pragma once
#include <variant>
#include <optional>
#include <iostream>
#include <string>

using ValueData = std::variant<std::monostate, bool, double, std::string>;

struct Value
{
    ValueData v;

    // Constructors for easy value creation;
    Value() : v(std::monostate{}) {};
    Value(bool b) : v(b) {};
    Value(double d) : v(d) {};
    Value(std::string s) : v(std::move(s)) {};

    // helper to print to the console

    std::string toString() const
    {
        if (std::holds_alternative<double>(v))
        {
            std::string str = std::to_string(std::get<double>(v));
            str.erase(str.find_last_not_of('0') + 1, std::string::npos); // remove the trailing 0s for the double

            if (str.back() == '.')
                str.pop_back();
            return str;
        }
        if (std::holds_alternative<bool>(v))
        {
            return std::get<bool>(v) ? "sach" : "jhooth"; // for bool types sach for true.
        }
        if (std::holds_alternative<std::string>(v))
        {
            return std::get<std::string>(v);
        }
        return "null";
    }
};