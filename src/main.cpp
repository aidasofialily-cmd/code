#include <iostream>
#include <chrono>
#include <thread>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>

// ANSI Color codes for UX
const char* RESET = "\033[0m";
const char* BOLD = "\033[1m";
const char* GREEN = "\033[1;32m";
const char* RED = "\033[1;31m";
const char* BLUE = "\033[1;34m";
const char* YELLOW = "\033[1;33m";

void updateUI(int score, bool hardMode) {
    std::cout << GREEN << "Score: " << score << RESET;
    if (hardMode) {
        std::cout << RED << " [FAST]    " << RESET;
    } else {
        std::cout << BLUE << " [NORMAL]  " << RESET;
    }
    std::cout << "\r" << std::flush;
}

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int score = 0; bool hardMode = false; char input;
    std::cout << BOLD << "==========================\n      SPEED CLICKER\n==========================\n" << RESET
              << "Controls:\n " << YELLOW << "[h]" << RESET << " Toggle Hard Mode (10x Speed!)\n "
              << YELLOW << "[q]" << RESET << " Quit Game\n "
              << YELLOW << "[Any key]" << RESET << " Click!\n\n";

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
    updateUI(score, hardMode);

    while (true) {
        int base_timeout = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int remaining = std::max(0, base_timeout - static_cast<int>(elapsed));

        if (poll(fds, 1, remaining) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') {
                hardMode = !hardMode;
            } else {
                score++;
            }
            updateUI(score, hardMode);
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= base_timeout) {
            score++;
            last_tick = now;
            updateUI(score, hardMode);
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\nFinal Score: " << GREEN << score << RESET << "\nThanks for playing!\n";
    return 0;
}
