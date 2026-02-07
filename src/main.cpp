#include <iostream>
#include <chrono>
#include <thread>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>

#define RESET       "\033[0m"
#define BOLD_GREEN   "\033[1;32m"
#define BOLD_RED     "\033[1;31m"
#define BOLD_BLUE    "\033[1;34m"
#define BOLD_YELLOW  "\033[1;33m"

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int score = 0; bool hardMode = false; char input;
    std::cout << BOLD_YELLOW << "==========================\n      SPEED CLICKER\n==========================\n" << RESET
              << "Controls:\n"
              << BOLD_YELLOW << " [h] " << RESET << "Toggle Hard Mode (10x Speed!)\n"
              << BOLD_YELLOW << " [q] " << RESET << "Quit Game\n"
              << BOLD_YELLOW << " [Any key] " << RESET << "Click!\n\n";

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
    bool updateUI = true;
    while (true) {
        if (updateUI) {
            std::cout << "\r" << BOLD_GREEN << "Score: " << score << RESET << "  "
                      << (hardMode ? BOLD_RED "[HARD MODE] " : BOLD_BLUE "[NORMAL]    ") << RESET
                      << "  (Press 'q' to quit)      " << std::flush;
            updateUI = false;
        }

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int timeout = hardMode ? 100 : 1000;
        int remaining = std::max(0, timeout - static_cast<int>(elapsed));

        if (poll(fds, 1, remaining) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') hardMode = !hardMode;
            else score++;
            updateUI = true;
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= (hardMode ? 100 : 1000)) {
            score++;
            last_tick = now;
            updateUI = true;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\n" << BOLD_YELLOW << "==========================\n" << RESET
              << BOLD_GREEN << "  FINAL SCORE: " << score << RESET << "\n"
              << BOLD_YELLOW << "==========================\n" << RESET
              << "  Thanks for playing!\n\n";
    return 0;
}
