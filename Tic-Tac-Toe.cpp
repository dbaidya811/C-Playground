#include <stdio.h>    // For standard input/output (printf, scanf)
#include <stdlib.h>   // For system("cls")/system("clear"), rand(), srand()
#include <stdbool.h>  // For boolean type (true/false)
#include <time.h>     // For time() to seed random number generator
#ifdef _WIN32         // Required for Sleep() on Windows
#include <windows.h>
#else                 // Required for usleep() on Linux/macOS
#include <unistd.h>
#endif

// Define symbols for players
#define HUMAN_PLAYER 'X'
#define COMPUTER_PLAYER 'O'
#define EMPTY_CELL ' ' // Use a space for truly empty cells

// Function prototypes
void initializeBoard(char board[3][3]);
void printBoard(char board[3][3]);
int checkWin(char board[3][3]);
bool isValidMove(char board[3][3], int choice);
void makeMove(char board[3][3], char playerSymbol, int choice); // Modified to take symbol directly
void getHumanMove(int *choice); // Simplified, player/symbol passed in main
void getComputerMove(char board[3][3], int *choice, int difficulty);

// Helper functions for AI
bool isBoardFull(char board[3][3]);
int evaluate(char board[3][3]); // For Minimax: scores board state
int minimax(char board[3][3], int depth, bool isMaximizingPlayer); // Minimax algorithm
int findBestMove(char board[3][3]); // Finds best move using minimax

