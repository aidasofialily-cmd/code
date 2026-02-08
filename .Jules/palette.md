## 2024-05-22 - DX as UX in a CLI Environment

**Learning:** In a developer-facing project with no graphical user interface (GUI), the Developer Experience (DX) *is* the User Experience (UX). A clean, well-documented, and easy-to-use build process is the equivalent of an intuitive UI for a developer. First impressions matter, and a project that's hard to get running creates a poor initial experience.

**Action:** I will apply UX principles to the DX by improving documentation, adding a clear build process, and establishing a logical project structure. This will make the project more welcoming and accessible to new developers.

## 2026-01-09 - Terminal I/O and Blocking
**Learning:** Standard terminal I/O is line-buffered by default. For real-time games, it's essential to use non-canonical mode (raw mode) to capture keypresses immediately. Also, internal journals should be kept clean if they are to be included in the repo.

## 2025-05-14 - Tactile Feedback in CLI
**Learning:** In terminal-based interactive applications, waiting for the next "tick" of a game loop to update the UI creates a "laggy" feel. Users expect immediate visual confirmation of their actions. Consolidating the rendering logic and triggering it immediately upon both input events and timer ticks significantly improves the perceived responsiveness (tactile feedback).
**Action:** Always trigger a UI refresh immediately after processing user input, even if a periodic timer update is already scheduled.

## 2025-05-14 - CI Integrity as Developer UX
**Learning:** Broken CI workflows or redundant configuration files (like Rust files in a C++ project) create a confusing and frustrating experience for developers. Maintaining a clean and relevant project structure is essential for a good "first contact" UX.
**Action:** Always verify that CI workflows match the project's actual language and build system, and remove any leftover boilerplate from other languages.
