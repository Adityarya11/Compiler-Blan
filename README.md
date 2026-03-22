# Bad Language Compiler (Blan)

<!-- ## Overview -->

The Bad Language Compiler (Blan) is a specialized programming language compiler implemented in C++. Inspired by the [Zen Programming Language Compiler](https://github.com/zenlang-rs/compiler), this project translates code written in a culturally unique syntax—utilizing Hindi slang and profanities for keywords and functions—into an executable program.

## Language Specifications

### Program Structure

The language strictly enforces designated entry and exit points for scope management.

- **Entry Point:** `Haan Meri Jaan` marks the beginning of the program.
- **Exit Point:** `Bhag Bsdk` marks the safe termination of the program. Any tokens found after this point will result in a syntax error.

### Comments

Single-line comments are supported and function identically to C++ or JavaScript.

```bl
// This is a single-line comment and will be ignored by the parser.
```

### Data Types

The evaluator utilizes a `Value` class (implemented via `std::variant`) to support the following data types:

- **Numbers:** Implemented internally as double-precision floating-point numbers (`double`).
- **Strings:** Dynamically typed sequence of characters. String concatenation is supported.
- **Booleans:** Represents true (`sach`) or false (`jhooth`).
- **Null / Monostate:** Represents an empty or uninitialized state.

### Variable Declaration & Assignment

Variables are declared and assigned using the language's specific keyword set.

- `bhadwa` is used for variable declaration.
- `matlb` serves as the assignment operator (`=`).

### Built-in Functions

- `bolna`: Evaluates the subsequent expression and prints the result to the standard output console.

### Supported Operators

The language supports standard operations with enforced type checking during evaluation:

1. **Arithmetic:** `+`, `-`, `*`, `/`, `%`
   - The addition operator supports both numerical addition and string concatenation. Type mismatches (e.g., subtracting strings) will result in a semantic error.
2. **Logical:** `&&` (AND), `||` (OR), `!` (NOT)
   - Short-circuit evaluation is implemented to optimize performance by skipping unnecessary right-hand side evaluations.
3. **Comparison:** `<`, `<=`, `>`, `>=`, `==` (Equality), `!=` (Inequality)

## Build and Execution

The compiler utilizes **CMake** for its build ecosystem.

### Prerequisites

Ensure you have CMake and a compatible C++ compiler installed on your system.

### Build Instructions

1. Prepare the build directory:
   `cmake -S . -B build`
2. Compile the binary:
   `cmake --build build`

### Execution

Run the compiled executable and pass your `.bl` source file as an argument:

- **Standard Execution:**
  `.\build\blan.exe examples/example.bl`
- **Debug Execution:**
  `.\build\blan.exe examples/example.bl --debug`

> Website is WIP, keep an eye on that. If you liked the project, consider starring the repo and follow me for more such chutiyapa.
