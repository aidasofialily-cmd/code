#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <iomanip>
#include "utils.hpp"

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
#define ERASE_LINE "\033[K"

struct termios oldt;

void restore_terminal(int signum) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // Use write() and _exit() because they are async-signal-safe
    const char msg[] = "\033[0m\033[?25h\n\nGame interrupted. Terminal settings restored.\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    _exit(signum);
}

long long load_highscore() {
    long long highscore = 0;
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highscore;
        file.close();
    }
    return highscore;
}

void save_highscore(long long score) {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << score;
        file.close();
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

    std::cout << "\033[?25l" << std::flush; // Hide cursor

    long long highscore = load_highscore();
    long long initialHighscore = highscore;
    long long score = 0;
    bool hardMode = false;
    char input;

    std::cout << CLR_CTRL << "==========================\n      SPEED CLICKER\n==========================\n" << CLR_RESET;

    if (highscore > 0) {
        std::cout << " Personal Best: " << CLR_SCORE << formatWithCommas(highscore) << CLR_RESET << "\n\n";
    }

    std::cout << "Controls:\n " << CLR_CTRL << "[h]" << CLR_RESET << " Toggle Hard Mode (10x Speed!)\n "
              << CLR_CTRL << "[q]" << CLR_RESET << " Quit Game\n " << CLR_CTRL << "[Any key]" << CLR_RESET << " Click!\n\n";

    std::cout << "Press any key to start... " << std::flush;
    struct pollfd start_fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    if (poll(start_fds, 1, -1) > 0) {
        if (read(STDIN_FILENO, &input, 1) > 0 && input == 'q') {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            std::cout << "\033[?25h" << std::flush;
            return 0;
        }
    }

    for (int i = 3; i > 0; --i) {
        std::cout << "\r" ERASE_LINE "Starting in " << CLR_CTRL << i << CLR_RESET << "... " << std::flush;
        auto start_wait = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_wait).count() < 1000) {
            int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_wait).count();
            int remaining = std::max(0, 1000 - elapsed);
            if (poll(start_fds, 1, std::min(remaining, 100)) > 0) {
                if (read(STDIN_FILENO, &input, 1) > 0 && input == 'q') {
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    std::cout << "\n\033[?25h" << std::flush;
                    return 0;
                }
            }
        }
    }
    std::cout << "\r" ERASE_LINE << CLR_NORM << "GO!" << CLR_RESET << "\n" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    tcflush(STDIN_FILENO, TCIFLUSH);

    long long manualClicks = 0;
    auto game_start_time = std::chrono::steady_clock::now();
    auto last_tick = game_start_time;
    bool updateUI = true;
    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    while (true) {
        int timeout_ms = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int remaining = std::max(0, static_cast<int>(timeout_ms - elapsed));

        if (poll(fds, 1, remaining) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') hardMode = !hardMode;
            else {
                manualClicks++;
                score++;
                if (score > highscore) highscore = score;
            }
            updateUI = true;
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= timeout_ms) {
            score++;
            if (score > highscore) highscore = score;
            last_tick = now;
            updateUI = true;
        }

        if (updateUI) {
            std::cout << "\r" ERASE_LINE << CLR_SCORE << "Score: " << formatWithCommas(score) << CLR_RESET << " | High: " << formatWithCommas(highscore) << " "
                      << (hardMode ? CLR_HARD "[HARD MODE]" : CLR_NORM "[NORMAL MODE]")
                      << (score > initialHighscore ? " NEW BEST! 🥳 (was " + formatWithCommas(initialHighscore) + ")" : "")
                      << std::flush;
            updateUI = false;
        }
    }

    auto game_end_time = std::chrono::steady_clock::now();
    double duration_sec = std::chrono::duration_cast<std::chrono::milliseconds>(game_end_time - game_start_time).count() / 1000.0;
    double cps = duration_sec > 0.0 ? (static_cast<double>(manualClicks) / duration_sec) : 0.0;

    if (score > initialHighscore) {
        save_highscore(score);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\n" << CLR_CTRL << "======================================\n"
              << "              GAME OVER\n"
              << "======================================\n" << CLR_RESET;
    std::cout << " " << CLR_SCORE << "Final Score:    " << formatWithCommas(score) << CLR_RESET << "\n";
    std::cout << " Total Clicks:   " << CLR_NORM << formatWithCommas(manualClicks) << CLR_RESET << "\n";
    std::cout << " Play Time:      " << CLR_NORM << std::fixed << std::setprecision(2) << duration_sec << "s" << CLR_RESET << "\n";
    std::cout << " Average CPS:    " << CLR_NORM << std::fixed << std::setprecision(2) << cps << " clicks/s" << CLR_RESET << "\n";
    std::cout << CLR_CTRL << "======================================\n" << CLR_RESET;

    if (score > initialHighscore) {
        std::cout << "\n ✨ " << CLR_CTRL << "NEW BEST! 🥳 " << CLR_RESET << "You beat your old score of " << CLR_SCORE << formatWithCommas(initialHighscore) << CLR_RESET << "!\n";
        std::cout << " " << CLR_NORM << "Congratulations! A new personal best!\n\n" << CLR_RESET;
    } else {
        std::cout << "\n" << CLR_SCORE << "Personal Best: " << formatWithCommas(highscore) << CLR_RESET << "\n\n";
    }
    std::cout << "Thanks for playing!\n";
    std::cout << "\033[?25h" << std::flush;
    return 0;
}
