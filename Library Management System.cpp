#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// --- Constants ---
#define LIB_FILENAME "library.dat"
#define LIB_MAX_TITLE_LENGTH 100
#define LIB_MAX_AUTHOR_LENGTH 50
#define LIB_MAX_BOOKS 1000

// --- Structure Definition ---
struct Book {
    int book_id;
    char title[LIB_MAX_TITLE_LENGTH];
    char author[LIB_MAX_AUTHOR_LENGTH];
    bool is_issued; // true if issued, false if available
};

// --- Function Prototypes ---
// Utility
void lib_clearScreen();
void lib_clearInputBuffer();
void lib_pressEnterToContinue();
void lib_displayMenu();

// CRUD operations
void lib_addBook();
void lib_displayAllBooks();
void lib_searchBook();
void lib_issueReturnBook(bool issue_operation); // true for issue, false for return
void lib_deleteBook();

// File I/O helpers
int lib_loadBooks(struct Book book_array[]);
void lib_saveBooks(struct Book book_array[], int count);

// --- Main Function for Library System ---
int main() {
    int choice;
    do {
        lib_clearScreen();
        lib_displayMenu();
        printf("Enter your choice: ");
        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 6) {
            printf("Invalid choice. Please enter a number between 0 and 6: ");
            lib_clearInputBuffer();
        }
        lib_clearInputBuffer();

        switch (choice) {
            case 1: lib_addBook(); break;
            case 2: lib_displayAllBooks(); break;
            case 3: lib_searchBook(); break;
            case 4: lib_issueReturnBook(true); break; // Issue Book
            case 5: lib_issueReturnBook(false); break; // Return Book
            case 6: lib_deleteBook(); break;
            case 0: printf("\nExiting Library Management System. Goodbye!\n"); break;
            default: printf("\nAn unexpected error occurred.\n"); break;
        }
        if (choice != 0) {
            lib_pressEnterToContinue();
        }
    } while (choice != 0);

    return 0;
}

// --- Utility Functions Implementation ---
void lib_clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void lib_clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lib_pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    lib_clearInputBuffer();
    getchar();
}

void lib_displayMenu() {
    printf("=======================================\n");
    printf("  LIBRARY MANAGEMENT SYSTEM\n");
    printf("=======================================\n");
    printf("1. Add New Book\n");
    printf("2. Display All Books\n");
    printf("3. Search Book\n");
    printf("4. Issue Book\n");
    printf("5. Return Book\n");
    printf("6. Delete Book\n");
    printf("0. Exit\n");
    printf("---------------------------------------\n");
}

// --- File I/O Helper Functions Implementation ---
int lib_loadBooks(struct Book book_array[]) {
    FILE *fp = fopen(LIB_FILENAME, "rb");
    if (fp == NULL) return 0; // File doesn't exist or empty

    int count = 0;
    while (count < LIB_MAX_BOOKS && fread(&book_array[count], sizeof(struct Book), 1, fp) == 1) {
        count++;
    }
    fclose(fp);
    return count;
}

void lib_saveBooks(struct Book book_array[], int count) {
    FILE *fp = fopen(LIB_FILENAME, "wb"); // Overwrites existing file
    if (fp == NULL) {
        perror("Error opening file for saving books");
        return;
    }
    fwrite(book_array, sizeof(struct Book), count, fp);
    fclose(fp);
}

// --- CRUD Operations Implementation ---
void lib_addBook() {
    lib_clearScreen();
    printf("--- Add New Book ---\n");
    struct Book new_book;
    struct Book existing_books[LIB_MAX_BOOKS];
    int current_count = lib_loadBooks(existing_books);

    printf("Enter Book ID: ");
    while (scanf("%d", &new_book.book_id) != 1 || new_book.book_id <= 0) {
        printf("Invalid ID. Enter a positive integer: ");
        lib_clearInputBuffer();
    }
    lib_clearInputBuffer();

    for (int i = 0; i < current_count; i++) {
        if (existing_books[i].book_id == new_book.book_id) {
            printf("Error: Book with ID %d already exists.\n", new_book.book_id);
            return;
        }
    }

    printf("Enter Title (max %d chars): ", LIB_MAX_TITLE_LENGTH - 1);
    fgets(new_book.title, LIB_MAX_TITLE_LENGTH, stdin);
    new_book.title[strcspn(new_book.title, "\n")] = 0;

    printf("Enter Author (max %d chars): ", LIB_MAX_AUTHOR_LENGTH - 1);
    fgets(new_book.author, LIB_MAX_AUTHOR_LENGTH, stdin);
    new_book.author[strcspn(new_book.author, "\n")] = 0;

    new_book.is_issued = false; // New books are initially not issued

    if (current_count < LIB_MAX_BOOKS) {
        existing_books[current_count] = new_book;
        current_count++;
        lib_saveBooks(existing_books, current_count);
        printf("\nBook added successfully!\n");
    } else {
        printf("\nSystem capacity reached. Cannot add more books.\n");
    }
}