// --- Main Game Logic ---
int main() {
    char board[3][3];
    int player;         // Current player (1 for Human, 2 for Computer)
    int choice;         // Player's chosen box number (1-9)
    int gameStatus;     // -1: Game in progress, 0: Draw, 1: Human wins, 2: Computer wins
    int moves;          // Counts the number of moves made
    int gameMode;       // 1 for Human vs Human, 2 for Human vs Computer
    int aiDifficulty;   // 1: Easy, 2: Medium, 3: Impossible
    char playAgain;     // User's choice to play again (y/n)

    // Seed the random number generator for the computer's moves
    srand(time(NULL));

    do { // Main game loop, continues until user chooses not to play again
        player = 1;     // Reset player to 1 for new game (Human)
        gameStatus = -1; // Reset game status
        moves = 0;      // Reset moves count

        // Clear screen at the beginning of each new game
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printf("\n=== Tic-Tac-Toe ===\n");
        printf("Select game mode:\n");
        printf("1. Human vs Human\n");
        printf("2. Human vs Computer\n");
        printf("Enter your choice (1 or 2): ");
        scanf("%d", &gameMode);

        // Input validation for game mode
        while (gameMode != 1 && gameMode != 2) {
            printf("Invalid choice. Please enter 1 or 2: ");
            while (getchar() != '\n'); // Clear input buffer
            scanf("%d", &gameMode);
        }
        while (getchar() != '\n'); // Clear any remaining newline from scanf

        aiDifficulty = 0; // Reset difficulty
        if (gameMode == 2) {
            printf("\nSelect AI difficulty:\n");
            printf("1. Easy\n");
            printf("2. Medium\n");
            printf("3. Impossible\n");
            printf("Enter your choice (1, 2, or 3): ");
            scanf("%d", &aiDifficulty);
            while (aiDifficulty < 1 || aiDifficulty > 3) {
                printf("Invalid choice. Please enter 1, 2, or 3: ");
                while (getchar() != '\n'); // Clear input buffer
                scanf("%d", &aiDifficulty);
            }
            while (getchar() != '\n'); // Clear any remaining newline from scanf
        }

        initializeBoard(board); // Set up the empty board for the new game

        do { // Game round loop (for a single game)
            // Clear screen before each turn
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif

            printf("\n=== Tic-Tac-Toe ===\n\n");
            printBoard(board);

            player = (moves % 2 == 0) ? 1 : 2; // Player 1 (Human) starts

            if (player == 1) { // Human Player's turn
                printf("\nHuman (X)'s turn.\n");
                getHumanMove(&choice);
                // Validate move before making it
                if (!isValidMove(board, choice)) {
                    printf("Invalid move! Please try again.\n");
                    moves--; // Decrement to give current player another turn
                    // Pause to let the user read the message
                    #ifdef _WIN32
                        system("pause");
                    #else
                        printf("Press Enter to continue...");
                        while(getchar() != '\n'); // Clear input buffer
                        getchar(); // Wait for Enter
                    #endif
                    continue; // Skip to next iteration of game round loop
                }
                makeMove(board, HUMAN_PLAYER, choice);
            } else { // Computer's turn (Player 2)
                if (gameMode == 1) { // Human vs Human (Player 2 is Human)
                    printf("\nPlayer 2 (O)'s turn.\n");
                    getHumanMove(&choice);
                    // Validate move before making it
                    if (!isValidMove(board, choice)) {
                        printf("Invalid move! Please try again.\n");
                        moves--; // Decrement to give current player another turn
                        #ifdef _WIN32
                            system("pause");
                        #else
                            printf("Press Enter to continue...");
                            while(getchar() != '\n'); // Clear input buffer
                            getchar(); // Wait for Enter
                        #endif
                        continue; // Skip to next iteration of game round loop
                    }
                    makeMove(board, COMPUTER_PLAYER, choice);
                } else { // Human vs Computer
                    printf("\nComputer (O)'s turn...\n");
                    getComputerMove(board, &choice, aiDifficulty); // Pass difficulty
                    makeMove(board, COMPUTER_PLAYER, choice);
                }
            }

            moves++;
            gameStatus = checkWin(board); // Check for win after each valid move

        } while (gameStatus == -1 && moves < 9); // Continue as long as game is in progress and board is not full

        // Final board display after game ends
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        printf("\n=== Tic-Tac-Toe - Game Over ===\n\n");
        printBoard(board);

        // Announce result
        if (gameStatus == 1) {
            printf("\nHuman (X) wins!\n");
        } else if (gameStatus == 2) {
            if (gameMode == 1) {
                printf("\nPlayer 2 (O) wins!\n");
            } else {
                printf("\nComputer (O) wins!\n");
            }
        } else if (gameStatus == 0) {
            printf("\nIt's a draw!\n");
        } else {
            printf("\nSomething went wrong with game status.\n"); // Should not happen
        }

        // --- Play Again Option ---
        printf("\nDo you want to play again? (y/n): ");
        while (getchar() != '\n'); // Clear any lingering newline from previous scanf/gets
        scanf(" %c", &playAgain);   // Notice the space before %c to consume whitespace

        // Convert to lowercase for easier comparison
        if (playAgain >= 'A' && playAgain <= 'Z') {
            playAgain += 32; // Convert uppercase to lowercase
        }

    } while (playAgain == 'y'); // Loop back to main game if user enters 'y'

    printf("\nThanks for playing!\n");

    return 0;
}

// --- Board and Game State Functions ---

// Initializes the board with empty spaces
void initializeBoard(char board[3][3]) {
    int i, j, count = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            board[i][j] = (char)(count + '0'); // Show numbers 1-9
            // board[i][j] = EMPTY_CELL; // Alternative: initialize with spaces for cleaner look, but hides numbers
            count++;
        }
    }
}

// Prints the current state of the board
void printBoard(char board[3][3]) {
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n", board[0][0], board[0][1], board[0][2]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n", board[1][0], board[1][1], board[1][2]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n", board[2][0], board[2][1], board[2][2]);
    printf("     |     |     \n");
}

