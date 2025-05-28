#include <stdio.h>
#include <stdlib.h> // For system("cls") or system("clear")
#include <stdbool.h> // For bool type
#include <string.h> // For strcmp

// Function Prototypes
bool isLeapYear(int year);
int getDaysInMonth(int month, int year);
int getStartingDayOfWeek(int year, int month, int day); // Modified for correct calculation
void printMonthCalendar(int year, int month);
void clearInputBuffer();

// --- Main Function ---
int main() {
    int year, month;
    char choice;

    do {
        // Clear screen at the beginning of each new display
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("\n=== C Calendar Application ===\n");

        printf("Enter year (e.g., 2024): ");
        // Loop for robust integer input
        while (scanf("%d", &year) != 1 || year < 1) {
            printf("Invalid year. Please enter a positive integer: ");
            clearInputBuffer();
        }
        clearInputBuffer(); // Clear any remaining newline

        printf("Enter month (1-12): ");
        // Loop for robust integer input
        while (scanf("%d", &month) != 1 || month < 1 || month > 12) {
            printf("Invalid month. Please enter a number between 1 and 12: ");
            clearInputBuffer();
        }
        clearInputBuffer(); // Clear any remaining newline

        printMonthCalendar(year, month);

        printf("\nDo you want to view another month? (y/n): ");
        scanf(" %c", &choice); // Space before %c to consume whitespace
        if (choice >= 'A' && choice <= 'Z') {
            choice += 32; // Convert to lowercase
        }
        clearInputBuffer(); // Clear buffer after reading choice

    } while (choice == 'y');

    printf("\nThank you for using the Calendar Application!\n");
    return 0;
}

// --- Date Calculation Functions ---

// Checks if a given year is a leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Returns the number of days in a given month of a given year
int getDaysInMonth(int month, int year) {
    switch (month) {
        case 1:  // January
        case 3:  // March
        case 5:  // May
        case 7:  // July
        case 8:  // August
        case 10: // October
        case 12: // December
            return 31;
        case 4:  // April
        case 6:  // June
        case 9:  // September
        case 11: // November
            return 30;
        case 2:  // February
            return isLeapYear(year) ? 29 : 28;
        default:
            return 0; // Invalid month
    }
}

// Calculates the day of the week for a given date using Zeller's Congruence
// Returns 0 for Sunday, 1 for Monday, ..., 6 for Saturday (our desired mapping)
int getStartingDayOfWeek(int year, int month, int day) {
    // Zeller's Congruence considers January and February as months 13 and 14 of the previous year
    // So, if month is 1 (Jan) or 2 (Feb), adjust it and decrement year
    if (month < 3) {
        month += 12;
        year--;
    }

    int k = day;       // Day of the month
    int m = month;     // Month (adjusted)
    int j = year / 100; // Century (J)
    int y = year % 100; // Year of the century (K in Zeller's original formula, here called y)

    // Zeller's Congruence Formula (with adjusted m for Jan/Feb):
    // h = (q + floor(13*(m+1)/5) + K + floor(K/4) + floor(J/4) - 2*J) mod 7
    // Using integer division for floor and our variable names (k, m, y, j)
    int h = (k + (13 * (m + 1) / 5) + y + (y / 4) + (j / 4) - (2 * j));

    // Zeller's formula result 'h' typically gives:
    // 0 = Saturday, 1 = Sunday, 2 = Monday, 3 = Tuesday, 4 = Wednesday, 5 = Thursday, 6 = Friday

    // We want to map it to:
    // 0 = Sunday, 1 = Monday, 2 = Tuesday, 3 = Wednesday, 4 = Thursday, 5 = Friday, 6 = Saturday

    // To convert Zeller's (0=Sat, 1=Sun, ..., 6=Fri) to our (0=Sun, 1=Mon, ..., 6=Sat):
    // Add 6 (or -1) and take modulo 7. Adding 6 is safer with modulo for potential negative results.
    return ((h % 7 + 7) % 7 + 6) % 7; // This adjustment ensures Sunday is 0.
}

// --- Calendar Printing Function ---

// Prints the calendar for a specified month and year
void printMonthCalendar(int year, int month) {
    char *monthNames[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    int daysInMonth = getDaysInMonth(month, year);
    // Get the day of the week for the 1st day of the month
    int startingDay = getStartingDayOfWeek(year, month, 1);

    printf("\n\n");
    printf("        %s %d\n", monthNames[month - 1], year);
    printf("-----------------------------\n");
    printf(" Sun Mon Tue Wed Thu Fri Sat\n");
    printf("-----------------------------\n");

    // Print leading spaces for the first week
    for (int i = 0; i < startingDay; i++) {
        printf("    "); // 4 spaces for each day slot
    }

    // Print days of the month
    for (int day = 1; day <= daysInMonth; day++) {
        printf("%4d", day); // Print day with 4 spaces (right-aligned)

        // If it's Saturday (our 6th day of the week 0-6), or end of month, move to next line
        // (startingDay + day) gives the index in the week after accounting for leading spaces
        if ((startingDay + day) % 7 == 0 || day == daysInMonth) {
            printf("\n");
        }
    }
    printf("-----------------------------\n");
}

// --- Utility Functions ---

// Clears the input buffer to prevent issues with scanf
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}