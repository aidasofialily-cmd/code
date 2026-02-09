#include <iostream>
#include <chrono>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>

// Colors
#define RESET        "\033[0m"
#define BOLD_GREEN   "\033[1;32m"
#define BOLD_RED     "\033[1;31m"
#define BOLD_BLUE    "\033[1;34m"
#define BOLD_YELLOW  "\033[1;33m"

void updateUI(int score, bool hardMode) {
    std::cout << "\r" << BOLD_GREEN << "Score: " << score << RESET
              << (hardMode ? BOLD_RED " [FAST]  " RESET : BOLD_BLUE " [NORMAL]  " RESET)
              << "            " << std::flush;
}

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int score = 0; bool hardMode = false; char input;
    std::cout << BOLD_YELLOW << "==========================\n      SPEED CLICKER\n==========================\n" << RESET
              << BOLD_YELLOW << "Controls:\n" << RESET
              << " [h] Toggle Hard Mode (10x Speed!)\n [q] Quit Game\n [Any key] Click!\n\n";

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
    updateUI(score, hardMode);

    while (true) {
        int base_timeout = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int remaining = std::max(0, static_cast<int>(base_timeout - elapsed));

        if (poll(fds, 1, remaining) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') hardMode = !hardMode;
            else score++;
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
    std::cout << BOLD_GREEN << "\nFinal Score: " << score << RESET << "\nThanks for playing!\n";
    return 0;
}
