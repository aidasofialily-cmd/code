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
    std::cout << "==========================\n      SPEED CLICKER\n==========================\n"
              << "Controls:\n [h] Toggle Hard Mode (10x Speed!)\n [q] Quit Game\n [Any key] Click!\n\n";

    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
    while (true) {
        auto now = std::chrono::steady_clock::now();
        int tick_duration = hardMode ? 100 : 1000;
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int time_to_wait = tick_duration - elapsed;
        if (time_to_wait < 0) time_to_wait = 0;

        if (poll(fds, 1, time_to_wait) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') {
                hardMode = !hardMode;
                std::cout << (hardMode ? "\n[HARD MODE] Speed x10!\n" : "\n[NORMAL MODE]\n");
            } else score++;
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= tick_duration) {
            score++;
            last_tick = now;
            std::cout << "Score: " << score << (hardMode ? " [FAST]  " : " [NORMAL]  ") << "\r" << std::flush;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\nFinal Score: " << score << "\nThanks for playing!\n";
    return 0;
}
