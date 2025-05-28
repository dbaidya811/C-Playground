#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For string functions like strcpy, strlen
#include <ctype.h>  // For toupper

#define MAX_TASKS 100       // Maximum number of tasks
#define MAX_TASK_LENGTH 256 // Maximum length of a task description

// Structure to represent a single task
struct Task {
    char description[MAX_TASK_LENGTH];
    int isCompleted; // 0 for not completed, 1 for completed
};

// Array to store all tasks
struct Task tasks[MAX_TASKS];
int taskCount = 0; // Current number of tasks

// Function prototypes
void addTask();
void viewTasks();
void markTaskAsCompleted();
void deleteTask();
void displayMenu();
void clearInputBuffer(); // Utility function to clear the input buffer

int main() {
    int choice;

    printf("-------------------------------------\n");
    printf("   Welcome to Your To-Do List App   \n");
    printf("-------------------------------------\n");

    do {
        displayMenu();
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer(); // Clear the faulty input
            choice = -1; // Set to an invalid choice to re-prompt
            continue;
        }
        clearInputBuffer(); // Clear the newline character left by scanf

        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                markTaskAsCompleted();
                break;
            case 4:
                deleteTask();
                break;
            case 5:
                printf("\nExiting To-Do List. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        if (choice != 5) {
             printf("\nPress Enter to continue...");
             getchar(); // Wait for user to press Enter
        }
        system("clear || cls"); // Clears the console (works on Linux/macOS and Windows)

    } while (choice != 5);

    return 0;
}

// Utility function to clear the input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to display the menu
void displayMenu() {
    printf("\nTo-Do List Menu:\n");
    printf("1. Add Task\n");
    printf("2. View Tasks\n");
    printf("3. Mark Task as Completed\n");
    printf("4. Delete Task\n");
    printf("5. Exit\n");
}

// Function to add a new task
void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("To-Do list is full. Cannot add more tasks.\n");
        return;
    }

    printf("\nEnter task description: ");
    // Use fgets to read the whole line, including spaces
    if (fgets(tasks[taskCount].description, MAX_TASK_LENGTH, stdin) != NULL) {
        // Remove trailing newline character if present
        size_t len = strlen(tasks[taskCount].description);
        if (len > 0 && tasks[taskCount].description[len - 1] == '\n') {
            tasks[taskCount].description[len - 1] = '\0';
        }

        tasks[taskCount].isCompleted = 0; // New tasks are not completed by default
        taskCount++;
        printf("Task added successfully!\n");
    } else {
        printf("Error reading task description.\n");
        // It's good practice to also clear the buffer if fgets fails unexpectedly
        // though with stdin, it's less common for fgets itself to leave things in a bad state
        // unless an EOF was hit.
        if (feof(stdin)) {
            clearerr(stdin); // Clear EOF indicator if that was the cause
        }
    }
}

// Function to view all tasks
void viewTasks() {
    if (taskCount == 0) {
        printf("\nNo tasks in the list.\n");
        return;
    }

    printf("\n--- Your Tasks ---\n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. [%c] %s\n",
               i + 1,
               (tasks[i].isCompleted ? 'X' : ' '), // 'X' for completed, ' ' for pending
               tasks[i].description);
    }
    printf("------------------\n");
}

// Function to mark a task as completed
void markTaskAsCompleted() {
    int taskNumber;
    if (taskCount == 0) {
        printf("\nNo tasks to mark as completed.\n");
        return;
    }

    viewTasks(); // Show tasks so user can choose
    printf("\nEnter the task number to mark as completed: ");

    if (scanf("%d", &taskNumber) != 1) {
        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (taskNumber < 1 || taskNumber > taskCount) {
        printf("Invalid task number.\n");
    } else {
        if (tasks[taskNumber - 1].isCompleted) {
            printf("Task #%d is already marked as completed.\n", taskNumber);
        } else {
            tasks[taskNumber - 1].isCompleted = 1;
            printf("Task #%d marked as completed!\n", taskNumber);
        }
    }
}

// Function to delete a task
void deleteTask() {
    int taskNumber;
    if (taskCount == 0) {
        printf("\nNo tasks to delete.\n");
        return;
    }

    viewTasks(); // Show tasks so user can choose
    printf("\nEnter the task number to delete: ");

    if (scanf("%d", &taskNumber) != 1) {
        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (taskNumber < 1 || taskNumber > taskCount) {
        printf("Invalid task number.\n");
    } else {
        // To delete, shift all subsequent tasks one position up
        for (int i = taskNumber - 1; i < taskCount - 1; i++) {
            strcpy(tasks[i].description, tasks[i + 1].description);
            tasks[i].isCompleted = tasks[i + 1].isCompleted;
        }
        taskCount--; // Decrease the total task count
        printf("Task #%d deleted successfully!\n", taskNumber);
    }
}