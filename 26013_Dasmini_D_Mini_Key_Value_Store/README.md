# Command-Line Key-Value Store

A simple in-memory key-value store written in C, using a hash table.

## Features
- Store, retrieve, and delete key-value pairs
- Hash table with collision handling (chaining)
- Interactive command-line interface

## Build
```bash
gcc *.c -o store
./store
```

## Usage
```bash
make
> SET name Alice
OK
> GET name
Alice
> DELETE name
OK
> EXIT
```

## Commands
| Command | Description |
|---------|-------------|
| `SET <key> <value>` | Store a value under a key |
| `GET <key>` | Print the value for a key |
| `DELETE <key>` | Remove a key |
| `EXIT` | Quit the program |
| `LIST` | List all the entries |
| `HELP` | Display how to use every command |

## Project Structure
- `main.c` – command-line loop and parsing
- `kvstore.c` - initialization and all command functions
- `storage.c` - saving to and loading from the db file