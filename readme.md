# Library Management System

This is a simple command-line Library Management System written in C. It allows librarians to manage books and members, perform checkouts and returns, and search for books in the library's collection.

## Features

- Book Management:
  - Add new books
  - Remove books
  - Checkout books to members
  - Return books
  - Search for books by ISBN, title, or author
  - Display all books in the library

- Member Management:
  - Register new members
  - Remove members
  - Display member information

## How to Use

1. Compile the program using a C compiler. For example:
   ```
   gcc -o library_system main.c
   ```

2. Run the compiled program:
   ```
   ./library_system
   ```

3. Follow the on-screen menu to navigate through different functions.

## File Structure

The program uses two text files to store data:

- `books.txt`: Stores information about books (title, author, ISBN, availability)
- `members.txt`: Stores information about library members (name, ID, email, phone, checked out books)

## Functions

- `displayMenu()`: Shows the main menu
- `addBook()`: Adds a new book to the library
- `removeBook()`: Removes a book from the library
- `checkoutBook()`: Checks out a book to a member
- `returnBook()`: Returns a book to the library
- `searchBooks()`: Searches for books by ISBN, title, or author
- `displayBooks()`: Displays all books in the library
- `registerMember()`: Registers a new library member
- `removeMember()`: Removes a member from the library
- `displayMembers()`: Displays information about a specific member

## Small Word:

This is just a small project made by me for testing my skills. Do let me know if you like this project.