// Checks if there's a winner
// Returns 1 if Human (X) wins
// Returns 2 if Computer (O) wins
// Returns 0 if Draw
// Returns -1 if game is still in progress
int checkWin(char board[3][3]) {
    int i;

    // Check rows and columns
    for (i = 0; i < 3; i++) {
        // Rows
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == HUMAN_PLAYER) return 1;
            if (board[i][0] == COMPUTER_PLAYER) return 2;
        }
        // Columns
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == HUMAN_PLAYER) return 1;
            if (board[0][i] == COMPUTER_PLAYER) return 2;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == HUMAN_PLAYER) return 1;
        if (board[0][0] == COMPUTER_PLAYER) return 2;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == HUMAN_PLAYER) return 1;
        if (board[0][2] == COMPUTER_PLAYER) return 2;
    }

    // Check for Draw (if no winner and board is full)
    if (isBoardFull(board)) {
        return 0; // It's a draw
    }

    return -1; // No winner yet, game in progress
}

// Converts a 1-9 choice into 0-indexed row and column coordinates
void convertChoiceToCoords(int choice, int *row, int *col) {
    *row = (choice - 1) / 3;
    *col = (choice - 1) % 3;
}

// Checks if the chosen move is valid
bool isValidMove(char board[3][3], int choice) {
    // Check if choice is within the valid range (1-9)
    if (choice < 1 || choice > 9) {
        return false;
    }

    int row, col;
    convertChoiceToCoords(choice, &row, &col); // Get 0-indexed coords

    // Check if the cell is already taken by 'X' or 'O'
    if (board[row][col] == HUMAN_PLAYER || board[row][col] == COMPUTER_PLAYER) {
        return false;
    }
    return true; // Valid move
}

// Places the player's symbol on the board
void makeMove(char board[3][3], char playerSymbol, int choice) {
    int row, col;
    convertChoiceToCoords(choice, &row, &col); // Get 0-indexed coords
    board[row][col] = playerSymbol;
}

// --- Player Input Functions ---

// Gets a move from a human player
void getHumanMove(int *choice) {
    int inputChoice;
    do {
        printf("Enter your move (1-9): ");
        // Clear input buffer before scanf
        while (getchar() != '\n');
        if (scanf("%d", &inputChoice) != 1) { // Check if one integer was successfully read
            printf("Invalid input. Please enter a number between 1 and 9.\n");
            inputChoice = -1; // Set to an invalid value to force re-loop
        }
    } while (inputChoice < 1 || inputChoice > 9); // Loop until valid range input
    *choice = inputChoice;
}

// --- AI Logic Functions ---

// Gets a move for the computer based on difficulty
void getComputerMove(char board[3][3], int *choice, int difficulty) {
    int r, c;
    int proposedChoice = -1; // Initialize to an invalid choice

    switch (difficulty) {
        case 1: // Easy AI: Random move
            do {
                proposedChoice = (rand() % 9) + 1; // Random choice (1-9)
            } while (!isValidMove(board, proposedChoice));
            break;

        case 2: // Medium AI: Win, Block, then Random
            // 1. Check for winning move for computer (O)
            for (int i = 1; i <= 9; i++) {
                if (isValidMove(board, i)) {
                    makeMove(board, COMPUTER_PLAYER, i); // Try the move
                    if (checkWin(board) == 2) { // If O wins
                        proposedChoice = i;
                        // Undo the move to restore board state for next iteration
                        convertChoiceToCoords(i, &r, &c);
                        board[r][c] = (char)(i + '0'); // Restore number
                        break; // Found winning move
                    }
                    // Undo the move
                    convertChoiceToCoords(i, &r, &c);
                    board[r][c] = (char)(i + '0'); // Restore number
                }
            }

            // 2. If no winning move, check for blocking move for human (X)
            if (proposedChoice == -1) {
                for (int i = 1; i <= 9; i++) {
                    if (isValidMove(board, i)) {
                        makeMove(board, HUMAN_PLAYER, i); // Try opponent's move
                        if (checkWin(board) == 1) { // If X wins
                            proposedChoice = i; // Block this spot
                            // Undo the move
                            convertChoiceToCoords(i, &r, &c);
                            board[r][c] = (char)(i + '0'); // Restore number
                            break; // Found blocking move
                        }
                        // Undo the move
                        convertChoiceToCoords(i, &r, &c);
                        board[r][c] = (char)(i + '0'); // Restore number
                    }
                }
            }

            // 3. If no winning or blocking move, pick a random valid move
            if (proposedChoice == -1) {
                do {
                    proposedChoice = (rand() % 9) + 1;
                } while (!isValidMove(board, proposedChoice));
            }
            break;

        case 3: // Impossible AI: Minimax algorithm
            proposedChoice = findBestMove(board);
            break;
    }

    *choice = proposedChoice;
    printf("Computer chose box %d.\n", proposedChoice);
    // Pause briefly for dramatic effect
    #ifdef _WIN32
        Sleep(1000); // 1 second delay for Windows
    #else
        usleep(1000000); // 1 second delay for Unix/Linux (1,000,000 microseconds)
    #endif
}

