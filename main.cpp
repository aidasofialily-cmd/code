#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));
    int secretNumber = rand() % 100 + 1;
    int guess;
    int tries = 0;

    std::cout << "Guess the number (between 1 and 100): ";

    do {
        std::cin >> guess;
        tries++;
        if (guess > secretNumber) {
            std::cout << "Too high! Try again: ";
        } else if (guess < secretNumber) {
            std::cout << "Too low! Try again: ";
        }
    } while (guess != secretNumber);

    std::cout << "Congratulations! You guessed the number in " << tries << " tries." << std::endl;

    return 0;
}
