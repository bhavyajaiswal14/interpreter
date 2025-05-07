# C++ Interpreter

This project is a C++ implementation of a programming language interpreter. Currently, it supports the tokenization (lexical analysis) stage of the interpretation process.

## Project Overview

The interpreter is being implemented in stages, with the first stage focusing on tokenization. The lexer recognizes various token types including:

- Keywords (like `var`, `if`, `while`, etc.)
- Identifiers (variable names)
- Literals (strings and numbers)
- Operators and punctuation

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

## Features

Current features include:
- Tokenization of a source file
- Recognition of keywords, identifiers, literals, operators, and punctuation
- Basic error reporting for unterminated strings and unexpected characters
