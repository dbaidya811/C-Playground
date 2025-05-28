#include <stdio.h>    // For standard input/output functions (printf, scanf, fopen, fclose, etc.)
#include <stdlib.h>   // For system() function (to clear screen, pause)
#include <string.h>   // For string manipulation functions (strcpy, strcmp, strcspn)
#include <stdbool.h>  // For boolean data type (true, false)

// Conditional compilation for Windows specific headers/functions
#ifdef _WIN32
    #include <windows.h> // For Sleep() if needed, though system("pause") is often sufficient
#else
    #include <unistd.h> // For usleep() on Unix/Linux systems if specific delays are needed
#endif

// --- Constants ---
#define FILENAME "students.dat"      // The name of the binary file to store student data
#define MAX_NAME_LENGTH 50           // Maximum length for a student's name
#define MAX_STUDENTS 1000            // Maximum number of students the system can handle in memory

// --- Structure Definition ---
// Defines the blueprint for a single student record
struct Student {
    int roll_no;                // Unique identifier for the student
    char name[MAX_NAME_LENGTH]; // Student's name
    float marks;                // Student's marks (e.g., out of 100)
};

// --- Function Prototypes ---
// Utility functions for UI and input handling
void clearScreen();
void clearInputBuffer();
void pressEnterToContinue();
void displayMenu();

// CRUD operations
void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

// File I/O helper functions (internal use by CRUD operations)
// These functions operate on the global FILENAME
int loadStudentsFromFile(struct Student student_array[]);
void saveStudentsToFile(struct Student student_array[], int count);

// --- Main Function ---
int main() {
    int choice; // To store user's menu choice

    do {
        clearScreen();   // Clear the console for a clean menu display
        displayMenu();   // Show the main menu options
        printf("Enter your choice: ");

        // Input validation loop for menu choice
        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 5) {
            printf("Invalid choice. Please enter a number between 0 and 5: ");
            clearInputBuffer(); // Clear buffer to prevent infinite loops on bad input
        }
        clearInputBuffer(); // Clear the newline character left by scanf

        // Perform action based on user's choice
        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 0: // Exit option
                printf("\nExiting Student Record Management System. Goodbye!\n");
                break;
            default:
                // This case should ideally not be reached due to input validation
                printf("\nAn unexpected error occurred.\n");
                break;
        }

        // Pause before redisplaying the menu, unless exiting
        if (choice != 0) {
            pressEnterToContinue();
        }

    } while (choice != 0); // Continue loop until user chooses to exit

    return 0; // Indicate successful program execution
}

// --- Utility Functions Implementation ---

// Clears the console screen
void clearScreen() {
    #ifdef _WIN32 // For Windows operating systems
        system("cls");
    #else         // For Unix-like operating systems (Linux, macOS)
        system("clear");
    #endif
}

// Clears the standard input buffer
// This is essential after using scanf to read numbers, before using fgets to read strings.
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Pauses the program execution until the user presses Enter
void pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    clearInputBuffer(); // Ensure buffer is empty before reading the Enter key
    getchar(); // Reads the Enter key press
}

// Displays the main menu options to the user
void displayMenu() {
    printf("=======================================\n");
    printf("  STUDENT RECORD MANAGEMENT SYSTEM\n");
    printf("=======================================\n");
    printf("1. Add New Student\n");
    printf("2. Display All Students\n");
    printf("3. Search Student by Roll Number\n");
    printf("4. Update Student Record\n");
    printf("5. Delete Student Record\n");
    printf("0. Exit\n");
    printf("---------------------------------------\n");
}

// --- File I/O Helper Functions Implementation ---

/*
 * Loads all student records from the binary file into a Student array in memory.
 * Returns the number of students loaded.
 */
int loadStudentsFromFile(struct Student student_array[]) {
    FILE *fp;
    int count = 0;

    fp = fopen(FILENAME, "rb"); // Open file in read binary mode
    if (fp == NULL) {
        // If file doesn't exist yet (first run) or cannot be opened, return 0 students.
        // perror("Error opening file for reading"); // Uncomment for debugging file issues
        return 0;
    }

    // Read student records one by one until EOF or MAX_STUDENTS is reached
    while (count < MAX_STUDENTS && fread(&student_array[count], sizeof(struct Student), 1, fp) == 1) {
        count++;
    }

    fclose(fp); // Close the file
    return count; // Return the total number of students loaded
}

