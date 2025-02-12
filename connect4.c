
// By: mathewn24

#include <stdio.h>

/* Macros used to represent the state of each square */
#define EMPTY 0
#define CIRCLE 1
#define CROSS 2

/* Initialize the game board by setting all squares to EMPTY */
void initGameBoard(int gameBoard[6][7]) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            gameBoard[i][j] = EMPTY;
        }   
    }
}

/* Display the game board on the screen.*/
void printGameBoard(int gameBoard[6][7]) {
    // Print the game board
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            printf("|");
            if (gameBoard[i][j] == EMPTY) {
                printf("   ");
            } else if (gameBoard[i][j] == CIRCLE) {
                printf(" O ");
            } else if (gameBoard[i][j] == CROSS) {
                printf(" X ");
            }
            if (j == 6) {
                printf("|");
            }
        }
        printf("\n");
    }

    // Print the column numbers
    for (int cols = 0; cols < 7; cols++) {
        printf("  %d ", cols + 1);
    }
    printf("\n");
}

/* Ask the human player to place the mark.*/
void placeMarkByHumanPlayer(int gameBoard[6][7], int *mark) {
    printf("Enter a column (1-7): ");
    scanf("%d", mark);
    while (*mark < 1 || *mark > 7) {
        printf("Input out of range. Please input again: ");
        scanf("%d", mark);
    }
    while (gameBoard[0][*mark-1] != EMPTY) {
        printf("Colum is full. Please input again:");
        scanf("%d", mark);
    }
}

/* Return 1 if there is a winner.*/
int hasWinner(int gameBoard[6][7]){

    // Check horizontal locations for win
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 4; col++) { // Check up to column 4
            if (gameBoard[row][col] != EMPTY &&
                gameBoard[row][col] == gameBoard[row][col + 1] &&
                gameBoard[row][col] == gameBoard[row][col + 2] &&
                gameBoard[row][col] == gameBoard[row][col + 3]) {
                return 1; // Winner found
            }
        }
    }

    // Check vertical locations for win
    for (int col = 0; col < 7; col++) {
        for (int row = 0; row < 3; row++) { // Check up to row 3
            if (gameBoard[row][col] != EMPTY &&
                gameBoard[row][col] == gameBoard[row + 1][col] &&
                gameBoard[row][col] == gameBoard[row + 2][col] &&
                gameBoard[row][col] == gameBoard[row + 3][col]) {
                return 1; // Winner found
            }
        }
    }

    // Check diagonal locations (bottom-left to top-right)
    for (int row = 3; row < 6; row++) {
        for (int col = 0; col < 4; col++) {
            if (gameBoard[row][col] != EMPTY &&
                gameBoard[row][col] == gameBoard[row - 1][col + 1] &&
                gameBoard[row][col] == gameBoard[row - 2][col + 2] &&
                gameBoard[row][col] == gameBoard[row - 3][col + 3]) {
                return 1; // Winner found
            }
        }
    }

    // Check diagonal locations (top-left to bottom-right)
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 4; col++) {
            if (gameBoard[row][col] != EMPTY &&
                gameBoard[row][col] == gameBoard[row + 1][col + 1] &&
                gameBoard[row][col] == gameBoard[row + 2][col + 2] &&
                gameBoard[row][col] == gameBoard[row + 3][col + 3]) {
                return 1; // Winner found
            }
        }
    }

    return 0; // No winner found
}

/* Return 1 if the game board is full. */
int isFull(int gameBoard[6][7]) {
    for (int i = 0; i < 7; i++) {
        if (gameBoard[0][i] == EMPTY) {
            return 0;
        }
    }
    return 1;
}

