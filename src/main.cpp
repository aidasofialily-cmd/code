#include <iostream>
#include <chrono>
#include <thread>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int score = 0; bool hardMode = false; char input;
    const char *G = "\033[1;32m", *R = "\033[1;31m", *B = "\033[1;34m", *Y = "\033[1;33m", *N = "\033[0m";

    std::cout << "==========================\n      SPEED CLICKER\n==========================\n"
              << "Controls:\n " << Y << "[h]" << N << " Toggle Hard Mode (10x Speed!)\n "
              << Y << "[q]" << N << " Quit Game\n " << Y << "[Any key]" << N << " Click!\n\n";

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
    while (true) {
        int base = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();

        if (poll(fds, 1, std::max(0, static_cast<int>(base - elapsed))) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') hardMode = !hardMode;
            else score++;
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= base) {
            score++;
            last_tick = now;
        }

        std::cout << "\rScore: " << G << score << N << " Status: "
                  << (hardMode ? R : B) << (hardMode ? "[HARD MODE]" : "[NORMAL]") << N << "          " << std::flush;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\nFinal Score: " << G << score << N << "\nThanks for playing!\n";
    return 0;
}
