# Examples and Error Handling

This document outlines standard code examples and the specific error-handling mechanisms built into the Bad Language (Blan) Compiler.

## Basic Example

Here is a full, working example demonstrating program structure, variable assignment, and standard output.

```bl
Haan Meri Jaan
bhadwa x matlb 10
bolna x
Bhag Bsdk
```

### Explanation of Syntax

| Syntax              | Description                                                       |
| ------------------- | ----------------------------------------------------------------- |
| `Haan Meri Jaan`    | The mandatory entry point that initializes the program scope.     |
| `bhadwa x matlb 10` | Declares a variable `x` and assigns the integer value `10` to it. |
| `bolna x`           | Built-in function that prints the value of `x` to the console.    |
| `Bhag Bsdk`         | The mandatory exit point that safely terminates the program.      |

## Data Types and Operations

### Strings and Assignment

```bl
Haan Meri Jaan
bhadwa name matlb "Aditya"
bolna name
Bhag Bsdk
```

### Booleans

```bl
Haan Meri Jaan
bhadwa isTrue matlb sach
bhadwa isFalse matlb jhooth
Bhag Bsdk
```

## Error Handling

The compiler implements strict error checking across its different phases. Errors are categorized and labeled with specific language keywords to maintain the theme.

### 1. Syntax Errors (`BehenChod!`)

These errors occur during the Lexical and Parsing phases. They are thrown when the code violates the structural grammar of the language (e.g., missing keywords, malformed expressions, or tokens trailing after the exit point).

**Example Trigger:**
If you forget to close the program with `Bhag Bsdk`, or misspell a keyword.

```text
Syntax Error: BehenChod! Unexpected token found...
```

### 2. Runtime Errors (`CHUDDI!`)

These errors occur during the Semantic Analysis and Evaluation phase. They are thrown when structurally correct code attempts an impossible or illegal operation (e.g., trying to subtract a string from a number, or referencing an undeclared variable).

**Example Trigger:**

```bl
Haan Meri Jaan
bhadwa x matlb 10
bhadwa y matlb "string"
bhadwa result matlb x - y  // Will trigger a runtime error
Bhag Bsdk
```

```text
Runtime Error: CHUDDI! Type mismatch in arithmetic operation...
```
