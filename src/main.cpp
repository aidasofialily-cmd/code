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
    std::cout << CLR_CTRL << "==========================\n      SPEED CLICKER\n==========================\n" << CLR_RESET
              << "Controls:\n " << CLR_CTRL << "[h]" << CLR_RESET << " Toggle Hard Mode (10x Speed!)\n "
              << CLR_CTRL << "[q]" << CLR_RESET << " Quit Game\n [Any key] Click!\n\n";

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
    bool updateUI = true;
    while (true) {
        int timeout = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int poll_timeout = std::max(0, timeout - elapsed);

        if (poll(fds, 1, poll_timeout) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') {
                hardMode = !hardMode;
            } else {
                score++;
            }
            updateUI = true;
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= timeout) {
            score++; last_tick = now;
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
