#include <iostream>
#include <chrono>
#include <thread>
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#include <algorithm>

void render_ui(int score, bool hardMode) {
    const char* green = "\033[32m";
    const char* red = "\033[31m";
    const char* bold = "\033[1m";
    const char* reset = "\033[0m";

    std::cout << "\r" << "Score: " << green << bold << score << reset;
    if (hardMode) {
        std::cout << "  [" << red << bold << "HARD MODE" << reset << "]  ";
    } else {
        std::cout << "  [NORMAL]     ";
    }
    std::cout << std::flush;
}

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int score = 0; bool hardMode = false; char input;
    std::cout << "==========================\n      SPEED CLICKER\n==========================\n"
              << "Controls:\n [h] Toggle Hard Mode (10x Speed!)\n [q] Quit Game\n [Any key] Click!\n\n";

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();

    render_ui(score, hardMode);

    while (true) {
        int interval = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int wait_time = std::max(0, (int)(interval - elapsed));

        if (poll(fds, 1, wait_time) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') {
                hardMode = !hardMode;
            } else {
                score++;
            }
            render_ui(score, hardMode);
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= interval) {
            score++;
            last_tick = now;
            render_ui(score, hardMode);
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\n==========================\n"
              << "      GAME OVER\n"
              << "==========================\n"
              << "Final Score: " << score << "\n"
              << "Thanks for playing!\n";
    return 0;
}
