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
#define EMP_FILENAME "employees.dat"
#define EMP_MAX_NAME_LENGTH 100
#define EMP_MAX_DEPT_LENGTH 50
#define EMP_MAX_DATE_LENGTH 15 // e.g., "YYYY-MM-DD"
#define EMP_MAX_EMPLOYEES 1000

// --- Structure Definition ---
struct Employee {
    int employee_id;
    char name[EMP_MAX_NAME_LENGTH];
    char department[EMP_MAX_DEPT_LENGTH];
    float salary;
    char hire_date[EMP_MAX_DATE_LENGTH]; // Stored as YYYY-MM-DD string
};

// --- Function Prototypes ---
// Utility
void emp_clearScreen();
void emp_clearInputBuffer();
void emp_pressEnterToContinue();
void emp_displayMenu();

// CRUD operations
void emp_addEmployee();
void emp_displayAllEmployees();
void emp_searchEmployee();
void emp_updateEmployee();
void emp_deleteEmployee();

// File I/O helpers
int emp_loadEmployees(struct Employee emp_array[]);
void emp_saveEmployees(struct Employee emp_array[], int count);

// --- Main Function for Employee System ---
int main() {
    int choice;
    do {
        emp_clearScreen();
        emp_displayMenu();
        printf("Enter your choice: ");
        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 5) {
            printf("Invalid choice. Please enter a number between 0 and 5: ");
            emp_clearInputBuffer();
        }
        emp_clearInputBuffer();

        switch (choice) {
            case 1: emp_addEmployee(); break;
            case 2: emp_displayAllEmployees(); break;
            case 3: emp_searchEmployee(); break;
            case 4: emp_updateEmployee(); break;
            case 5: emp_deleteEmployee(); break;
            case 0: printf("\nExiting Employee Management System. Goodbye!\n"); break;
            default: printf("\nAn unexpected error occurred.\n"); break;
        }
        if (choice != 0) {
            emp_pressEnterToContinue();
        }
    } while (choice != 0);

    return 0;
}

// --- Utility Functions Implementation ---
void emp_clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void emp_clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void emp_pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    emp_clearInputBuffer();
    getchar();
}

void emp_displayMenu() {
    printf("=======================================\n");
    printf("  EMPLOYEE MANAGEMENT SYSTEM\n");
    printf("=======================================\n");
    printf("1. Add New Employee\n");
    printf("2. Display All Employees\n");
    printf("3. Search Employee\n");
    printf("4. Update Employee Record\n");
    printf("5. Delete Employee Record\n");
    printf("0. Exit\n");
    printf("---------------------------------------\n");
}

// --- File I/O Helper Functions Implementation ---
int emp_loadEmployees(struct Employee emp_array[]) {
    FILE *fp = fopen(EMP_FILENAME, "rb");
    if (fp == NULL) return 0;

    int count = 0;
    while (count < EMP_MAX_EMPLOYEES && fread(&emp_array[count], sizeof(struct Employee), 1, fp) == 1) {
        count++;
    }
    fclose(fp);
    return count;
}

void emp_saveEmployees(struct Employee emp_array[], int count) {
    FILE *fp = fopen(EMP_FILENAME, "wb");
    if (fp == NULL) {
        perror("Error opening file for saving employees");
        return;
    }
    fwrite(emp_array, sizeof(struct Employee), count, fp);
    fclose(fp);
}

