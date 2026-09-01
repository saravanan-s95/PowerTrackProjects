# 📝 Lexical Analyzer in C

## 📌 Brief Summary

The **Lexical Analyzer** is a compiler design project developed in **C** that scans a source code file and converts it into a sequence of meaningful **tokens**. It identifies keywords, identifiers, operators, literals, delimiters, and special symbols while ignoring comments and whitespace. This project demonstrates the first phase of a compiler and provides a foundation for syntax analysis and further compilation stages.

---

# 📖 Overview

A lexical analyzer (also known as a scanner or tokenizer) is the first phase of a compiler. It reads the source code character by character, groups characters into meaningful lexemes, and generates tokens that are passed to the syntax analyzer.

This project implements a lexical analyzer capable of recognizing various token types in a C-like programming language. It efficiently processes source code while reporting lexical errors such as invalid identifiers or unrecognized symbols.

The application primarily performs:

* **Tokenization** – Converts source code into a stream of valid tokens.
* **Lexical Error Detection** – Identifies invalid or unsupported lexical patterns.

---

# 🎯 Problem Statement

Compilers require source code to be broken into meaningful units before parsing and semantic analysis can begin.

The objective of this project is to develop a lexical analyzer that scans source code, identifies valid tokens, ignores unnecessary characters such as whitespace and comments, and reports lexical errors efficiently.

---

# 📂 Datasets

No external datasets are required.

The project uses:

* Source code file (`.c`)
* Token output generated after lexical analysis
* Error report (if invalid tokens are encountered)

Example:

```text
sample.c
Lexical.c
Syntax_Analyzer.c
```

---

# 🛠️ Tools and Technologies Used

### Programming Language

* C

### Operating System

* Linux (Ubuntu)

### Compiler

* GCC

### Development Tools

* VS Code / Vim
* Makefile
* Git & GitHub

### Concepts Used

* Compiler Design
* Lexical Analysis
* Tokenization
* File Handling
* String Manipulation
* Finite State Concepts
* Modular Programming

---

# ⚙️ Methods

### Tokenization Process

1. Read the source code file.
2. Scan the input character by character.
3. Identify lexemes.
4. Classify lexemes into tokens:

   * Keywords
   * Identifiers
   * Operators
   * Constants
   * String Literals
   * Delimiters
   * Special Symbols
5. Ignore comments and whitespace.
6. Generate token output.
7. Report lexical errors if invalid symbols are found.

---

# 💡 Key Insights

* Understood the first phase of compiler design.
* Learned token generation techniques.
* Improved knowledge of file handling and string processing.
* Gained experience in lexical error detection.
* Practiced modular programming and debugging.
* Strengthened problem-solving skills in language processing.

---

# 📤 Output

### Sample Input

```c
#include<stdio.h>

int main()
{
    int a
    int ;
    int c;
    int i;

    a = 10;
    b = 20;
    c = 0;

    if(a < b
    {
        c = a + b;
    }

    for(i = 0; i < 5; i++)
    {
        c = c + i;    //sum
    }

    if(c > 20)
    {
        a = c;
    }
    else
    {
        b = c;
    }

    return 0;


#include<stdio.h> -> Preprocessor Directive
int -> Keyword
main -> Identifier
( -> Symbol
) -> Symbol
{ -> Symbol
int -> Keyword
a -> Identifier
int -> Keyword
; -> Symbol
int -> Keyword
c -> Identifier
; -> Symbol
int -> Keyword
i -> Identifier
; -> Symbol
a -> Identifier
= -> Operator
10 -> Number
; -> Symbol
b -> Identifier
= -> Operator
20 -> Number
; -> Symbol
c -> Identifier
= -> Operator
0 -> Number
; -> Symbol
if -> Keyword
( -> Symbol
a -> Identifier
< -> special character
b -> Identifier
{ -> Symbol
c -> Identifier
= -> Operator
a -> Identifier
+ -> Operator
b -> Identifier
; -> Symbol
} -> Symbol
for -> Keyword
( -> Symbol
i -> Identifier
= -> Operator
0 -> Number
; -> Symbol
i -> Identifier
< -> special character
5 -> Number
; -> Symbol
i -> Identifier
+ -> Operator
+ -> Operator
) -> Symbol
{ -> Symbol
c -> Identifier
= -> Operator
c -> Identifier
+ -> Operator
i -> Identifier
; -> Symbol
} -> Symbol
if -> Keyword
( -> Symbol
c -> Identifier
> -> special character
20 -> Number
) -> Symbol
{ -> Symbol
a -> Identifier
= -> Operator
c -> Identifier
; -> Symbol
} -> Symbol
else -> Keyword
{ -> Symbol
b -> Identifier
= -> Operator
c -> Identifier
; -> Symbol
} -> Symbol
return -> Keyword
0 -> Number
; -> Symbol
Line 5 : Missing semicolon
Line 6 : Missing identifier
Line 14 : Missing closing parenthesis ')'
Missing closing brace '}'

# 🚀 How to Run This Project

### Clone Repository

```bash
git clone https://github.com/yourusername/lexical-analyzer.git
```

### Navigate to Project Folder

```bash
cd lexical-analyzer
```

### Compile

```bash
gcc *.c -o lexer
```

### Run

```bash
./lexer sample.c
```

---

# ✅ Result and Conclusion

The lexical analyzer successfully scans the source code and converts it into a stream of meaningful tokens while detecting lexical errors. The project demonstrates the fundamental concepts of compiler construction, including token recognition, lexical classification, and efficient source code processing. It strengthened my understanding of compiler design, string manipulation, file processing, and modular software development in C.

---

# 🔮 Future Work

* Develop a Syntax Analyzer (Parser).
* Generate a Symbol Table.
* Improve lexical error reporting.
* Support additional programming languages.
* Add line number tracking for tokens.
* Implement finite state machine (FSM)-based token recognition.
* Integrate with semantic analysis and code generation phases.

---

# 👩‍💻 Author

**Manjusha Thiruveedhula**

Electronics and Communication Engineering

**Areas of Interest**

* Embedded Systems
* C Programming
* Compiler Design
* Linux
* Data Structures
* Software Development



