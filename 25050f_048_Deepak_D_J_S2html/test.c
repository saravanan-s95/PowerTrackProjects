/* this is multi-line comment */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int secret_number;
    int user_guess;
    int attempts = 0;

    // Seed the random number generator using the current system time
    srand(time(NULL));
    secret_number = (rand() % 100) + 1; // Generates a number between 1 and 100

    printf("Welcome to the Number Guessing Game!\n");
    printf("I have chosen a secret number between 1 and 100.\n\n");

    // Loop continues until the user guesses the correct number
    do {
        printf("Enter your guess: ");
        
        // Validate that the input is a valid integer
        if (scanf("%d", &user_guess) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }

        attempts++;

        if (user_guess > secret_number) {
            printf("Too high! Try a lower number.\n\n");
        } else if (user_guess < secret_number) {
            printf("Too low! Try a higher number.\n\n");
        } else {
            printf("\nCongratulations! You guessed the number %d correctly.\n", secret_number);
            printf("Total attempts made: %d\n", attempts);
        }

    } while (user_guess != secret_number);

    return 0;
}
