# About the Project

This project is a custom language interpreter built entirely around Hindi slang and swear words. The core motivation was to take a standard academic curriculum the Compiler Design (CS312) from my university—and apply it to an unconventional and humorous concept.

The architecture follows the classical first phase of compiler design:

1. **Lexical Analysis (Lexer)**
2. **Syntax Analysis (Parser)**
3. **Semantic Analysis (Evaluator)**

In a traditional scenario, a full production interpreter like CPython requires additional phases such as Bytecode conversion and Virtual Machine execution. However, this project is scoped to the semantic analyzer and evaluator phase. It successfully parses and evaluates the language's abstract syntax tree in memory.

Beyond the academic exercise, this implementation represents a playful take on programming itself—as if C++ simply got fed up with strict, polite rules and decided to embrace a more chaotic, expressive syntax. Further development would require deeper explorations into low-level compiler concepts, but as a personal challenge and an exercise in compiler theory, the current state fulfills its original vision perfectly.
