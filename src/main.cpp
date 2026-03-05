#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>
#include <fstream>

// ANSI Colors for CLI Polish
#define CLR_SCORE "\033[1;32m" // Bold Green
#define CLR_HARD  "\033[1;31m" // Bold Red
#define CLR_NORM  "\033[1;34m" // Bold Blue
#define CLR_CTRL  "\033[1;33m" // Bold Yellow
#include <csignal>
#include <cstdlib>
#include <fstream>

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
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"

struct termios oldt;

void restore_terminal(int signum) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // Use write() and _exit() because they are async-signal-safe
    const char msg[] = "\033[0m\033[?25h\n\nGame interrupted. Terminal settings restored.\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
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

#define CLR_SCORE "\033[1;32m"
#define CLR_HARD  "\033[1;31m"
#define CLR_NORM  "\033[1;34m"
#define CLR_CTRL  "\033[1;33m"
#define CLR_RESET "\033[0m"

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

    std::cout << "\033[?25l" << std::flush; // Hide cursor
    long long score = 0; bool hardMode = false; char input;
    long long highscore = 0;
    {
        std::ifstream hf("highscore.txt");
        if (hf.is_open()) {
            hf >> highscore;
            hf.close();
        }
    }

    std::cout << CLR_CTRL << "==========================\n      SPEED CLICKER\n==========================\n" << CLR_RESET
              << "Controls:\n " << CLR_CTRL << "[h]" << CLR_RESET << " Toggle Hard Mode (10x Speed!)\n "
              << CLR_CTRL << "[q]" << CLR_RESET << " Quit Game\n " << CLR_CTRL << "[Any key]" << CLR_RESET << " Click!\n\n";

    std::cout << CLR_CTRL << "Press any key to start... " << CLR_RESET << std::flush;
    if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
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
                    return 0;
                }
            }
        }
    }
    std::cout << "\r" << CLR_CTRL << "GO!           " << CLR_RESET << std::endl;
    tcflush(STDIN_FILENO, TCIFLUSH);
    std::cout << "\r" << CLR_NORM << "GO!             " << CLR_RESET << "\n" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    tcflush(STDIN_FILENO, TCIFLUSH);

    auto last_tick = std::chrono::steady_clock::now();
    bool updateUI = true;

    while (true) {
        bool update = false;
        if (poll(fds, 1, 0) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') hardMode = !hardMode; else score++;
            update = true;
        }
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count() >= (hardMode ? 100 : 1000)) {
            score++; last_tick = now; update = true;
        }

        if (updateUI) {
            std::cout << "\r" << CLR_SCORE << "Score: " << score
                      << " | High: " << std::max(score, highscore) << CLR_RESET << " "
            std::cout << "\r" << CLR_SCORE << "Score: " << score << CLR_RESET << " "
            highScore = std::max(highScore, score);
            std::cout << "\r" << CLR_SCORE << "Score: " << score << CLR_RESET
                      << " (High: " << highScore << ") "
                      << (hardMode ? CLR_HARD "[HARD MODE]" : CLR_NORM "[NORMAL MODE]")
                      << "           " << std::flush;
            updateUI = false;
        }
    }

    // Save high score
    std::ofstream hsFileOut("highscore.txt");
    hsFileOut << highScore;
    hsFileOut.close();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\n" << CLR_SCORE << "Final Score: " << score << CLR_RESET << "\n";
    if (score > highscore) {
        std::ofstream hf("highscore.txt");
        if (hf.is_open()) {
            hf << score;
            hf.close();
            std::cout << GREEN << "New High Score! 🏆" << RESET << "\n";
        }
    }
    std::cout << "Thanks for playing!\n";
    return 0;
}
