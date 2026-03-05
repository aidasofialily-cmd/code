## 2024-05-23 - Signal Handling and Terminal Restoration
**Vulnerability:** Terminal applications that modify terminal settings (like disabling echo or canonical mode) can leave the user's terminal in a "broken" state if they are interrupted by signals (SIGINT, SIGTERM) without restoring the original settings.
**Learning:** Always implement signal handlers to restore terminal state. However, signal handlers MUST be async-signal-safe. Using `std::cout` or `std::exit` in a signal handler is unsafe and can lead to deadlocks or undefined behavior.
**Prevention:** Use a global `struct termios` to save settings, and in the signal handler, use only async-signal-safe functions like `tcsetattr`, `write`, and `_exit`.

## 2024-05-23 - Integer Overflow in CLI Games
**Vulnerability:** Simple counters like game scores are often implemented as `int`, which can easily overflow if the game runs for a long time or user input is frequent. Signed integer overflow is undefined behavior in C++.
**Learning:** Use larger integer types like `long long` for counters that could grow indefinitely, and consider adding explicit checks if the value is used for critical operations.
**Prevention:** Default to `long long` for scores and other monotonically increasing counters in C++ projects.
