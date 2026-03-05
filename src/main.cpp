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

struct termios oldt;

void restore_terminal(int signum) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    const char* msg = "\033[0m\n\nGame interrupted. Terminal settings restored.\n";
    write(STDOUT_FILENO, msg, 52);
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

    int score = 0; bool hardMode = false; char input;
    std::cout << "==========================\n\033[1;33m      SPEED CLICKER\033[0m\n==========================\n"
              << "\033[1;33mControls:\033[0m\n [h] Toggle Hard Mode (10x Speed!)\n [q] Quit Game\n [Any key] Click!\n\n";

    int score = 0; bool hardMode = false; char input;
    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    if (poll(fds, 1, -1) > 0) {
        if (read(STDIN_FILENO, &input, 1) > 0 && input == 'q') {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            std::cout << "\033[?25h" << std::flush;
            return 0;
        }
    }

    for (int i = 3; i > 0; --i) {
        std::cout << "\rStarting in " << i << "... " << std::flush;
        auto start_wait = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_wait).count() < 1000) {
            int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_wait).count();
            int remaining = std::max(0, 1000 - elapsed);
            if (poll(fds, 1, std::min(remaining, 100)) > 0) {
                if (read(STDIN_FILENO, &input, 1) > 0 && input == 'q') {
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    std::cout << "\033[?25h\n" << std::flush;
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
        if (update) {
            std::cout << "\r\033[1;32mScore: " << score << "\033[0m"
                      << (hardMode ? "\033[1;31m [FAST]  \033[0m" : "\033[1;34m [NORMAL]  \033[0m") << std::flush;
        }
    }

    if (score > highScore) {
        std::ofstream hsFileOut("highscore.txt");
        hsFileOut << score;
        hsFileOut.close();
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\033[1;32m\nFinal Score: " << score << "\033[0m\nThanks for playing!\n";
    return 0;
}
