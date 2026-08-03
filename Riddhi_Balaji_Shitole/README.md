# Minishell Project

This project is a simple Linux mini shell written in C. It reads commands from the user, checks whether the command is built-in or external, and then executes it. The project also supports pipes, signal handling, and basic job control.

## Features

- Custom shell prompt: `minishell$:-`
- Change prompt using `PS1=new_prompt`
- Execute external Linux commands like `ls`, `cat`, `grep`, `date`, etc.
- Execute built-in commands like `cd`, `pwd`, `exit`, `clear`, and selected `echo` commands
- Support for pipes, for example `ls | grep .c`
- Signal handling for `Ctrl+C` and `Ctrl+Z`
- Basic job control using `jobs`, `fg`, and `bg`

## Project Files

- `main.c` - starts the shell and registers signal handlers
- `main.h` - contains header files, macros, structures, and function declarations
- `scan_input.c` - reads input, checks command type, and executes commands
- `n_pipe.c` - handles pipe execution
- `signal.c` - handles signals and job control
- `bulitin_cmd.txt` - list of built-in commands used by the shell
- `external_cmd.txt` - list of external commands used by the shell

## Requirements

- Linux operating system
- GCC compiler

## How to Compile

Open a terminal inside the project folder:

```sh
cd /home/eswar/LI_MODULE/minishell_project
```

Compile the project:

```sh
gcc main.c scan_input.c n_pipe.c signal.c -o minishell
```

## How to Run

Run the compiled executable:

```sh
./minishell
```

You will see the shell prompt:

```text
minishell$:-
```

Now you can type commands.

## Example Commands

```sh
pwd
ls
cd ..
echo $$
echo $?
echo $SHELL
ls | grep .c
PS1=my_shell$ 
jobs
fg
bg
clear
exit
```

## Notes

- Run the program from the same folder where `bulitin_cmd.txt` and `external_cmd.txt` are present.
- If a command is not listed in `external_cmd.txt` or `bulitin_cmd.txt`, the shell may print `command not found`.
- Use `exit` to close the minishell.
