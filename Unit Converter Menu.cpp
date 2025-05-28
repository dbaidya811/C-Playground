#include <stdio.h>
#include <stdlib.h> // For system("clear || cls")
#include <ctype.h>  // For toupper

// Function prototypes for main categories
void temperatureConverter();
void lengthConverter();
void weightConverter();

// Utility function prototypes
void clearInputBuffer();
double getValidDoubleInput(const char* prompt);

// --- Conversion Factor Constants ---
// Length
const double METERS_TO_FEET = 3.28084;
const double KILOMETERS_TO_MILES = 0.621371;
const double METERS_TO_INCHES = 39.3701;

// Weight/Mass
const double KILOGRAMS_TO_POUNDS = 2.20462;
const double GRAMS_TO_OUNCES = 0.035274;


int main() {
    int choice;

    do {
        // Clear screen for better readability
        system("clear || cls"); // "clear" for Linux/macOS, "cls" for Windows

        printf("-------------------------------------\n");
        printf("        Unit Converter Menu          \n");
        printf("-------------------------------------\n");
        printf("1. Temperature Converter\n");
        printf("2. Length Converter\n");
        printf("3. Weight/Mass Converter\n");
        printf("4. Exit\n");
        printf("-------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            clearInputBuffer(); // Clear the faulty input
            choice = 0; // Reset choice to loop again
        } else {
            clearInputBuffer(); // Clear the newline character
        }

        switch (choice) {
            case 1:
                temperatureConverter();
                break;
            case 2:
                lengthConverter();
                break;
            case 3:
                weightConverter();
                break;
            case 4:
                printf("\nExiting Unit Converter. Goodbye!\n");
                break;
            default:
                if (choice !=0) { // Avoid double message if scanf failed
                   printf("Invalid choice. Please select an option from 1 to 4.\n");
                }
        }

        if (choice != 4 && choice != 0) {
            printf("\nPress Enter to return to the main menu...");
            getchar(); // Wait for user to press Enter
        }

    } while (choice != 4);

    return 0;
}

// Utility function to clear the input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Utility function to get a valid double input
double getValidDoubleInput(const char* prompt) {
    double value;
    printf("%s", prompt);
    while (scanf("%lf", &value) != 1) {
        printf("Invalid input. Please enter a numeric value: ");
        clearInputBuffer();
    }
    clearInputBuffer(); // Clear the newline
    return value;
}

// --- Temperature Converter ---
void temperatureConverter() {
    int choice;
    double value, result;
    system("clear || cls");
    printf("-------------------------------------\n");
    printf("      Temperature Converter        \n");
    printf("-------------------------------------\n");
    printf("1. Celsius to Fahrenheit\n");
    printf("2. Fahrenheit to Celsius\n");
    printf("3. Celsius to Kelvin\n");
    printf("4. Kelvin to Celsius\n");
    printf("5. Back to Main Menu\n");
    printf("-------------------------------------\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a number between 1 and 5.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (choice == 5) return;
    if (choice < 1 || choice > 4) {
        printf("Invalid choice.\n");
        return;
    }

    value = getValidDoubleInput("Enter the temperature value: ");

    switch (choice) {
        case 1: // C to F
            result = (value * 9.0 / 5.0) + 32.0;
            printf("%.2f Celsius is %.2f Fahrenheit\n", value, result);
            break;
        case 2: // F to C
            result = (value - 32.0) * 5.0 / 9.0;
            printf("%.2f Fahrenheit is %.2f Celsius\n", value, result);
            break;
        case 3: // C to K
            result = value + 273.15;
            printf("%.2f Celsius is %.2f Kelvin\n", value, result);
            break;
        case 4: // K to C
            result = value - 273.15;
            printf("%.2f Kelvin is %.2f Celsius\n", value, result);
            break;
    }
}

// --- Length Converter ---
void lengthConverter() {
    int choice;
    double value, result;
    system("clear || cls");
    printf("-------------------------------------\n");
    printf("         Length Converter            \n");
    printf("-------------------------------------\n");
    printf("1. Meters to Feet\n");
    printf("2. Feet to Meters\n");
    printf("3. Kilometers to Miles\n");
    printf("4. Miles to Kilometers\n");
    printf("5. Meters to Inches\n");
    printf("6. Inches to Meters\n");
    printf("7. Back to Main Menu\n");
    printf("-------------------------------------\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a number between 1 and 7.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (choice == 7) return;
    if (choice < 1 || choice > 6) {
        printf("Invalid choice.\n");
        return;
    }

    value = getValidDoubleInput("Enter the length value: ");

    switch (choice) {
        case 1: // Meters to Feet
            result = value * METERS_TO_FEET;
            printf("%.2f Meters is %.2f Feet\n", value, result);
            break;
        case 2: // Feet to Meters
            result = value / METERS_TO_FEET;
            printf("%.2f Feet is %.2f Meters\n", value, result);
            break;
        case 3: // Kilometers to Miles
            result = value * KILOMETERS_TO_MILES;
            printf("%.2f Kilometers is %.2f Miles\n", value, result);
            break;
        case 4: // Miles to Kilometers
            result = value / KILOMETERS_TO_MILES;
            printf("%.2f Miles is %.2f Kilometers\n", value, result);
            break;
        case 5: // Meters to Inches
            result = value * METERS_TO_INCHES;
            printf("%.2f Meters is %.2f Inches\n", value, result);
            break;
        case 6: // Inches to Meters
            result = value / METERS_TO_INCHES;
            printf("%.2f Inches is %.2f Meters\n", value, result);
            break;
    }
}

// --- Weight/Mass Converter ---
void weightConverter() {
    int choice;
    double value, result;
    system("clear || cls");
    printf("-------------------------------------\n");
    printf("      Weight/Mass Converter        \n");
    printf("-------------------------------------\n");
    printf("1. Kilograms to Pounds\n");
    printf("2. Pounds to Kilograms\n");
    printf("3. Grams to Ounces\n");
    printf("4. Ounces to Grams\n");
    printf("5. Back to Main Menu\n");
    printf("-------------------------------------\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a number between 1 and 5.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (choice == 5) return;
    if (choice < 1 || choice > 4) {
        printf("Invalid choice.\n");
        return;
    }

    value = getValidDoubleInput("Enter the weight/mass value: ");

    switch (choice) {
        case 1: // Kilograms to Pounds
            result = value * KILOGRAMS_TO_POUNDS;
            printf("%.2f Kilograms is %.2f Pounds\n", value, result);
            break;
        case 2: // Pounds to Kilograms
            result = value / KILOGRAMS_TO_POUNDS;
            printf("%.2f Pounds is %.2f Kilograms\n", value, result);
            break;
        case 3: // Grams to Ounces
            result = value * GRAMS_TO_OUNCES;
            printf("%.2f Grams is %.2f Ounces\n", value, result);
            break;
        case 4: // Ounces to Grams
            result = value / GRAMS_TO_OUNCES;
            printf("%.2f Ounces is %.2f Grams\n", value, result);
            break;
    }
}