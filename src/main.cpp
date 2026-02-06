#include <iostream>
#include <chrono>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>

#define RESET       "\033[0m"
#define B_GREEN     "\033[1;32m"
#define B_RED       "\033[1;31m"
#define B_BLUE      "\033[1;34m"
#define B_YELLOW    "\033[1;33m"

void renderUI(int score, bool hardMode) {
    std::cout << "\r" << B_GREEN << "Score: " << score << RESET
              << (hardMode ? B_RED " [FAST]   " : B_BLUE " [NORMAL] ") << RESET << std::flush;
}

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int score = 0; bool hardMode = false; char input;
    std::cout << B_YELLOW << "==========================\n      SPEED CLICKER\n==========================\n"
              << "Controls:\n [h] Toggle Hard Mode (10x Speed!)\n [q] Quit Game\n [Any key] Click!\n\n" << RESET;

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
    renderUI(score, hardMode);

    while (true) {
        int interval = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int remaining = std::max(0, static_cast<int>(interval - elapsed));

        if (poll(fds, 1, remaining) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') hardMode = !hardMode;
            else score++;
            renderUI(score, hardMode);
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= (hardMode ? 100 : 1000)) {
            score++;
            last_tick = now;
            renderUI(score, hardMode);
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\n" << B_GREEN << "Final Score: " << score << RESET << "\nThanks for playing!\n";
    return 0;
}
