# Address Book Management System

A command-line Address Book application developed in C that allows users to efficiently manage contact information. The application provides functionalities to create, search, edit, delete, and save contact records using file handling techniques.

## Features

* Add new contacts
* Search contacts by name, phone number, or email
* Edit existing contact details
* Delete contacts
* View all contacts
* Persistent storage using CSV file handling
* Input validation for phone numbers and email addresses
* Menu-driven user interface

## Technologies Used

* C Programming
* Structures
* File Handling
* Dynamic Memory Management
* String Manipulation

## Project Overview

The Address Book Management System is designed to store and manage personal contact information. Each contact record contains:

* Name
* Mobile Number
* Email Address

The application maintains records in a CSV file, ensuring data persistence between program executions.

## Project Structure

```text
.
├── main.c
├── contact.c
├── contact.h
├── populate.c
├── file.c
├── addressBook.csv
└── README.md
```

## Functionalities

### Add Contact

Creates a new contact record and stores it in the address book.

```text
Name   : John Doe
Phone  : 9876543210
Email  : john@example.com
```

### Search Contact

Search contacts using:

* Name
* Phone Number
* Email Address

### Edit Contact

Modify existing contact details without affecting other records.

### Delete Contact

Remove unwanted contacts from the address book.

### List Contacts

Display all stored contacts in a formatted manner.

## Build Instructions

Compile the project using GCC:

```bash
gcc *.c -o addressbook
```

## Usage

Run the executable:

```bash
./addressbook
```

Sample Menu:

```text
===== ADDRESS BOOK =====

1. Create Contact
2. Search Contact
3. Edit Contact
4. Delete Contact
5. List Contacts
6. Save Contacts
7. Exit

Enter your choice:
```

## Sample Output

### Creating a Contact

```text
Enter Name  : Deepak
Enter Phone : 9876543210
Enter Email : deepak@example.com

Contact added successfully.
```

### Searching a Contact

```text
Enter name to search: Deepak

Name  : Deepak
Phone : 9876543210
Email : deepak@example.com
```

## Learning Outcomes

This project demonstrates:

* Data structures using structures
* File handling in C
* CSV data management
* Searching and sorting concepts
* Input validation techniques
* Modular programming

## Future Improvements

* Contact sorting feature
* Duplicate contact detection
* Password protection
* Contact import/export functionality
* Graphical User Interface (GUI)

## Author

Deepak Jayadev

## License

This project is developed for educational and learning purposes.
