#include <iostream>
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
#define CLR_RESET "\033[0m"

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int highScore = 0;
    std::ifstream hsFileIn("highscore.txt");
    if (hsFileIn.is_open()) hsFileIn >> highScore;
    hsFileIn.close();

    std::cout << CLR_CTRL << "==========================\n      SPEED CLICKER\n==========================\n" << CLR_RESET
              << "High Score: " << CLR_SCORE << highScore << CLR_RESET << "\n\n"
              << "Controls:\n " << CLR_CTRL << "[h]" << CLR_RESET << " Toggle Hard Mode (10x Speed!)\n "
              << CLR_CTRL << "[q]" << CLR_RESET << " Quit Game\n [Any key] Click!\n\n";

    for (int i = 3; i > 0; --i) {
        std::cout << "Starting in " << i << "... \r" << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    tcflush(STDIN_FILENO, TCIFLUSH); // Clear input buffer before starting
    std::cout << "GO!                 \n" << std::flush;

    int score = 0; bool hardMode = false; char input;
    struct pollfd fds[1] = {{STDIN_FILENO, POLLIN, 0}};
    auto last_tick = std::chrono::steady_clock::now();
    bool updateUI = true;

    while (true) {
        int timeout_ms = hardMode ? 100 : 1000;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        int remaining = std::max(0, static_cast<int>(timeout_ms - elapsed));

        if (poll(fds, 1, remaining) > 0) {
            if (read(STDIN_FILENO, &input, 1) <= 0 || input == 'q') break;
            if (input == 'h') hardMode = !hardMode;
            else score++;
            updateUI = true;
        }

        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();
        if (elapsed >= timeout_ms) {
            score++;
            last_tick = now;
            updateUI = true;
        }

        if (updateUI) {
            std::cout << "\r" << CLR_SCORE << "Score: " << score << CLR_RESET << " "
                      << (hardMode ? CLR_HARD "[HARD MODE]" : CLR_NORM "[NORMAL MODE]")
                      << (score > highScore ? CLR_HARD " *NEW HIGH SCORE!*" : "")
                      << "    " << std::flush;
            updateUI = false;
        }
    }

    if (score > highScore) {
        std::ofstream hsFileOut("highscore.txt");
        hsFileOut << score;
        hsFileOut.close();
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n\n" << CLR_HARD << "==========================\n"
              << "        GAME OVER\n"
              << "==========================\n" << CLR_RESET
              << CLR_SCORE << "Final Score: " << score << CLR_RESET << "\n"
              << "Thanks for playing!\n";
    return 0;
}