// --- CRUD Operations Implementation ---
void emp_addEmployee() {
    emp_clearScreen();
    printf("--- Add New Employee ---\n");
    struct Employee new_emp;
    struct Employee existing_employees[EMP_MAX_EMPLOYEES];
    int current_count = emp_loadEmployees(existing_employees);

    printf("Enter Employee ID: ");
    while (scanf("%d", &new_emp.employee_id) != 1 || new_emp.employee_id <= 0) {
        printf("Invalid ID. Enter a positive integer: ");
        emp_clearInputBuffer();
    }
    emp_clearInputBuffer();

    for (int i = 0; i < current_count; i++) {
        if (existing_employees[i].employee_id == new_emp.employee_id) {
            printf("Error: Employee with ID %d already exists.\n", new_emp.employee_id);
            return;
        }
    }

    printf("Enter Name (max %d chars): ", EMP_MAX_NAME_LENGTH - 1);
    fgets(new_emp.name, EMP_MAX_NAME_LENGTH, stdin);
    new_emp.name[strcspn(new_emp.name, "\n")] = 0;

    printf("Enter Department (max %d chars): ", EMP_MAX_DEPT_LENGTH - 1);
    fgets(new_emp.department, EMP_MAX_DEPT_LENGTH, stdin);
    new_emp.department[strcspn(new_emp.department, "\n")] = 0;

    printf("Enter Salary (e.g., 50000.00): ");
    while (scanf("%f", &new_emp.salary) != 1 || new_emp.salary < 0.0) {
        printf("Invalid Salary. Enter 0 or a positive number: ");
        emp_clearInputBuffer();
    }
    emp_clearInputBuffer();

    printf("Enter Hire Date (YYYY-MM-DD): ");
    fgets(new_emp.hire_date, EMP_MAX_DATE_LENGTH, stdin);
    new_emp.hire_date[strcspn(new_emp.hire_date, "\n")] = 0;

    if (current_count < EMP_MAX_EMPLOYEES) {
        existing_employees[current_count] = new_emp;
        current_count++;
        emp_saveEmployees(existing_employees, current_count);
        printf("\nEmployee added successfully!\n");
    } else {
        printf("\nSystem capacity reached. Cannot add more employees.\n");
    }
}

void emp_displayAllEmployees() {
    emp_clearScreen();
    printf("--- All Employees ---\n");
    struct Employee employees[EMP_MAX_EMPLOYEES];
    int count = emp_loadEmployees(employees);

    if (count == 0) {
        printf("\nNo employee records found.\n");
        return;
    }

    printf("---------------------------------------------------------------------------------------------------\n");
    printf("%-8s %-*s %-*s %-15s %-12s\n", "ID", EMP_MAX_NAME_LENGTH, "Name", EMP_MAX_DEPT_LENGTH, "Department", "Salary", "Hire Date");
    printf("---------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-8d %-*s %-*s %-15.2f %-12s\n",
               employees[i].employee_id,
               EMP_MAX_NAME_LENGTH, employees[i].name,
               EMP_MAX_DEPT_LENGTH, employees[i].department,
               employees[i].salary, employees[i].hire_date);
    }
    printf("---------------------------------------------------------------------------------------------------\n");
}

void emp_searchEmployee() {
    emp_clearScreen();
    printf("--- Search Employee ---\n");
    int choice;
    bool found = false;

    printf("Search by:\n1. Employee ID\n2. Name\nEnter choice: ");
    while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Invalid choice. Enter 1 or 2: ");
        emp_clearInputBuffer();
    }
    emp_clearInputBuffer();

    struct Employee employees[EMP_MAX_EMPLOYEES];
    int count = emp_loadEmployees(employees);

    if (choice == 1) {
        int search_id;
        printf("Enter Employee ID to search: ");
        while (scanf("%d", &search_id) != 1 || search_id <= 0) {
            printf("Invalid ID. Enter a positive integer: ");
            emp_clearInputBuffer();
        }
        emp_clearInputBuffer();
        for (int i = 0; i < count; i++) {
            if (employees[i].employee_id == search_id) {
                printf("\nEmployee Found:\n");
                printf("ID: %d\nName: %s\nDepartment: %s\nSalary: %.2f\nHire Date: %s\n",
                       employees[i].employee_id, employees[i].name, employees[i].department,
                       employees[i].salary, employees[i].hire_date);
                found = true;
                break;
            }
        }
    } else { // Search by Name
        char search_name[EMP_MAX_NAME_LENGTH];
        printf("Enter Name to search (partial or full): ");
        fgets(search_name, EMP_MAX_NAME_LENGTH, stdin);
        search_name[strcspn(search_name, "\n")] = 0;
        for (int i = 0; i < count; i++) {
            // Case-insensitive search
            if (strstr(employees[i].name, search_name) != NULL) {
                printf("\nEmployee Found:\n");
                printf("ID: %d\nName: %s\nDepartment: %s\nSalary: %.2f\nHire Date: %s\n",
                       employees[i].employee_id, employees[i].name, employees[i].department,
                       employees[i].salary, employees[i].hire_date);
                found = true;
                // Don't break, show all matches
            }
        }
    }

    if (!found) {
        printf("\nEmployee not found.\n");
    }
}

