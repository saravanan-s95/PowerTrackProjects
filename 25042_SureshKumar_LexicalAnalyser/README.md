# PROJECT TITLE

## Lexical Analyzer for C Language developed by Suresh Kumar KR

---

# INTRODUCTION

This project is a simple **Lexical Analyzer** developed using the C programming language. The analyzer reads a C source file and breaks the program into meaningful tokens such as keywords, identifiers, constants, operators, symbols, string literals, and character literals. It also detects common lexical errors and displays appropriate error messages with line and column information.

---

# OBJECTIVES

1. To understand the first phase of a compiler.
2. To identify different tokens in a C program.
3. To validate constants and identifiers.
4. To detect lexical errors such as invalid identifiers, invalid constants, unmatched braces, brackets, and parentheses.

---

#  FEATURES

Detects C keywords

Detects identifiers

Detects integer and float constants

Detects string and character literals

Detects operators and symbols

Ignores comments and preprocessor directives

Reports lexical errors with line and column numbers

---

# WORKING PRINCIPLE

### Step 1:

Read the source program from **program.c**

### Step 2:

Store the contents in a buffer

### Step 3:

Scan the source code character by character

### Step 4:

Classify each token

### Step 5:

Display the token type

### Step 6:

Report any lexical errors encountered during scanning

---

# 🔹 LEARNING OUTCOMES

* Understanding of compiler design basics
* Knowledge of lexical analysis phase
* Practical use of string handling functions
* Understanding tokenization techniques
* Experience in error detection and validation
* Improved programming skills in C language

---

# 🔹 CHALLENGES FACED

* Handling comments correctly
* Identifying string and character literals
* Detecting invalid identifiers such as **12abc**
* Validating floating-point numbers
* Tracking line and column numbers for accurate error reporting
* Checking matching braces, brackets, and parentheses