/*
 * Saves all student records from a Student array in memory to the binary file.
 * This overwrites the existing file content.
 */
void saveStudentsToFile(struct Student student_array[], int count) {
    FILE *fp;

    // Open file in write binary mode. "wb" truncates the file if it exists.
    fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        perror("Error opening file for writing (saving all students)");
        return;
    }

    // Write all student records from the array to the file
    fwrite(student_array, sizeof(struct Student), count, fp);

    fclose(fp); // Close the file
}

// --- CRUD Operations Implementation ---

// Adds a new student record to the system
void addStudent() {
    clearScreen();
    printf("--- Add New Student ---\n");
    struct Student new_student; // Create a new student struct to populate

    // Load existing students to check for duplicate roll numbers
    struct Student current_students[MAX_STUDENTS];
    int current_count = loadStudentsFromFile(current_students);

    // Get Roll Number with validation and duplicate check
    printf("Enter Roll Number: ");
    while (scanf("%d", &new_student.roll_no) != 1 || new_student.roll_no <= 0) {
        printf("Invalid Roll Number. Please enter a positive integer: ");
        clearInputBuffer();
    }
    clearInputBuffer(); // Clear newline after scanf

    // Check for duplicate roll number before proceeding
    for (int i = 0; i < current_count; i++) {
        if (current_students[i].roll_no == new_student.roll_no) {
            printf("\nError: Student with Roll Number %d already exists.\n", new_student.roll_no);
            printf("Please choose a unique Roll Number.\n");
            return; // Exit function if duplicate found
        }
    }

    // Get Name with validation
    printf("Enter Name (max %d characters): ", MAX_NAME_LENGTH - 1);
    // fgets reads the string safely, including spaces, until newline or MAX_NAME_LENGTH-1 chars
    if (fgets(new_student.name, MAX_NAME_LENGTH, stdin) != NULL) {
        // Remove the trailing newline character that fgets might include
        new_student.name[strcspn(new_student.name, "\n")] = 0;
    } else {
        printf("Error reading name. Please try again.\n");
        return;
    }
    // Basic check for empty name after stripping newline
    if (strlen(new_student.name) == 0) {
        printf("Name cannot be empty. Please try again.\n");
        return;
    }

    // Get Marks with validation
    printf("Enter Marks (0.0 to 100.0): ");
    while (scanf("%f", &new_student.marks) != 1 || new_student.marks < 0.0 || new_student.marks > 100.0) {
        printf("Invalid Marks. Please enter a value between 0.0 and 100.0: ");
        clearInputBuffer();
    }
    clearInputBuffer(); // Clear newline after scanf

    // Add the new student to the array in memory
    if (current_count < MAX_STUDENTS) {
        current_students[current_count] = new_student;
        current_count++;
        saveStudentsToFile(current_students, current_count); // Save the updated list back to file
        printf("\nStudent added successfully!\n");
    } else {
        printf("\nSystem capacity reached. Cannot add more students.\n");
    }
}

// Displays all student records from the system
void displayAllStudents() {
    clearScreen();
    printf("--- All Student Records ---\n");

    struct Student students[MAX_STUDENTS];
    int count = loadStudentsFromFile(students); // Load all students into memory

    if (count == 0) {
        printf("\nNo student records found. The system is empty.\n");
        return;
    }

    // Print table header
    printf("------------------------------------------------------------------\n");
    printf("%-10s %-*s %-10s\n", "Roll No", MAX_NAME_LENGTH, "Name", "Marks");
    printf("------------------------------------------------------------------\n");

    // Print each student's details
    for (int i = 0; i < count; i++) {
        printf("%-10d %-*s %-10.2f\n",
               students[i].roll_no, MAX_NAME_LENGTH, students[i].name, students[i].marks);
    }
    printf("------------------------------------------------------------------\n");
}