void emp_updateEmployee() {
    emp_clearScreen();
    printf("--- Update Employee Record ---\n");
    int update_id;
    bool found = false;

    printf("Enter Employee ID to update: ");
    while (scanf("%d", &update_id) != 1 || update_id <= 0) {
        printf("Invalid ID. Enter a positive integer: ");
        emp_clearInputBuffer();
    }
    emp_clearInputBuffer();

    struct Employee employees[EMP_MAX_EMPLOYEES];
    int count = emp_loadEmployees(employees);

    for (int i = 0; i < count; i++) {
        if (employees[i].employee_id == update_id) {
            found = true;
            printf("\nEmployee found. Enter new details:\n");

            printf("Current Name: %s\n", employees[i].name);
            printf("Enter New Name (max %d chars): ", EMP_MAX_NAME_LENGTH - 1);
            fgets(employees[i].name, EMP_MAX_NAME_LENGTH, stdin);
            employees[i].name[strcspn(employees[i].name, "\n")] = 0;

            printf("Current Department: %s\n", employees[i].department);
            printf("Enter New Department (max %d chars): ", EMP_MAX_DEPT_LENGTH - 1);
            fgets(employees[i].department, EMP_MAX_DEPT_LENGTH, stdin);
            employees[i].department[strcspn(employees[i].department, "\n")] = 0;

            printf("Current Salary: %.2f\n", employees[i].salary);
            printf("Enter New Salary (0.0 or more): ");
            while (scanf("%f", &employees[i].salary) != 1 || employees[i].salary < 0.0) {
                printf("Invalid Salary. Enter 0 or a positive number: ");
                emp_clearInputBuffer();
            }
            emp_clearInputBuffer();

            printf("Current Hire Date: %s\n", employees[i].hire_date);
            printf("Enter New Hire Date (YYYY-MM-DD): ");
            fgets(employees[i].hire_date, EMP_MAX_DATE_LENGTH, stdin);
            employees[i].hire_date[strcspn(employees[i].hire_date, "\n")] = 0;

            emp_saveEmployees(employees, count);
            printf("\nEmployee record updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("\nEmployee with ID %d not found for update.\n", update_id);
    }
}

void emp_deleteEmployee() {
    emp_clearScreen();
    printf("--- Delete Employee Record ---\n");
    int delete_id;
    bool found = false;
    int delete_index = -1;

    printf("Enter Employee ID to delete: ");
    while (scanf("%d", &delete_id) != 1 || delete_id <= 0) {
        printf("Invalid ID. Enter a positive integer: ");
        emp_clearInputBuffer();
    }
    emp_clearInputBuffer();

    struct Employee employees[EMP_MAX_EMPLOYEES];
    int count = emp_loadEmployees(employees);

    for (int i = 0; i < count; i++) {
        if (employees[i].employee_id == delete_id) {
            found = true;
            delete_index = i;
            break;
        }
    }

    if (!found) {
        printf("\nEmployee with ID %d not found for deletion.\n", delete_id);
        return;
    }

    for (int i = delete_index; i < count - 1; i++) {
        employees[i] = employees[i+1];
    }
    count--;

    emp_saveEmployees(employees, count);
    printf("\nEmployee with ID %d deleted successfully!\n", delete_id);
}