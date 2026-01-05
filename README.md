# C++ Interpreter

This project is a C++ implementation of a programming language interpreter. Currently, it supports **tokenization** (lexical analysis) and **parsing** of expressions. The parser converts arithmetic and boolean expressions into a string representation (S-expressions).

## Project Overview

The interpreter is being implemented in stages:
1.  **Tokenization:** The lexer recognizes various token types including keywords, identifiers, literals, and operators.
2.  **Parsing:** The parser uses a recursive descent approach to evaluate expressions and print their Abstract Syntax Tree (AST).

## Getting Started

### Prerequisites

- C++ compiler with C++23 support
- CMake (version 3.13 or higher)
- vcpkg package manager (optional)

### Building the Project

1. Clone the repository
2. Run the build script:

```bash
./start.sh
```

This will compile the project using CMake.

### Usage
**Tokenization**

To tokenize a file:

```bash
./build/interpreter tokenize path/to/your/file.ext
```

The program will output each token in the format:
```
TOKEN_TYPE lexeme value
```

For example:
```
LEFT_PAREN ( null
IDENTIFIER print null
STRING "Hello, World!" Hello, World!
RIGHT_PAREN ) null
```


**Parsing Expressions**

To parse an expression and see its string representation:
```bash
./build/interpreter parse path/to/your/file.ext
```

For an input like 1 + 2 * 3, the output will be an S-expression:
```
(+ 1 (* 2 3))
```

## Features

Current features include:

**Tokenization**
- Recognition of keywords (like var, if, while, etc.)

- Identification of variable names (identifiers)

- Handling of string and number literals

- Basic error reporting for unterminated strings and unexpected characters


**Parsing**
- Recursive Descent Parser: Parses expressions with correct operator precedence.

- Supported Expressions:

    - Equality (==, !=)

    - Comparison (<, <=, >, >=)

    - Arithmetic (+, -, *, /)

    - Unary (!, -)

    - Grouping (( ... ))

- S-Expression Output: Prints the parsed Abstract Syntax Tree (AST) in a standardized string format.
