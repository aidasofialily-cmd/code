#include <iostream>
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
    std::cout << "==========================\n\033[1;33m      SPEED CLICKER\033[0m\n==========================\n"
              << "\033[1;33mControls:\033[0m\n [h] Toggle Hard Mode (10x Speed!)\n [q] Quit Game\n [Any key] Click!\n\n";

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\033[1;32m\nFinal Score: " << score << "\033[0m\nThanks for playing!\n";
    return 0;
}
