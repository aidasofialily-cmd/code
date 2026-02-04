## 2024-05-22 - DX as UX in a CLI Environment

**Learning:** In a developer-facing project with no graphical user interface (GUI), the Developer Experience (DX) *is* the User Experience (UX). A clean, well-documented, and easy-to-use build process is the equivalent of an intuitive UI for a developer. First impressions matter, and a project that's hard to get running creates a poor initial experience.

**Action:** I will apply UX principles to the DX by improving documentation, adding a clear build process, and establishing a logical project structure. This will make the project more welcoming and accessible to new developers.

## 2026-01-09 - Terminal I/O and Blocking
**Learning:** Standard terminal I/O is line-buffered by default. For real-time games, it's essential to use non-canonical mode (raw mode) to capture keypresses immediately. Also, internal journals should be kept clean if they are to be included in the repo.

## 2024-06-03 - Real-time CLI Feedback
**Learning:** For terminal applications, immediate feedback on user input is crucial for a "tactile" feel. Decoupling the UI update from the main game tick and centralizing it in a `render_ui` function ensures the display always reflects the current state instantly. Using `\r` and space padding allows for smooth, in-place updates without the flicker of clearing the whole screen.
**Action:** Always provide immediate visual confirmation of user actions in CLI tools, even if the primary state update happens on a timer.