void lib_displayAllBooks() {
    lib_clearScreen();
    printf("--- All Books in Library ---\n");
    struct Book books[LIB_MAX_BOOKS];
    int count = lib_loadBooks(books);

    if (count == 0) {
        printf("\nNo books found in the library.\n");
        return;
    }

    printf("-------------------------------------------------------------------------------------------------\n");
    printf("%-8s %-*s %-*s %-10s\n", "ID", LIB_MAX_TITLE_LENGTH, "Title", LIB_MAX_AUTHOR_LENGTH, "Author", "Status");
    printf("-------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-8d %-*s %-*s %-10s\n",
               books[i].book_id,
               LIB_MAX_TITLE_LENGTH, books[i].title,
               LIB_MAX_AUTHOR_LENGTH, books[i].author,
               books[i].is_issued ? "Issued" : "Available");
    }
    printf("-------------------------------------------------------------------------------------------------\n");
}

void lib_searchBook() {
    lib_clearScreen();
    printf("--- Search Book ---\n");
    int search_id;
    char search_title[LIB_MAX_TITLE_LENGTH];
    int choice;
    bool found = false;

    printf("Search by:\n1. Book ID\n2. Title\nEnter choice: ");
    while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Invalid choice. Enter 1 or 2: ");
        lib_clearInputBuffer();
    }
    lib_clearInputBuffer();

    struct Book books[LIB_MAX_BOOKS];
    int count = lib_loadBooks(books);

    if (choice == 1) {
        printf("Enter Book ID to search: ");
        while (scanf("%d", &search_id) != 1 || search_id <= 0) {
            printf("Invalid ID. Enter a positive integer: ");
            lib_clearInputBuffer();
        }
        lib_clearInputBuffer();
        for (int i = 0; i < count; i++) {
            if (books[i].book_id == search_id) {
                printf("\nBook Found:\n");
                printf("ID: %d\nTitle: %s\nAuthor: %s\nStatus: %s\n",
                       books[i].book_id, books[i].title, books[i].author,
                       books[i].is_issued ? "Issued" : "Available");
                found = true;
                break;
            }
        }
    } else { // Search by Title
        printf("Enter Title to search: ");
        fgets(search_title, LIB_MAX_TITLE_LENGTH, stdin);
        search_title[strcspn(search_title, "\n")] = 0;
        for (int i = 0; i < count; i++) {
            // Case-insensitive search using strstr or custom logic if needed
            if (strstr(books[i].title, search_title) != NULL) {
                printf("\nBook Found:\n");
                printf("ID: %d\nTitle: %s\nAuthor: %s\nStatus: %s\n",
                       books[i].book_id, books[i].title, books[i].author,
                       books[i].is_issued ? "Issued" : "Available");
                found = true;
                // Don't break to show all matches
            }
        }
    }

    if (!found) {
        printf("\nBook not found.\n");
    }
}

void lib_issueReturnBook(bool issue_operation) {
    lib_clearScreen();
    printf("--- %s Book ---\n", issue_operation ? "Issue" : "Return");
    int book_id;
    bool found = false;

    printf("Enter Book ID: ");
    while (scanf("%d", &book_id) != 1 || book_id <= 0) {
        printf("Invalid ID. Enter a positive integer: ");
        lib_clearInputBuffer();
    }
    lib_clearInputBuffer();

    struct Book books[LIB_MAX_BOOKS];
    int count = lib_loadBooks(books);

    for (int i = 0; i < count; i++) {
        if (books[i].book_id == book_id) {
            found = true;
            if (issue_operation) { // Issue operation
                if (!books[i].is_issued) {
                    books[i].is_issued = true;
                    printf("Book '%s' (ID: %d) successfully issued.\n", books[i].title, books[i].book_id);
                } else {
                    printf("Book '%s' (ID: %d) is already issued.\n", books[i].title, books[i].book_id);
                }
            } else { // Return operation
                if (books[i].is_issued) {
                    books[i].is_issued = false;
                    printf("Book '%s' (ID: %d) successfully returned.\n", books[i].title, books[i].book_id);
                } else {
                    printf("Book '%s' (ID: %d) is already available.\n", books[i].title, books[i].book_id);
                }
            }
            lib_saveBooks(books, count); // Save changes
            break;
        }
    }

    if (!found) {
        printf("\nBook with ID %d not found.\n", book_id);
    }
}

void lib_deleteBook() {
    lib_clearScreen();
    printf("--- Delete Book ---\n");
    int delete_id;
    bool found = false;
    int delete_index = -1;

    printf("Enter Book ID to delete: ");
    while (scanf("%d", &delete_id) != 1 || delete_id <= 0) {
        printf("Invalid ID. Enter a positive integer: ");
        lib_clearInputBuffer();
    }
    lib_clearInputBuffer();

    struct Book books[LIB_MAX_BOOKS];
    int count = lib_loadBooks(books);

    for (int i = 0; i < count; i++) {
        if (books[i].book_id == delete_id) {
            found = true;
            delete_index = i;
            break;
        }
    }

    if (!found) {
        printf("\nBook with ID %d not found.\n", delete_id);
        return;
    }

    // Shift elements to overwrite the deleted book
    for (int i = delete_index; i < count - 1; i++) {
        books[i] = books[i+1];
    }
    count--; // Decrement the total count

    lib_saveBooks(books, count); // Save the modified list
    printf("\nBook with ID %d deleted successfully!\n", delete_id);
}