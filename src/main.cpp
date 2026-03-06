#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>
#include <csignal>
#include <cstdlib>

// Color and formatting macros for terminal output
#define CLR_SCORE "\033[1;36m" // Bold Cyan
#define CLR_HARD  "\033[1;31m" // Bold Red
#define CLR_NORM  "\033[1;32m" // Bold Green
#define CLR_CTRL  "\033[1;33m" // Bold Yellow
#define CLR_RESET "\033[0m"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"

struct termios oldt;

void restore_terminal(int signum) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // Use write() and _exit() because they are async-signal-safe
    const char* msg = "\033[?25h\033[0m\n\nGame interrupted. Terminal settings restored.\n";
    write(STDOUT_FILENO, msg, 58);
    _exit(signum);
}

long long load_highscore() {
    long long hs = 0;
    std::ifstream f("highscore.txt");
    if (f.is_open()) {
        f >> hs;
    }
    return hs;
}

void save_highscore(long long hs) {
    std::ofstream f("highscore.txt");
    if (f.is_open()) {
        f << hs;
    }
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

    long long highscore = load_highscore();
    long long score = 0;
    bool hardMode = false;
    char input;

    std::cout << CLR_CTRL << "==========================\n      SPEED CLICKER\n==========================\n" << CLR_RESET
              << CLR_SCORE << "   CURRENT HIGH SCORE: " << highscore << CLR_RESET << "\n\n"
              << "Controls:\n " << CLR_CTRL << "[h]" << CLR_RESET << " Toggle Hard Mode (10x Speed!)\n "
              << CLR_CTRL << "[q]" << CLR_RESET << " Quit Game\n " << CLR_CTRL << "[Any key]" << CLR_RESET << " Click!\n\n";

    if (highscore > 0) {
        std::cout << CLR_SCORE << "Personal Best: " << highscore << CLR_RESET << "\n\n";
    }

    std::cout << CLR_CTRL << "Press any key to start... " << CLR_RESET << std::flush;
    if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        std::cout << SHOW_CURSOR << std::endl;
        return 0;
    }

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    for (int i = 3; i > 0; --i) {
        std::cout << "\r" << CLR_CTRL << "Starting in " << i << "...   " << CLR_RESET << std::flush;
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < 1000) {
            if (poll(fds, 1, 100) > 0) {
                if (read(STDIN_FILENO, &input, 1) > 0 && input == 'q') {
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    std::cout << SHOW_CURSOR << "\n" << std::flush;
                    return 0;
                }
            }
        }
    }
    std::cout << "\r" << CLR_NORM << "GO!             " << CLR_RESET << "\n" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    tcflush(STDIN_FILENO, TCIFLUSH);

    auto last_tick = std::chrono::steady_clock::now();
    bool updateUI = true;

    while (true) {
        if (poll(fds, 1, 0) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') hardMode = !hardMode; else score++;
            updateUI = true;
        }
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count() >= (hardMode ? 100 : 1000)) {
            score++; last_tick = now; updateUI = true;
        }

        if (updateUI) {
            std::cout << "\r" << CLR_SCORE << "Score: " << score << CLR_RESET
                      << " | High: " << (score > highscore ? score : highscore) << " "
                      << (hardMode ? CLR_HARD "[HARD MODE]" : CLR_NORM "[NORMAL MODE]")
                      << "           " << std::flush;
            updateUI = false;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\n" << CLR_SCORE << "Final Score: " << score << CLR_RESET << "\n";

    if (score > highscore) {
        std::cout << CLR_NORM << "NEW HIGH SCORE! " << CLR_RESET << "Previous was " << highscore << "\n";
        save_highscore(score);
    }

    std::cout << "Thanks for playing!\n";
    return 0;
}
