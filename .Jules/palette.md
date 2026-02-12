## 2024-05-22 - DX as UX in a CLI Environment

**Learning:** In a developer-facing project with no graphical user interface (GUI), the Developer Experience (DX) *is* the User Experience (UX). A clean, well-documented, and easy-to-use build process is the equivalent of an intuitive UI for a developer. First impressions matter, and a project that's hard to get running creates a poor initial experience.

**Action:** I will apply UX principles to the DX by improving documentation, adding a clear build process, and establishing a logical project structure. This will make the project more welcoming and accessible to new developers.

## 2024-06-15 - Immediate Feedback in CLI
**Learning:** In terminal applications, users expect tactile feedback. Tying UI updates to an `updateUI` flag that is triggered by BOTH input events and timer ticks ensures the interface remains responsive and consistent. Using `poll()` with a calculated timeout allows for immediate input processing without the latency of fixed sleep intervals.
**Action:** Use `poll()` with dynamic timeouts and centralized rendering for all future CLI-based interactive tools.

## 2026-01-09 - Terminal I/O and Blocking
**Learning:** Standard terminal I/O is line-buffered by default. For real-time games, it's essential to use non-canonical mode (raw mode) to capture keypresses immediately. Also, internal journals should be kept clean if they are to be included in the repo.
