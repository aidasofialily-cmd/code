#include <iostream>
#include <chrono>
#include <thread>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>

// ANSI colors
#define RESET "\033[0m"
#define BOLD  "\033[1m"
#define BOLD_GREEN  "\033[1;32m"
#define BOLD_RED    "\033[1;31m"
#define BOLD_BLUE   "\033[1;34m"
#define BOLD_YELLOW "\033[1;33m"

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int score = 0; bool hardMode = false; char input;
    std::cout << BOLD << "==========================\n"
              << BOLD_GREEN << "      SPEED CLICKER\n"
              << BOLD << "==========================\n" << RESET
              << BOLD_YELLOW << "Controls:\n" << RESET
              << " [" << BOLD_YELLOW << "h" << RESET << "] Toggle Hard Mode (10x Speed!)\n"
              << " [" << BOLD_YELLOW << "q" << RESET << "] Quit Game\n"
              << " [" << BOLD_YELLOW << "Any key" << RESET << "] Click!\n\n";

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();

    while (true) {
        int base_timeout = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int remaining = std::max(0, (int)(base_timeout - (long long)elapsed));

        bool shouldUpdateUI = false;
        if (poll(fds, 1, remaining) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') {
                hardMode = !hardMode;
            } else {
                score++;
            }
            shouldUpdateUI = true;
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= base_timeout) {
            score++;
            last_tick = now;
            shouldUpdateUI = true;
        }

        if (shouldUpdateUI) {
            std::cout << "\r" << BOLD << "Score: " << BOLD_GREEN << score << RESET
                      << (hardMode ? BOLD_RED " [FAST!]    " : BOLD_BLUE " [NORMAL]    ")
                      << std::flush;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\n" << BOLD << "Final Score: " << BOLD_GREEN << score << RESET << "\n"
              << BOLD << "Thanks for playing!\n" << RESET;
    return 0;
}
