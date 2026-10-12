# Minishell

A lightweight, custom Unix command-line shell implemented in C. It supports built-in commands, external program execution, dynamic prompt customization, multi-pipe command chaining, and basic job control with signal handling.

---

## Features

### 1. Built-in Commands
* **`pwd`**: Prints the current working directory.
* **`cd <path>`**: Changes the working directory to the specified path.
* **`echo`**: Supports environment and status variable inspections:
  * `echo $$` — Prints the shell's process ID.
  * `echo $?` — Prints the exit status of the last executed command.
  * `echo $SHELL` — Prints the default shell environment variable (`getenv("SHELL")`).
* **`PS1=<new_prompt>`**: Dynamically updates the shell prompt.
* **`exit`**: Exits the Minishell process.

### 2. External Command Execution & Pipelining
* **Single Command Execution**: Executes system binaries via `fork()` and `execvp()`.
* **N-Pipes Support (`|`)**: Supports arbitrarily chained pipelines (e.g., `cat file | grep pattern | wc -l`) using `pipe()` and `dup2()`.

### 3. Job Control & Signal Management
* **`Ctrl + C` (`SIGINT`)**: Interrupts and terminates the active foreground process without exiting the shell.
* **`Ctrl + Z` (`SIGTSTP`)**: Suspends the active foreground process and pushes it onto the stopped jobs list.
* **`jobs`**: Lists all currently suspended processes.
* **`fg`**: Resumes the most recent suspended job in the foreground and waits for its completion.
* **`bg`**: Resumes the most recent suspended job in the background and returns to the prompt immediately.

---

## Project Structure

```text
Minishell/
├── Makefile                # Build configuration
├── README.md               # Project documentation
├── external_commands.txt   # List of supported external commands
└── src/
    ├── header.h            # Structures, macros, colors, and declarations
    ├── main.c              # Entry point and prompt initialization
    ├── repl.c              # REPL loop, input scanning, and signal handlers
    ├── command_handlers.c  # Built-ins, external commands, and pipeline logic
    ├── job_handlers.c      # Job stack linked list management (insert/remove/print)
    └── utils.c             # Tokenizer, pipeline parser, and binary search
```

---

## Building and Running

### Prerequisites
* GCC compiler
* GNU Make
* Linux / POSIX environment

### Compilation
To compile the shell and generate the `minishell` executable:

```bash
make
```

### Running the Shell
Launch Minishell by executing:

```bash
./minishell
```

### Cleaning Up
To remove compiled object files and the generated executable:

```bash
make clean
```

---

## Example Usage

```bash
# Basic external command execution
Mini$hell> ls -la

# Pipelining multiple commands
Mini$hell> cat /etc/passwd | grep root | wc -l

# Customizing the prompt
Mini$hell> PS1=my_shell
my_shell> pwd
/home/user/Minishell

# Checking shell PID and exit status
my_shell> echo $$
Process id -> 12345
my_shell> echo $?
Exit status -> 0

# Job control with Ctrl+Z, jobs, bg, and fg
my_shell> sleep 100
^Z
my_shell> jobs
Process ID : 12350 | Process Name : sleep 100

my_shell> bg
[12350] sleep 100 &

my_shell> exit
```