// Helper to check if the board is full
bool isBoardFull(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Check if cell contains an empty number (meaning it's not taken by X or O)
            if (board[i][j] != HUMAN_PLAYER && board[i][j] != COMPUTER_PLAYER) {
                return false;
            }
        }
    }
    return true;
}

// Minimax evaluation function
// Returns 10 if COMPUTER_PLAYER wins
// Returns -10 if HUMAN_PLAYER wins
// Returns 0 if Draw
int evaluate(char board[3][3]) {
    int winStatus = checkWin(board);
    if (winStatus == 2) return 10;   // Computer wins
    if (winStatus == 1) return -10;  // Human wins
    if (winStatus == 0) return 0;    // Draw
    return -2; // Still in progress (a value not 10, -10, or 0)
}

// Minimax algorithm
int minimax(char board[3][3], int depth, bool isMaximizingPlayer) {
    int score = evaluate(board);

    // If Maximizer (Computer) won, return their score
    if (score == 10) return score - depth; // Subtract depth to prefer faster wins

    // If Minimizer (Human) won, return their score
    if (score == -10) return score + depth; // Add depth to prefer faster losses (for opponent)

    // If it's a draw
    if (isBoardFull(board)) return 0;

    if (isMaximizingPlayer) { // Computer's turn
        int best = -1000; // Initialize to a very low value

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int choice = i * 3 + j + 1; // Convert 0-indexed to 1-9 choice

                if (isValidMove(board, choice)) {
                    char originalChar = board[i][j]; // Store original char (number)
                    board[i][j] = COMPUTER_PLAYER; // Make the move

                    // Call minimax recursively for the opponent
                    best = (best > minimax(board, depth + 1, false)) ? best : minimax(board, depth + 1, false);

                    board[i][j] = originalChar; // Undo the move
                }
            }
        }
        return best;
    } else { // Human's turn
        int best = 1000; // Initialize to a very high value

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int choice = i * 3 + j + 1; // Convert 0-indexed to 1-9 choice

                if (isValidMove(board, choice)) {
                    char originalChar = board[i][j]; // Store original char (number)
                    board[i][j] = HUMAN_PLAYER; // Make the move

                    // Call minimax recursively for the maximizer
                    best = (best < minimax(board, depth + 1, true)) ? best : minimax(board, depth + 1, true);

                    board[i][j] = originalChar; // Undo the move
                }
            }
        }
        return best;
    }
}

// Finds the best move using the minimax algorithm
int findBestMove(char board[3][3]) {
    int bestVal = -1000; // Computer wants to maximize score
    int bestMove = -1;   // The chosen move (1-9)
    char originalChar; // To store the original number of the cell

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int currentChoice = i * 3 + j + 1; // Convert 0-indexed to 1-9 choice

            if (isValidMove(board, currentChoice)) {
                originalChar = board[i][j]; // Save what was there (the number)
                board[i][j] = COMPUTER_PLAYER; // Make the move

                // Compute evaluation function for this move
                int moveVal = minimax(board, 0, false); // Opponent's turn next

                // Undo the move
                board[i][j] = originalChar;

                // If the value of the current move is more than the best value, then update bestVal
                if (moveVal > bestVal) {
                    bestMove = currentChoice;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}