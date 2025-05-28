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
#define BANK_FILENAME "bank_accounts.dat"
#define BANK_MAX_NAME_LENGTH 100
#define BANK_MAX_TYPE_LENGTH 20 // e.g., "Savings", "Current"
#define BANK_MAX_ACCOUNTS 1000

// --- Structure Definition ---
struct Account {
    long account_number; // Using long for potentially larger numbers
    char account_holder_name[BANK_MAX_NAME_LENGTH];
    float balance;
    char account_type[BANK_MAX_TYPE_LENGTH];
};

// --- Function Prototypes ---
// Utility
void bank_clearScreen();
void bank_clearInputBuffer();
void bank_pressEnterToContinue();
void bank_displayMenu();

// CRUD operations
void bank_createAccount();
void bank_displayAllAccounts();
void bank_searchAccount();
void bank_depositWithdraw(bool is_deposit); // true for deposit, false for withdraw
void bank_deleteAccount();

// File I/O helpers
int bank_loadAccounts(struct Account account_array[]);
void bank_saveAccounts(struct Account account_array[], int count);

// --- Main Function for Bank System ---
int main() {
    int choice;
    do {
        bank_clearScreen();
        bank_displayMenu();
        printf("Enter your choice: ");
        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 6) {
            printf("Invalid choice. Please enter a number between 0 and 6: ");
            bank_clearInputBuffer(); // Corrected this line
        }
        bank_clearInputBuffer(); // Corrected this line as well to ensure it's the bank version

        switch (choice) {
            case 1: bank_createAccount(); break;
            case 2: bank_displayAllAccounts(); break;
            case 3: bank_searchAccount(); break;
            case 4: bank_depositWithdraw(true); break; // Deposit
            case 5: bank_depositWithdraw(false); break; // Withdraw
            case 6: bank_deleteAccount(); break;
            case 0: printf("\nExiting Bank Management System. Goodbye!\n"); break;
            default: printf("\nAn unexpected error occurred.\n"); break;
        }
        if (choice != 0) {
            bank_pressEnterToContinue();
        }
    } while (choice != 0);

    return 0;
}

// --- Utility Functions Implementation ---
void bank_clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void bank_clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void bank_pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    bank_clearInputBuffer();
    getchar();
}

void bank_displayMenu() {
    printf("=======================================\n");
    printf("  BANK MANAGEMENT SYSTEM\n");
    printf("=======================================\n");
    printf("1. Create New Account\n");
    printf("2. Display All Accounts\n");
    printf("3. Search Account\n");
    printf("4. Deposit Money\n");
    printf("5. Withdraw Money\n");
    printf("6. Delete Account\n");
    printf("0. Exit\n");
    printf("---------------------------------------\n");
}

// --- File I/O Helper Functions Implementation ---
int bank_loadAccounts(struct Account account_array[]) {
    FILE *fp = fopen(BANK_FILENAME, "rb");
    if (fp == NULL) return 0;

    int count = 0;
    while (count < BANK_MAX_ACCOUNTS && fread(&account_array[count], sizeof(struct Account), 1, fp) == 1) {
        count++;
    }
    fclose(fp);
    return count;
}

void bank_saveAccounts(struct Account account_array[], int count) {
    FILE *fp = fopen(BANK_FILENAME, "wb");
    if (fp == NULL) {
        perror("Error opening file for saving accounts");
        return;
    }
    fwrite(account_array, sizeof(struct Account), count, fp);
    fclose(fp);
}

// --- CRUD Operations Implementation ---
void bank_createAccount() {
    bank_clearScreen();
    printf("--- Create New Account ---\n");
    struct Account new_acc;
    struct Account existing_accounts[BANK_MAX_ACCOUNTS];
    int current_count = bank_loadAccounts(existing_accounts);

    printf("Enter Account Number (e.g., 1001): ");
    while (scanf("%ld", &new_acc.account_number) != 1 || new_acc.account_number <= 0) {
        printf("Invalid Account Number. Enter a positive number: ");
        bank_clearInputBuffer();
    }
    bank_clearInputBuffer();

    for (int i = 0; i < current_count; i++) {
        if (existing_accounts[i].account_number == new_acc.account_number) {
            printf("Error: Account Number %ld already exists. Please use a unique Account Number.\n", new_acc.account_number);
            return;
        }
    }

    printf("Enter Account Holder Name (max %d chars): ", BANK_MAX_NAME_LENGTH - 1);
    fgets(new_acc.account_holder_name, BANK_MAX_NAME_LENGTH, stdin);
    new_acc.account_holder_name[strcspn(new_acc.account_holder_name, "\n")] = 0;
    if (strlen(new_acc.account_holder_name) == 0) {
        printf("Account Holder Name cannot be empty. Please try again.\n");
        return;
    }

    printf("Enter Initial Balance (0 or more): ");
    while (scanf("%f", &new_acc.balance) != 1 || new_acc.balance < 0.0) {
        printf("Invalid Balance. Enter 0 or a positive number: ");
        bank_clearInputBuffer();
    }
    bank_clearInputBuffer();

    printf("Enter Account Type (e.g., Savings, Current - max %d chars): ", BANK_MAX_TYPE_LENGTH - 1);
    fgets(new_acc.account_type, BANK_MAX_TYPE_LENGTH, stdin);
    new_acc.account_type[strcspn(new_acc.account_type, "\n")] = 0;
    if (strlen(new_acc.account_type) == 0) {
        printf("Account Type cannot be empty. Please try again.\n");
        return;
    }


    if (current_count < BANK_MAX_ACCOUNTS) {
        existing_accounts[current_count] = new_acc;
        current_count++;
        bank_saveAccounts(existing_accounts, current_count);
        printf("\nAccount created successfully!\n");
    } else {
        printf("\nSystem capacity reached. Cannot create more accounts.\n");
    }
}

