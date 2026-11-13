# Address Book Management System

## Project Description
The Address Book Management System is a C programming project that allows users to manage contact information. It provides a menu-driven interface to create, search, edit, delete, and list contacts.

## Features
- Create new contacts
- Search for existing contacts
- Edit contact details
- Delete contacts
- List all saved contacts
- Save contacts and exit the application

## Technologies Used
- Programming Language: C
- Concepts: Structures, Pointers, Functions, File Handling
- Compiler: GCC

## Project Files
- `main.c` – Main program and menu interface
- `contact.c`, `contact.h` – Contact management functions
- `file.c`, `file.h` – File handling and contact storage
- `populate.c`, `populate.h` – Contact data initialization
- `contacts.csv`, `contacts.txt` – Contact data files, if used by the program

## Compilation
Compile the project from its source directory:

```bash
gcc -Wall -Wextra main.c contact.c file.c populate.c -o addressbook
```

## Execution
Run the program using:

```bash
./addressbook
```

## Author
Sheeba Insily R Robert

## Batch Number
26013
