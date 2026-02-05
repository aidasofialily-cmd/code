## 2024-05-22 - DX as UX in a CLI Environment

**Learning:** In a developer-facing project with no graphical user interface (GUI), the Developer Experience (DX) *is* the User Experience (UX). A clean, well-documented, and easy-to-use build process is the equivalent of an intuitive UI for a developer. First impressions matter, and a project that's hard to get running creates a poor initial experience.

**Action:** I will apply UX principles to the DX by improving documentation, adding a clear build process, and establishing a logical project structure. This will make the project more welcoming and accessible to new developers.

## 2026-01-09 - Terminal I/O and Blocking
**Learning:** Standard terminal I/O is line-buffered by default. For real-time games, it's essential to use non-canonical mode (raw mode) to capture keypresses immediately. Also, internal journals should be kept clean if they are to be included in the repo.

## 2026-02-05 - Immediate Tactile Feedback in CLI
**Learning:** In terminal-based games, users expect immediate visual feedback for their actions. Relying solely on a fixed-interval game loop for UI updates can make the application feel unresponsive or "laggy" if the loop interval is high. Centralizing rendering logic and triggering it immediately upon user input (key events) significantly improves the perceived responsiveness.
**Action:** Use an `updateUI` flag or a dedicated render function that can be called from both input handling and timer tick branches of the game loop.
