## 2024-05-22 - DX as UX in a CLI Environment

**Learning:** In a developer-facing project with no graphical user interface (GUI), the Developer Experience (DX) *is* the User Experience (UX). A clean, well-documented, and easy-to-use build process is the equivalent of an intuitive UI for a developer. First impressions matter, and a project that's hard to get running creates a poor initial experience.

**Action:** I will apply UX principles to the DX by improving documentation, adding a clear build process, and establishing a logical project structure. This will make the project more welcoming and accessible to new developers.

## 2024-06-15 - Immediate Feedback in CLI
**Learning:** In terminal applications, users expect tactile feedback. Tying UI updates to an `updateUI` flag that is triggered by BOTH input events and timer ticks ensures the interface remains responsive and consistent. Using `poll()` with a calculated timeout allows for immediate input processing without the latency of fixed sleep intervals.
**Action:** Use `poll()` with dynamic timeouts and centralized rendering for all future CLI-based interactive tools.

## 2026-01-09 - Terminal I/O and Blocking
**Learning:** Standard terminal I/O is line-buffered by default. For real-time games, it's essential to use non-canonical mode (raw mode) to capture keypresses immediately. Also, internal journals should be kept clean if they are to be included in the repo.

## 2026-05-22 - Immediate Feedback in CLI Loops
**Learning:** In terminal-based interactive loops, relying solely on a fixed timer for UI updates creates a laggy "disconnected" feel for the user. Decoupling the input processing from the timer and using an `updateUI` flag to trigger immediate redraws upon input significantly improves the "tactile" feel of the application.
**Action:** Always trigger a UI refresh immediately after processing user input in CLI games or interactive tools, rather than waiting for the next scheduled tick.
## 2026-02-13 - Tactile Feedback in CLI
**Learning:** In terminal-based games, users expect immediate visual feedback for their actions. Relying on a periodic "tick" to update the UI creates a laggy feel. Using `poll()` with a dynamic timeout allows the application to remain idle yet wake up instantly to process and render user input.
**Action:** Always trigger a UI refresh immediately after processing user input in CLI applications, and use efficient waiting mechanisms (like `poll`) that can be interrupted by input.

## 2026-02-15 - Cursor Visibility in CLI Games
**Learning:** A blinking cursor in a TUI (Text User Interface) game is a significant visual distraction that breaks immersion. Furthermore, immediately dumping the user into the game loop without a "Press any key" prompt can be jarring and lead to accidental inputs. Hiding the cursor and adding a start prompt are small changes that drastically improve perceived polish.
**Action:** Always implement cursor hiding (`\033[?25l`) at the start of CLI interactive sessions and ensure it is restored (`\033[?25h`) on exit (including signal handlers). Always include a blocking wait for user input before starting the main loop.
