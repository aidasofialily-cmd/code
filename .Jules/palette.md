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

## 2026-05-23 - Ensuring a Fair Start in CLI Games
**Learning:** Users often spam keys during a game's countdown phase in anticipation. If these inputs are buffered and processed immediately when the game starts, it can lead to an unfair advantage or accidental actions. Using `tcflush(STDIN_FILENO, TCIFLUSH)` after the countdown ensures the game starts with a clean slate.
**Action:** Always clear the input buffer with `tcflush` after a blocking countdown or transition period in interactive CLI applications to ensure intent-based interaction.

## 2026-03-02 - Hiding the Cursor in CLI Games
**Learning:** In terminal applications that require rapid visual updates or where user input doesn't involve typing text, an actively blinking cursor can be a visual distraction. Hiding it during interaction (`\033[?25l`) and rigorously ensuring it is restored (`\033[?25h`) on exit—including signal interrupts—significantly improves the aesthetic and focus.
**Action:** Always hide the cursor for interactive CLI games and explicitly restore it across all exit paths, including async-signal-safe signal handlers.

## 2026-05-24 - Inclusive Achievement Feedback and Real-time Goal Visibility
**Learning:** For first-time users, the absence of initial data (like a high score of 0) should not block the feedback loop for achievements. Triggering celebrations like "NEW BEST!" as soon as the first point is scored provides immediate delight. Additionally, displaying the "target" (the current high score) in real-time next to the active score provides a constant sense of progression and challenge.
**Action:** Always ensure achievement logic handles "zero-state" starts gracefully to include new users, and provide persistent visibility of performance targets in live UIs.