/* Determine the next move of the computer player.*/
void placeMarkByComputerPlayer(int gameBoard[6][7]) {
    int winningMove = -1;

    // 1. Check for a winning move for the computer
    for (int col = 0; col < 7; col++) {
        // Check if the column is not full
        if (gameBoard[0][col] == EMPTY) {
            // Place the computer's mark temporarily
            for (int row = 5; row >= 0; row--) {
                if (gameBoard[row][col] == EMPTY) {
                    gameBoard[row][col] = CROSS; // Place computer's mark
                    if (hasWinner(gameBoard)) {
                        winningMove = col; // Found a winning move
                    }
                    gameBoard[row][col] = EMPTY; // Remove mark
                    break;
                }
            }
        }
        if (winningMove != -1) break; // Stop if a winning move is found
    }

    // If a winning move is found, place it
    if (winningMove != -1) {
        for (int row = 5; row >= 0; row--) {
            if (gameBoard[row][winningMove] == EMPTY) {
                gameBoard[row][winningMove] = CROSS; // Place computer's mark
                printf("Computer's turn: Column %d\n", winningMove + 1);
                return; // Exit after placing the mark
            }
        }
    }

    // 2. Check for a winning move for the human player
    for (int col = 0; col < 7; col++) {
        // Check if the column is not full
        if (gameBoard[0][col] == EMPTY) {
            // Place the human's mark temporarily
            for (int row = 5; row >= 0; row--) {
                if (gameBoard[row][col] == EMPTY) {
                    gameBoard[row][col] = CIRCLE; // Place human's mark
                    if (hasWinner(gameBoard)) {
                        // Block the winning move
                        gameBoard[row][col] = CROSS; // Place computer's mark
                        printf("Computer's turn: Column %d\n", col + 1);
                        return; // Exit after placing the mark
                    }
                    gameBoard[row][col] = EMPTY; // Remove mark
                    break;
                }
            }
        }
    }

    // 3. Choose the column with the fewest marks
    int fewestMarks = 7; // Max columns
    int bestColumn = -1;

    for (int col = 0; col < 7; col++) {
        int count = 0;
        for (int row = 0; row < 6; row++) {
            if (gameBoard[row][col] != EMPTY) {
                count++;
            }
        }
        // Check for the column with the fewest marks
        if (count < fewestMarks) {
            fewestMarks = count;
            bestColumn = col;
        } else if (count == fewestMarks && col > bestColumn) {
            bestColumn = col; // Prefer larger column number in case of a tie
        }
    }

    // Place the mark in the chosen column
    if (bestColumn != -1) {
        for (int row = 5; row >= 0; row--) {
            if (gameBoard[row][bestColumn] == EMPTY) {
                gameBoard[row][bestColumn] = CROSS; // Place computer's mark
                printf("Computer's turn: Column %d\n", bestColumn + 1);
                return; // Exit after placing the mark
            }
        }
    }
}

/* The main function */
int main() {
    /* Local variables */
    int gameBoard[6][7];    // Each element stores 0 (EMPTY), 1 (CIRCLE), or 2 (CROSS)
    int currentPlayer;      // 1: Player 1 (CIRCLE)  2: Player 2 (CROSS)
    int gameContinue;       // 1: The game continues   0: The game ends
    int numOfHumanPlayers;  // 1 or 2
    int mark;

    initGameBoard(gameBoard);
    printGameBoard(gameBoard);
    currentPlayer = 1; // Start with Player 1
    gameContinue = 1;
    printf("Enter the number of human players [1-2]:\n");
    scanf("%d", &numOfHumanPlayers); // Assuming valid input

    while (gameContinue) {
        printf("Player %d's turn:\n", currentPlayer);

        // Player's turn
        placeMarkByHumanPlayer(gameBoard, &mark); // Get the player's mark
        for (int row = 6 - 1; row >= 0; row--) {
        if (gameBoard[row][mark - 1] == EMPTY) {
            gameBoard[row][mark - 1] = currentPlayer; // Assuming player 1's mark is 1
            break;
        }
    }
        // Mark placement is handled inside `placeMarkByHumanPlayer`
        printGameBoard(gameBoard); // Print the board after the player's move

        if (hasWinner(gameBoard)) {
            printf("Congratulations! Player %d wins!\n", currentPlayer);
            gameContinue = 0; // End the game
            break; // Exit loop
        } else if (isFull(gameBoard)) {
            printf("The game is a draw!\n");
            gameContinue = 0; // End the game
            break; // Exit loop
        }

        // Toggle current player
        currentPlayer = (currentPlayer == 1) ? 2 : 1; // Switch between players

        // If there is only one human player, it's the computer's turn
        if (numOfHumanPlayers == 1 && gameContinue) {
            printf("Computer's turn:\n");
            placeMarkByComputerPlayer(gameBoard); // Computer makes its move
            printGameBoard(gameBoard); // Print the board after computer's move

            if (hasWinner(gameBoard)) {
                printf("Congratulations! Player %d wins!\n", currentPlayer);
                gameContinue = 0; // End the game
                break;
            } else if (isFull(gameBoard)) {
                printf("The game is a draw!\n");
                gameContinue = 0; // End the game
                break;
            }

            // Toggle current player back to human
            currentPlayer = 1; // Reset to Player 1
        }
    }

    return 0;
}
