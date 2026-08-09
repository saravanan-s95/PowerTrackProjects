# Lexical analyzer

A C-based command-line application to display lexeme-token pair of .c file. 

## Features

1.  Displays the following lexemes and matching token names.
    1.	KEYWORD
    2.	IDENTIFIER
    3.	INTEGER_CONSTANT
    4.	FLOAT_CONSTANT
    5.	CHAR_CONSTANT
    6.	STRING_CONSTANT
    7.	ARITHMETIC_OPERATOR
    8.	ASSIGNMENT_OPERATOR
    9.	RELATIONAL_OPERATOR
    10.	LOGICAL_OPERATOR
    11.	BITWISE_OPERATOR
    12.	INCREMENT_DECREMENT_OPERATOR
    13.	MEMBER_ACCESS_OPERATOR
    14.	OPEN_PARENTHESIS
    15.	CLOSE_PARENTHESIS
    16.	OPEN_CURLY_BRACE
    17.	CLOSE_CURLY_BRACE
    18.	OPEN_SQUARE_BRACKET
    19.	CLOSE_SQUARE_BRACKET
    20.	PREPROCESSOR_DIRECTIVE
    21.	SEMICOLON
    22.	COMMA
    23. TERNARY_OPERATOR
2. Skips single and multiline comments


## Build

```bash
make
```

This produces `lexical_analyzer`.

To remove build artifacts:

```bash
make clean
```

## Usage

### To display tokens

```bash
./lexical_analyzer <filename.c>
```

## Error Handling

* Prints an error if the file cannot be opened.
* Prints an error if the file does not have a `.c` extension.
* Prints input syntax.
