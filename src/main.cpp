#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <poll.h>
#include <unistd.h>
#include <termios.h>

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
    while (true) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int period = hardMode ? 100 : 1000;
        int timeout = std::max(0, period - static_cast<int>(elapsed));

        if (poll(fds, 1, timeout) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') hardMode = !hardMode;
            else score++;
        } else {
            score++;
            last_tick = std::chrono::steady_clock::now();
        }
        std::cout << "Score: " << score << (hardMode ? " [FAST]   " : " [NORMAL] ") << "\r" << std::flush;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\nFinal Score: " << score << "\nThanks for playing!\n";
    return 0;
}