// Searches for and displays a specific student record by roll number
void searchStudent() {
    clearScreen();
    printf("--- Search Student ---\n");
    int search_roll;
    bool found = false;

    // Get the roll number to search for
    printf("Enter Roll Number to search: ");
    while (scanf("%d", &search_roll) != 1 || search_roll <= 0) {
        printf("Invalid Roll Number. Please enter a positive integer: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    struct Student students[MAX_STUDENTS];
    int count = loadStudentsFromFile(students); // Load all students into memory

    // Iterate through students to find a match
    for (int i = 0; i < count; i++) {
        if (students[i].roll_no == search_roll) {
            printf("\nStudent Found:\n");
            printf("------------------------------------------------------------------\n");
            printf("%-10s %-*s %-10s\n", "Roll No", MAX_NAME_LENGTH, "Name", "Marks");
            printf("------------------------------------------------------------------\n");
            printf("%-10d %-*s %-10.2f\n",
                   students[i].roll_no, MAX_NAME_LENGTH, students[i].name, students[i].marks);
            printf("------------------------------------------------------------------\n");
            found = true;
            break; // Exit loop once student is found
        }
    }

    if (!found) {
        printf("\nStudent with Roll Number %d not found.\n", search_roll);
    }
}

// Updates an existing student record
void updateStudent() {
    clearScreen();
    printf("--- Update Student Record ---\n");
    int update_roll;
    bool found = false;

    // Get the roll number of the student to update
    printf("Enter Roll Number of student to update: ");
    while (scanf("%d", &update_roll) != 1 || update_roll <= 0) {
        printf("Invalid Roll Number. Please enter a positive integer: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    struct Student students[MAX_STUDENTS];
    int count = loadStudentsFromFile(students); // Load all students into memory

    // Find the student to update
    for (int i = 0; i < count; i++) {
        if (students[i].roll_no == update_roll) {
            found = true;
            printf("\nStudent found. Enter new details:\n");

            // Update Name
            printf("Current Name: %s\n", students[i].name);
            printf("Enter New Name (max %d characters): ", MAX_NAME_LENGTH - 1);
            if (fgets(students[i].name, MAX_NAME_LENGTH, stdin) != NULL) {
                students[i].name[strcspn(students[i].name, "\n")] = 0;
            } else {
                printf("Error reading new name.\n");
                return;
            }
            if (strlen(students[i].name) == 0) {
                printf("Name cannot be empty. Update cancelled for name.\n");
                // Optionally keep old name or force re-entry
            }

            // Update Marks
            printf("Current Marks: %.2f\n", students[i].marks);
            printf("Enter New Marks (0.0 to 100.0): ");
            while (scanf("%f", &students[i].marks) != 1 || students[i].marks < 0.0 || students[i].marks > 100.0) {
                printf("Invalid Marks. Please enter a value between 0.0 and 100.0: ");
                clearInputBuffer();
            }
            clearInputBuffer();

            // Save the entire updated array back to the file
            saveStudentsToFile(students, count);
            printf("\nStudent record updated successfully!\n");
            break; // Exit loop after updating
        }
    }

    if (!found) {
        printf("\nStudent with Roll Number %d not found for update.\n", update_roll);
    }
}

// Deletes a student record from the system
void deleteStudent() {
    clearScreen();
    printf("--- Delete Student Record ---\n");
    int delete_roll;
    bool found = false;
    int delete_index = -1; // To store the index of the student to be deleted

    // Get the roll number of the student to delete
    printf("Enter Roll Number of student to delete: ");
    while (scanf("%d", &delete_roll) != 1 || delete_roll <= 0) {
        printf("Invalid Roll Number. Please enter a positive integer: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    struct Student students[MAX_STUDENTS];
    int count = loadStudentsFromFile(students); // Load all students into memory

    // Find the student to delete and store their index
    for (int i = 0; i < count; i++) {
        if (students[i].roll_no == delete_roll) {
            found = true;
            delete_index = i;
            break;
        }
    }

    if (!found) {
        printf("\nStudent with Roll Number %d not found for deletion.\n", delete_roll);
        return;
    }

    // Shift elements in the array to "overwrite" the deleted student's data
    // This effectively removes the student from the in-memory array
    for (int i = delete_index; i < count - 1; i++) {
        students[i] = students[i+1];
    }
    count--; // Decrement the total count of students

    // Save the modified (smaller) array back to the file, overwriting the old content
    saveStudentsToFile(students, count);
    printf("\nStudent with Roll Number %d deleted successfully!\n", delete_roll);
}