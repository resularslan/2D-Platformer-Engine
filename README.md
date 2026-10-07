# 2D Platformer Engine built with C++ and SDL3

This project is a custom-built 2D platformer engine developed from scratch using C++ and SDL3. Originally written with a procedural approach, **the entire codebase was recently refactored into an object-oriented architecture**. This migration significantly improved the engine's readability, maintainability, and extensibility while faithfully recreating classic 8-bit game mechanics.

### Quick Preview
https://github.com/user-attachments/assets/886cc959-e684-48ce-a138-c66571f8e062


## 🎮 Engine Features & Architecture

* **Object-Oriented Architecture:** Transitioned from a procedural codebase to a modular OOP design, enabling easier implementation of new entities and game states.
* **Physics & Collision:** Custom AABB collision detection and frame-independent physics (gravity, acceleration) calculations.
* **State Management:** Robust entity state machines handling logic for walking, jumping, sliding, and power-up transitions.
* **Rendering:** Memory-efficient texture rendering and sprite management using SDL3.

## ⚙️ Prerequisites

To edit, build, and run this project, you will need the following libraries:
* **SDL3**
* **SDL3_image**

You must configure the exact paths for `SDL3` and `SDL3_image` within the required sections of the `CMakeLists.txt` file before building.

> **Note:** The mechanics and level structures are heavily inspired by classic 8-bit platformer games from the 1980s. Due to intellectual property regulations, no copyrighted assets (images, sounds) are included in this repository. The engine requires placeholder graphics in the `assets/` directory to run successfully.
