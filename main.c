#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// library management system

// first lets declare all the functions and all the functionality we want in our system

// we'll use camel case

void displayMenu(); // Prints the main menu

// Book section
void addBook();
void removeBook();
void checkoutBook();
void returnBook();
void searchBooks();
void displayBooks();

// Member section
void registerMember();
void removeMember();
void displayMembers();

// declare structs

struct book
{
    char title[100];
    char author[100];
    char ISBN[14];
    int available; // 1 if available, 0 if not
};

struct member
{
    char name[100];
    char id[20];
    char email[100];
    char phone[15];
    int booksCheckedOut; // Number of books currently checked out
    char checkedOutBooks[10][14]; // Array to store ISBNs of checked out books (max 10)
};


int main()
{

    // Prints the menu and accepts the choice
    int choice;
    do
    {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            removeBook();
            break;
        case 3:
            checkoutBook();
            break;
        case 4:
            returnBook();
            break;
        case 5:
            searchBooks();
            break;
        case 6:
            displayBooks();
            break;
        case 7:
            registerMember();
            break;
        case 8:
            removeMember();
            break;
        case 9:
            displayMembers();
            break;
        case 10:
            printf("Exiting program. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 10);
    
}

void displayMenu()
{
    printf("1. Add Book\n");
    printf("2. Remove Book\n");
    printf("3. Checkout Book\n");
    printf("4. Return Book\n");
    printf("5. Search Books\n");
    printf("6. Display Books\n");
    printf("7. Register Member\n");
    printf("8. Remove Member\n");
    printf("9. Display Members\n");
    printf("10. Exit\n");
}

void addBook()
{
    struct book newbook; // declare a new book

    printf("Enter the title of the book: ");
    scanf("%s", newbook.title);
    printf("Enter the author of the book: ");
    scanf("%s", newbook.author);
    printf("Enter the ISBN of the book: ");
    scanf("%s", newbook.ISBN);
    printf("Enter the available status of the book: ");
    scanf("%d", &newbook.available);

    // add the new book to the list of books
    FILE *fp;
    fp = fopen("books.txt", "a");

    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    fprintf(fp, "%s %s %s %d\n", newbook.title, newbook.author, newbook.ISBN, newbook.available);
    fclose(fp);

    printf("Book added successfully!\n");
}

void removeBook()
{
    char ISBN[14];
    printf("Enter the ISBN of the book to remove: ");
    scanf("%13s", ISBN); // Limit the input to prevent overflow

    FILE *fp = fopen("books.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    struct book currentBook;
    int found = 0;

    while (fscanf(fp, "%99s %99s %13s %d\n", currentBook.title, currentBook.author, currentBook.ISBN, &currentBook.available) != EOF)
    {
        // If the ISBN does not match, write the book to the temp file
        if (strcmp(currentBook.ISBN, ISBN) != 0)
        {
            fprintf(temp, "%s %s %s %d\n", currentBook.title, currentBook.author, currentBook.ISBN, currentBook.available);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    // Replace original file with the temporary file
    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (found)
    {
        printf("Book removed successfully!\n");
    }
    else
    {
        printf("Book with ISBN %s not found.\n", ISBN);
    }
}

void checkoutBook()
{
    char ISBN[14];
    char memberID[20];
    printf("Enter the ISBN of the book to checkout: ");
    scanf("%s", ISBN);
    // Check if the member is registered
    printf("Is the member already registered? (y/n): ");
    char isRegistered;
    scanf(" %c", &isRegistered);
    getchar(); // Consume newline

    if (isRegistered == 'n' || isRegistered == 'N') {
        printf("Would you like to register the member? (y/n): ");
        char wantToRegister;
        scanf(" %c", &wantToRegister);
        getchar(); // Consume newline

        if (wantToRegister == 'y' || wantToRegister == 'Y') {
            registerMember();
            printf("Please enter the newly registered member's ID: ");
            scanf("%s", memberID);
        } else {
            printf("Cannot checkout book without a registered member.\n");
            return;
        }
    }

    

    // checkout the book from the list of books
    // if the book gets checked out successfully, update the available status to 0
    // if the book is not available, print a message to the user
    FILE *fp = fopen("books.txt", "r+");
    FILE *temp = fopen("temp.txt", "w");
    FILE *memberFile = fopen("members.txt", "r+");
    FILE *memberTemp = fopen("memberTemp.txt", "w");

    if (fp == NULL || temp == NULL || memberFile == NULL || memberTemp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    struct book currentBook;
    struct member currentMember;
    int bookFound = 0;
    int memberFound = 0;

    // Check if the book is available
    while (fscanf(fp, "%99s %99s %13s %d\n", currentBook.title, currentBook.author, currentBook.ISBN, &currentBook.available) != EOF)
    {
        if (strcmp(currentBook.ISBN, ISBN) == 0)
        {
            if (currentBook.available == 1)
            {
                currentBook.available = 0;
                fprintf(temp, "%s %s %s %d\n", currentBook.title, currentBook.author, currentBook.ISBN, currentBook.available);
                bookFound = 1;
            }
            else
            {
                fprintf(temp, "%s %s %s %d\n", currentBook.title, currentBook.author, currentBook.ISBN, currentBook.available);
                printf("Sorry, this book is not available for checkout.\n");
                bookFound = -1;
            }
        }
        else
        {
            fprintf(temp, "%s %s %s %d\n", currentBook.title, currentBook.author, currentBook.ISBN, currentBook.available);
        }
    }



    // If book is available, update member's checked out books
    if (bookFound == 1)
    {
        char line[1000];
        while (fgets(line, sizeof(line), memberFile))
        {
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d", 
                   currentMember.name, 
                   currentMember.id, 
                   currentMember.email, 
                   currentMember.phone, 
                   &currentMember.booksCheckedOut);
            
            if (strcmp(currentMember.id, memberID) == 0)
            {
                memberFound = 1;
                if (currentMember.booksCheckedOut < 10)
                {
                    strcpy(currentMember.checkedOutBooks[currentMember.booksCheckedOut], ISBN);
                    currentMember.booksCheckedOut++;
                    fprintf(memberTemp, "%s,%s,%s,%s,%d", 
                            currentMember.name, 
                            currentMember.id, 
                            currentMember.email, 
                            currentMember.phone, 
                            currentMember.booksCheckedOut);
                    for (int i = 0; i < currentMember.booksCheckedOut; i++)
                    {
                        fprintf(memberTemp, ",%s", currentMember.checkedOutBooks[i]);
                    }
                    fprintf(memberTemp, "\n");
                    printf("Book checked out successfully!\n");
                }
                else
                {
                    printf("Member has reached the maximum number of books that can be checked out.\n");
                    fputs(line, memberTemp);
                }
            }
            else
            {
                fputs(line, memberTemp);
            }
        }
    }

    fclose(fp);
    fclose(temp);
    fclose(memberFile);
    fclose(memberTemp);

    // Replace original files with the temporary files
    remove("books.txt");
    rename("temp.txt", "books.txt");
    remove("members.txt");
    rename("memberTemp.txt", "members.txt");

    if (bookFound == 0)
    {
        printf("Book with ISBN %s not found.\n", ISBN);
    }
    if (memberFound == 0)
    {
        printf("Member with ID %s not found.\n", memberID);
    }
}

void returnBook()
{
    // return the book to the list of books
    // if the book gets returned successfully, update the available status to 1
    // if the book is already returned, print a message to the user
    char ISBN[14];
    char memberID[20];
    printf("Enter the ISBN of the book you want to return: ");
    scanf("%13s", ISBN);
    printf("Enter the ID of the member returning the book: ");
    scanf("%s", memberID);

    FILE *fp = fopen("books.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    FILE *memberFile = fopen("members.txt", "r");
    FILE *memberTemp = fopen("memberTemp.txt", "w");

    if (fp == NULL || temp == NULL || memberFile == NULL || memberTemp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    struct book currentBook;
    struct member currentMember;
    int bookFound = 0;
    int memberFound = 0;

    while (fscanf(fp, "%99s %99s %13s %d\n", currentBook.title, currentBook.author, currentBook.ISBN, &currentBook.available) != EOF)
    {
        if (strcmp(currentBook.ISBN, ISBN) == 0)
        {
            if (currentBook.available == 0)
            {
                currentBook.available = 1;
                fprintf(temp, "%s %s %s %d\n", currentBook.title, currentBook.author, currentBook.ISBN, currentBook.available);
                bookFound = 1;
            }
            else
            {
                fprintf(temp, "%s %s %s %d\n", currentBook.title, currentBook.author, currentBook.ISBN, currentBook.available);
                printf("This book is already available in the library.\n");
                bookFound = -1;
            }
        }
        else
        {
            fprintf(temp, "%s %s %s %d\n", currentBook.title, currentBook.author, currentBook.ISBN, currentBook.available);
        }
    }

    if (bookFound == 1)
    {
        char line[1000];
        while (fgets(line, sizeof(line), memberFile))
        {
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d", 
                   currentMember.name, 
                   currentMember.id, 
                   currentMember.email, 
                   currentMember.phone, 
                   &currentMember.booksCheckedOut);
            
            if (strcmp(currentMember.id, memberID) == 0)
            {
                memberFound = 1;
                int i;
                for (i = 0; i < currentMember.booksCheckedOut; i++)
                {
                    if (strcmp(currentMember.checkedOutBooks[i], ISBN) == 0)
                    {
                        // Remove the book from the member's checked out books
                        for (int j = i; j < currentMember.booksCheckedOut - 1; j++)
                        {
                            strcpy(currentMember.checkedOutBooks[j], currentMember.checkedOutBooks[j+1]);
                        }
                        currentMember.booksCheckedOut--;
                        break;
                    }
                }
                fprintf(memberTemp, "%s,%s,%s,%s,%d", 
                        currentMember.name, 
                        currentMember.id, 
                        currentMember.email, 
                        currentMember.phone, 
                        currentMember.booksCheckedOut);
                for (i = 0; i < currentMember.booksCheckedOut; i++)
                {
                    fprintf(memberTemp, ",%s", currentMember.checkedOutBooks[i]);
                }
                fprintf(memberTemp, "\n");
                printf("Book returned successfully!\n");
            }
            else
            {
                fputs(line, memberTemp);
            }
        }
    }

    fclose(fp);
    fclose(temp);
    fclose(memberFile);
    fclose(memberTemp);

    // Replace original files with the temporary files
    remove("books.txt");
    rename("temp.txt", "books.txt");
    remove("members.txt");
    rename("memberTemp.txt", "members.txt");

    if (bookFound == 0)
    {
        printf("Book with ISBN %s not found in the library.\n", ISBN);
    }
    if (memberFound == 0)
    {
        printf("Member with ID %s not found.\n", memberID);
    }
}

void searchBooks()
{
    // search for a book by ISBN, title, or author
    // if the book is found, print the details of the book
    // if the book is not found, print a message to the user
    char searchTerm[100];
    int searchType;
    
    printf("Search by:\n");
    printf("1. ISBN\n");
    printf("2. Title\n");
    printf("3. Author\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &searchType);
    getchar(); // To remove the extra newline character

    printf("Enter search term: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0; // Remove trailing newline

    FILE *fp;
    fp = fopen("books.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    struct book currentBook;
    int found = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp))
    {
        sscanf(line, "%99[^,],%99[^,],%13[^,],%d", 
               currentBook.title, 
               currentBook.author, 
               currentBook.ISBN, 
               &currentBook.available);

        int match = 0;
        if (searchType == 1)
        {
            if (strcmp(currentBook.ISBN, searchTerm) == 0)
            {
                match = 1;
            }
        }
        else if (searchType == 2)
        {
            if (strstr(currentBook.title, searchTerm) != NULL)
            {
                match = 1;
            }
        }
        else if (searchType == 3)
        {
            if (strstr(currentBook.author, searchTerm) != NULL)
            {
                match = 1;
            }
        }

        if (match == 1)
        {
            printf("Title: %s\n", currentBook.title);
            printf("Author: %s\n", currentBook.author);
            printf("ISBN: %s\n", currentBook.ISBN);
            if (currentBook.available == 1)
            {
                printf("Available: Yes\n\n");
            }
            else
            {
                printf("Available: No\n\n");
            }
            found = 1;
        }
    }

    fclose(fp);

    if (found == 0)
    {
        printf("No books found matching the search term.\n");
    }
}

void displayBooks()
{
    // display all the books in the library
    FILE *fp;
    fp = fopen("books.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    struct book currentBook;
    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        sscanf(line, "%99[^,],%99[^,],%13[^,],%d", 
               currentBook.title, 
               currentBook.author, 
               currentBook.ISBN, 
               &currentBook.available);

        printf("Title: %s\n", currentBook.title);
        printf("Author: %s\n", currentBook.author);
        printf("ISBN: %s\n", currentBook.ISBN);
        if (currentBook.available == 1)
        {
            printf("Available: Yes\n\n");
        }
        else
        {
            printf("Available: No\n\n");
        }
        printf("**********************\n");
    }

    fclose(fp);
}

void registerMember()
{
    // register a new member to the library
    // if the member is registered successfully, print a message to the user
    struct member newMember;
    printf("Enter the name of the member: ");
    fgets(newMember.name, sizeof(newMember.name), stdin);
    newMember.name[strcspn(newMember.name, "\n")] = 0;

    printf("Enter the ID of the member: ");
    fgets(newMember.id, sizeof(newMember.id), stdin);
    newMember.id[strcspn(newMember.id, "\n")] = 0;

    printf("Enter the email of the member: ");
    fgets(newMember.email, sizeof(newMember.email), stdin);
    newMember.email[strcspn(newMember.email, "\n")] = 0;

    printf("Enter the phone number of the member: ");
    fgets(newMember.phone, sizeof(newMember.phone), stdin);
    newMember.phone[strcspn(newMember.phone, "\n")] = 0;

    newMember.booksCheckedOut = 0;
    // Initialize checkedOutBooks to empty strings
    for (int i = 0; i < 10; i++)
    {
        strcpy(newMember.checkedOutBooks[i], "");
    }

    // Open the file in append mode
    FILE *fp = fopen("members.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening file. Unable to register member.\n");
        return;
    }

    // Write the new member's information to the file
    fprintf(fp, "%s,%s,%s,%s,%d", newMember.name, newMember.id, newMember.email, newMember.phone, newMember.booksCheckedOut);
    for (int i = 0; i < 10; i++) {
        fprintf(fp, ",%s", newMember.checkedOutBooks[i]);
    }
    fprintf(fp, "\n");

    // Close the file
    fclose(fp);

    printf("Member %s has been registered successfully with ID: %s\n", newMember.name, newMember.id);
}

void removeMember()
{
    // remove a member from the library
    // if the member is removed successfully, print a message to the user
    char id[20];
    printf("Enter the ID of the member to remove: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0; // Remove trailing newline

    FILE *fp = fopen("members.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    // Check if we can open the files
    if (fp == NULL || temp == NULL)
    {
        printf("Error opening file. Unable to remove member.\n");
        return;
    }

    char line[1000];  // To store each line from the file
    int found = 0;    // Flag to check if we found the member
    
    // Read the file line by line
    while (fgets(line, sizeof(line), fp))
    {
        char name[100], current_id[20], email[100], phone[15];
        int booksCheckedOut;
        
        // Extract the ID from the current line
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d", name, current_id, email, phone, &booksCheckedOut);
        
        // If this is the member we want to remove
        if (strcmp(current_id, id) == 0)
        {
            found = 1;
            // Skip this line (effectively removing the member)
            continue;
        }
        
        // If it's not the member we're removing, write the line to the temp file
        fputs(line, temp);
    }

    // Close both files
    fclose(fp);
    fclose(temp);

    // If we found and removed the member
    if (found)
    {
        // Replace the original file with the updated one
        remove("members.txt");
        rename("temp.txt", "members.txt");
        printf("Member with ID %s has been removed successfully.\n", id);
    }
    else
    {
        // If we didn't find the member, just delete the temp file
        remove("temp.txt");
        printf("Member with ID %s not found.\n", id);
    }
}

void displayMembers()
{
    // display a specific member's information
    char id[20];
    printf("Enter the ID of the member to display: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0; // Remove trailing newline

    FILE *fp;
    fp = fopen("members.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    struct member currentMember;
    int found = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d", 
               currentMember.name, 
               currentMember.id, 
               currentMember.email, 
               currentMember.phone, 
               &currentMember.booksCheckedOut);
        
        if (strcmp(currentMember.id, id) == 0)
        {
            found = 1;
            printf("\nMember Information:\n");
            printf("Name: %s\n", currentMember.name);
            printf("ID: %s\n", currentMember.id);
            printf("Email: %s\n", currentMember.email);
            printf("Phone: %s\n", currentMember.phone);
            printf("Books Checked Out: %d\n", currentMember.booksCheckedOut);
            break;
        }
    }
    
    fclose(fp);
    
    if (found==0)
    {
        printf("Member with ID %s not found.\n", id);
    }
}