void bank_displayAllAccounts() {
    bank_clearScreen();
    printf("--- All Bank Accounts ---\n");
    struct Account accounts[BANK_MAX_ACCOUNTS];
    int count = bank_loadAccounts(accounts);

    if (count == 0) {
        printf("\nNo bank accounts found.\n");
        return;
    }

    printf("--------------------------------------------------------------------------------------\n");
    printf("%-15s %-*s %-15s %-15s\n", "Account No.", BANK_MAX_NAME_LENGTH, "Holder Name", "Balance", "Type");
    printf("--------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-15ld %-*s %-15.2f %-15s\n",
               accounts[i].account_number,
               BANK_MAX_NAME_LENGTH, accounts[i].account_holder_name,
               accounts[i].balance, accounts[i].account_type);
    }
    printf("--------------------------------------------------------------------------------------\n");
}

void bank_searchAccount() {
    bank_clearScreen();
    printf("--- Search Account ---\n");
    long search_acc_num;
    bool found = false;

    printf("Enter Account Number to search: ");
    while (scanf("%ld", &search_acc_num) != 1 || search_acc_num <= 0) {
        printf("Invalid Account Number. Enter a positive number: ");
        bank_clearInputBuffer();
    }
    bank_clearInputBuffer();

    struct Account accounts[BANK_MAX_ACCOUNTS];
    int count = bank_loadAccounts(accounts);

    for (int i = 0; i < count; i++) {
        if (accounts[i].account_number == search_acc_num) {
            printf("\nAccount Found:\n");
            printf("Account Number: %ld\nHolder Name: %s\nBalance: %.2f\nType: %s\n",
                   accounts[i].account_number, accounts[i].account_holder_name,
                   accounts[i].balance, accounts[i].account_type);
            found = true;
            break;
        }
    }

    if (!found) {
        printf("\nAccount with Number %ld not found.\n", search_acc_num);
    }
}

void bank_depositWithdraw(bool is_deposit) {
    bank_clearScreen();
    printf("--- %s Money ---\n", is_deposit ? "Deposit" : "Withdraw");
    long acc_num;
    float amount;
    bool found = false;

    printf("Enter Account Number: ");
    while (scanf("%ld", &acc_num) != 1 || acc_num <= 0) {
        printf("Invalid Account Number. Enter a positive number: ");
        bank_clearInputBuffer();
    }
    bank_clearInputBuffer();

    printf("Enter Amount to %s: ", is_deposit ? "Deposit" : "Withdraw");
    while (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid Amount. Enter a positive number: ");
        bank_clearInputBuffer();
    }
    bank_clearInputBuffer();

    struct Account accounts[BANK_MAX_ACCOUNTS];
    int count = bank_loadAccounts(accounts);

    for (int i = 0; i < count; i++) {
        if (accounts[i].account_number == acc_num) {
            found = true;
            if (is_deposit) {
                accounts[i].balance += amount;
                printf("\nDeposit successful! New balance for %ld: %.2f\n", acc_num, accounts[i].balance);
            } else { // Withdraw
                if (accounts[i].balance >= amount) {
                    accounts[i].balance -= amount;
                    printf("\nWithdrawal successful! New balance for %ld: %.2f\n", acc_num, accounts[i].balance);
                } else {
                    printf("\nInsufficient balance for withdrawal. Current balance: %.2f\n", accounts[i].balance);
                }
            }
            bank_saveAccounts(accounts, count); // Save changes
            break;
        }
    }

    if (!found) {
        printf("\nAccount with Number %ld not found.\n", acc_num);
    }
}

void bank_deleteAccount() {
    bank_clearScreen();
    printf("--- Delete Account ---\n");
    long delete_acc_num;
    bool found = false;
    int delete_index = -1;

    printf("Enter Account Number to delete: ");
    while (scanf("%ld", &delete_acc_num) != 1 || delete_acc_num <= 0) {
        printf("Invalid Account Number. Enter a positive number: ");
        bank_clearInputBuffer();
    }
    bank_clearInputBuffer();

    struct Account accounts[BANK_MAX_ACCOUNTS];
    int count = bank_loadAccounts(accounts);

    for (int i = 0; i < count; i++) {
        if (accounts[i].account_number == delete_acc_num) {
            found = true;
            delete_index = i;
            break;
        }
    }

    if (!found) {
        printf("\nAccount with Number %ld not found.\n", delete_acc_num);
        return;
    }

    for (int i = delete_index; i < count - 1; i++) {
        accounts[i] = accounts[i+1];
    }
    count--;

    bank_saveAccounts(accounts, count);
    printf("\nAccount with Number %ld deleted successfully!\n", delete_acc_num);
}