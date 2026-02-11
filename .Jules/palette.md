## 2024-05-22 - DX as UX in a CLI Environment

**Learning:** In a developer-facing project with no graphical user interface (GUI), the Developer Experience (DX) *is* the User Experience (UX). A clean, well-documented, and easy-to-use build process is the equivalent of an intuitive UI for a developer. First impressions matter, and a project that's hard to get running creates a poor initial experience.

**Action:** I will apply UX principles to the DX by improving documentation, adding a clear build process, and establishing a logical project structure. This will make the project more welcoming and accessible to new developers.

## 2026-01-09 - Terminal I/O and Blocking
**Learning:** Standard terminal I/O is line-buffered by default. For real-time games, it's essential to use non-canonical mode (raw mode) to capture keypresses immediately. Also, internal journals should be kept clean if they are to be included in the repo.

## 2026-05-22 - Immediate Feedback in CLI Loops
**Learning:** In terminal-based interactive loops, relying solely on a fixed timer for UI updates creates a laggy "disconnected" feel for the user. Decoupling the input processing from the timer and using an `updateUI` flag to trigger immediate redraws upon input significantly improves the "tactile" feel of the application.
**Action:** Always trigger a UI refresh immediately after processing user input in CLI games or interactive tools, rather than waiting for the next scheduled tick.
