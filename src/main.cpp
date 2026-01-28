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
    std::cout << "==========================\n      SPEED CLICKER\n==========================\n"
              << "Controls:\n [h] Toggle Hard Mode (10x Speed!)\n [q] Quit Game\n [Any key] Click!\n\n";

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
    while (true) {
        int tick_duration = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();

        if (elapsed >= tick_duration) {
            score++;
            last_tick = now;
            std::cout << "Score: " << score << (hardMode ? " [FAST]  " : " [NORMAL]  ") << "\r" << std::flush;
        }

        int poll_timeout = std::max(0LL, tick_duration - elapsed);

        if (poll(fds, 1, poll_timeout) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') {
                hardMode = !hardMode;
                std::cout << (hardMode ? "\n[HARD MODE] Speed x10!\n" : "\n[NORMAL MODE]\n");
                last_tick = std::chrono::steady_clock::now(); // Reset timer on mode change
            } else {
                score++;
                std::cout << "Score: " << score << (hardMode ? " [FAST]  " : " [NORMAL]  ") << "\r" << std::flush;
            }
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\nFinal Score: " << score << "\nThanks for playing!\n";
    return 0;
}
