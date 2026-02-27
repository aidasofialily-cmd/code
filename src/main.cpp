#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>
#include <csignal>
#include <cstdlib>

// Color and formatting macros for terminal output
#define RESET     "\033[0m"
#define RED       "\033[31m"
#define GREEN     "\033[32m"
#define YELLOW    "\033[33m"
#define BLUE      "\033[34m"
#define CLR_SCORE "\033[1;36m"
#define CLR_HARD  "\033[1;31m"
#define CLR_NORM  "\033[1;32m"
#define CLR_CTRL  "\033[1;33m"
#define CLR_RESET "\033[0m"

struct termios oldt;

void restore_terminal(int signum) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // Use write() and _exit() because they are async-signal-safe
    const char* msg = "\033[0m\n\nGame interrupted. Terminal settings restored.\n";
    write(STDOUT_FILENO, msg, 52);
    _exit(signum);
}

int main() {
    struct termios newt;
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
        perror("tcgetattr");
        return 1;
    }
    std::signal(SIGINT, restore_terminal);
    std::signal(SIGTERM, restore_terminal);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
        perror("tcsetattr");
        return 1;
    }

    long long score = 0; bool hardMode = false; char input;
    std::cout << CLR_CTRL << "==========================\n      SPEED CLICKER\n==========================\n" << CLR_RESET
              << "Controls:\n " << CLR_CTRL << "[h]" << CLR_RESET << " Toggle Hard Mode (10x Speed!)\n "
              << CLR_CTRL << "[q]" << CLR_RESET << " Quit Game\n " << CLR_CTRL << "[Any key]" << CLR_RESET << " Click!\n\n";

    std::cout << CLR_CTRL << "Press any key to start..." << CLR_RESET << std::flush;
    if (read(STDIN_FILENO, &input, 1) > 0 && input == 'q') {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return 0;
    }

    // Responsive 3-2-1 Countdown
    for (int i = 3; i >= 1; --i) {
        std::cout << "\r" << CLR_CTRL << "Starting in " << i << "...   " << CLR_RESET << std::flush;
        auto countdown_start = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - countdown_start).count() < 1000) {
            struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
            if (poll(fds, 1, 100) > 0) {
                if (read(STDIN_FILENO, &input, 1) > 0 && input == 'q') {
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    std::cout << "\nQuit during countdown.\n";
                    return 0;
                }
            }
        }
    }
    std::cout << "\r" << CLR_CTRL << "Starting in GO!    " << CLR_RESET << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "\r" << std::string(30, ' ') << "\r" << std::flush;

    // Clear input buffer for a fair start
    tcflush(STDIN_FILENO, TCIFLUSH);

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
    bool updateUI = true;
    while (true) {
        int timeout_ms = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int remaining = std::max(0, static_cast<int>(timeout_ms - elapsed));

        if (poll(fds, 1, remaining) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') hardMode = !hardMode;
            else score++;
            updateUI = true;
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= timeout_ms) {
            score++;
            last_tick = now;
            updateUI = true;
        }

        if (updateUI) {
            std::cout << "\r" << CLR_SCORE << "Score: " << score << CLR_RESET << " "
                      << (hardMode ? CLR_HARD "[HARD MODE]" : CLR_NORM "[NORMAL MODE]")
                      << "    " << std::flush;
            updateUI = false;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\n" << CLR_SCORE << "Final Score: " << score << CLR_RESET << "\nThanks for playing!\n";
    return 0;
}
