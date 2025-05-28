#include <stdio.h>
#include <stdlib.h> // Required for rand() and srand()
#include <time.h>   // Required for time()

int main() {
    int randomNumber;
    int userGuess;
    int numberOfGuesses = 0;
    int lowerBound = 1;
    int upperBound = 100;
    char playAgain;

    // Seed the random number generator
    // This should only be done once per program run
    srand(time(NULL));

    printf("Welcome to the Number Guessing Game!\n");

    do {
        // Generate a random number between lowerBound and upperBound (inclusive)
        randomNumber = (rand() % (upperBound - lowerBound + 1)) + lowerBound;
        numberOfGuesses = 0; // Reset guess count for a new game

        printf("\nI have selected a number between %d and %d.\n", lowerBound, upperBound);
        printf("Can you guess what it is?\n");

        do {
            printf("Enter your guess: ");
            // Check if scanf successfully read an integer
            if (scanf("%d", &userGuess) != 1) {
                printf("Invalid input. Please enter a number.\n");
                // Clear the input buffer
                while (getchar() != '\n'); // Read and discard characters until newline
                continue; // Skip the rest of the loop and ask for input again
            }

            numberOfGuesses++;

            if (userGuess < lowerBound || userGuess > upperBound) {
                printf("Your guess is out of the valid range (%d-%d). Please try again.\n", lowerBound, upperBound);
            } else if (userGuess < randomNumber) {
                printf("Too low! Try again.\n");
            } else if (userGuess > randomNumber) {
                printf("Too high! Try again.\n");
            } else {
                printf("\nCongratulations! You guessed the number %d correctly!\n", randomNumber);
                printf("It took you %d guesses.\n", numberOfGuesses);
            }

        } while (userGuess != randomNumber);

        // Ask if the user wants to play again
        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &playAgain); // Note the space before %c to consume any leftover newline character

    } while (playAgain == 'y' || playAgain == 'Y');

    printf("\nThanks for playing! Goodbye.\n");

    return 0;
}