# Examples and Error Handling

This document is the practical showcase for Blan. It walks through every example program in the `examples/` folder, shows what each one demonstrates, and gives the expected output so readers can verify behavior quickly.

## Repository Example Files

- [Basic program example](examples/example.bl)
- [If/Else example](examples/if_else.bl)
- [If without else example](examples/if_no_else.bl)
- [While loop example](examples/while.bl)
- [If/Else with While loop example](examples/if_else_while.bl)
- [Boolean condition example](examples/bool_if.bl)
- [Boolean numeric mapping example](examples/bool_implement.bl)
- [Modulo-by-zero error example](examples/error_mod_zero.bl)
- [Unterminated string error example](examples/error_unterminated_string.bl)

## Language Snapshot

Blan currently supports:

- program entry and exit markers: `Haan Meri Jaan` and `Bhag Bsdk`
- variable declarations with `bhadwa`
- assignment with `matlb`
- output with `bolna`
- strings, numbers, and booleans
- arithmetic, comparison, and logical operators
- `if` / `else-if` / `else`
- `while`

Boolean literals are part of the language and are tokenized as `sach` and `jhooth`.
At runtime, boolean values also participate numerically where it makes sense: `sach` behaves like `1` and `jhooth` behaves like `0`.

## Quick Start Example

This is the smallest useful Blan program in the current style.

```bl
Haan Meri Jaan
bhadwa x matlb 10
bolna x
bolna "hello"
Bhag Bsdk
```

Expected output:

```text
10
hello
```

What it shows:

- `Haan Meri Jaan` begins the program
- `bhadwa` declares a variable
- `matlb` assigns a value
- `bolna` prints numbers and strings
- `Bhag Bsdk` ends the program cleanly

## Example Files In Detail

### [examples/example.bl](examples/example.bl)

This is the basic sandbox example. It demonstrates comments, variables, printing, and a second assignment.

```bl
// test 1 --> errorless
// This is an example in blan language
Haan Meri Jaan
// Variables
bhadwa x matlb 10
bolna x
bolna "hello"
// Assign y
bhadwa y matlb 20
Bhag Bsdk
```

Expected output:

```text
10
hello
```

Notes:

- The comments are ignored by the lexer and parser.
- `y` is assigned but not printed, so it exists only to show multiple declarations.

### [examples/if_no_else.bl](examples/if_no_else.bl)

This example demonstrates the minimal valid `if` block with no `else` branch.

```bl
Haan Meri Jaan
agar 1 tab
    bolna "ok"
khtm
Bhag Bsdk
```

Expected output:

```text
ok
```

What it shows:

- `agar` starts the condition
- `tab` closes the condition header
- `khtm` closes the `if` block

### [examples/if_else.bl](examples/if_else.bl)

This is the canonical `if / else-if / else` example.

```bl
Haan Meri Jaan

bhadwa x matlb 10

agar x > 15 tab
    bolna "X is greater than 15"
warna x == 10 tab
    bolna "X is exactly 10"
nahi_toh
    bolna "X is less than 10"
khtm

Bhag Bsdk
```

Expected output:

```text
X is exactly 10
```

What it shows:

- comparison operators inside conditions
- chained conditional branches
- a final fallback branch

### [examples/while.bl](examples/while.bl)

This is the canonical `while` loop example.

```bl
Haan Meri Jaan

bhadwa count matlb 3

JabTak count > 0 TabTak
    bolna count
    bhadwa count matlb count - 1
hogya

Bhag Bsdk
```

Expected output:

```text
3
2
1
```

What it shows:

- loop condition evaluation
- arithmetic decrementing
- repeated execution until the condition becomes false

### [examples/if_else_while.bl](examples/if_else_while.bl)

This combines branching and looping in one file.

```bl
Haan Meri Jaan

bhadwa x matlb 10

agar x > 15 tab
    bolna "X is greater than 15"
warna x == 10 tab
    bolna "X is exactly 10"
nahi_toh
    bolna "X is less than 10"
khtm

bhadwa count matlb 3
JabTak count > 0 TabTak
    bolna count
    bhadwa count matlb count - 1
hogya

Bhag Bsdk
```

Expected output:

```text
X is exactly 10
3
2
1
```

What it shows:

- conditionals and loops can be combined in one program
- nested control flow works cleanly with the current parser/evaluator

### [examples/bool_if.bl](examples/bool_if.bl)

This example verifies that a boolean literal can drive control flow directly.

```bl
Haan Meri Jaan
bhadwa x matlb sach
agar x tab
    bolna "bool works"
khtm
Bhag Bsdk
```

Expected output:

```text
bool works
```

### [examples/bool_implement.bl](examples/bool_implement.bl)

This example demonstrates the numeric mapping of booleans in expressions.

```bl
Haan Meri Jaan
bhadwa x matlb sach
agar x == 1 tab
    bolna "ok"
khtm
Bhag Bsdk
```

Expected output:

```text
ok
```

What it shows:

- `sach` can be compared as `1`
- boolean values integrate into arithmetic and comparisons

## Feature Showcase

### Strings

```bl
Haan Meri Jaan
bhadwa name matlb "Aditya"
bolna name
Bhag Bsdk
```

Expected output:

```text
Aditya
```

### Boolean Logic

```bl
Haan Meri Jaan
bolna !sach
bolna sach && jhooth
bolna jhooth || sach
Bhag Bsdk
```

Expected output:

```text
jhooth
jhooth
sach
```

### Comparison Operators

```bl
Haan Meri Jaan
bolna 5 < 10
bolna 10 >= 10
bolna sach == 1
Bhag Bsdk
```

Expected output:

```text
sach
sach
sach
```

## Complex Combined Example

This example uses variables, booleans, comparisons, logical operators, branching, looping, and output in one small program.

```bl
Haan Meri Jaan

bhadwa count matlb 3
bhadwa ready matlb sach

agar ready && count == 3 tab
    bolna "starting"
warna ready == jhooth tab
    bolna "skipped"
nahi_toh
    bolna "fallback"
khtm

JabTak count > 0 TabTak
    bolna count
    bhadwa count matlb count - 1
hogya

Bhag Bsdk
```

Expected output:

```text
starting
3
2
1
```

Why this one matters:

- it shows boolean-to-number behavior (`ready` is `sach`)
- it combines `&&`, `==`, and branching control flow
- it confirms the parser handles conditionals and loops together

## Error Handling Examples

The compiler uses themed error messages for invalid programs.

### [examples/error_mod_zero.bl](examples/error_mod_zero.bl)

```bl
Haan Meri Jaan
bolna 10 % 0
Bhag Bsdk
```

This is a runtime error example.

Expected behavior:

- the evaluator stops with a modulo-by-zero runtime error

### [examples/error_unterminated_string.bl](examples/error_unterminated_string.bl)

```bl
Haan Meri Jaan
bhadwa s matlb "unterminated
Bhag Bsdk
```

This is a lexical / parsing error example.

Expected behavior:

- the lexer/parser reports an error because the closing quote is missing

## Notes on Legacy Syntax

Older aliases are still accepted for compatibility where needed:

- `warna_agar` can behave like `warna`
- `nhi_toh` can behave like `nahi_toh`
- `bas_itna_hi` can behave like `khtm`

For new examples and documentation, prefer the canonical keywords shown above.

## How To Run These Examples

```powershell
cmake -S . -B build
cmake --build build
./build/blan.exe ./examples/if_else.bl
```

To run the tests for the project, use:

```powershell
ctest --test-dir build --output-on-failure
```
