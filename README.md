# 2D Platformer Engine built with C++ and SDL3

This project is a custom-built 2D platformer engine developed from scratch using C++ and SDL3. It demonstrates core game engine mechanics such as physics, rigid collision detection, entity state machines, and dynamic rendering.

### Video Demonstration
[![Watch the Demo](https://img.shields.io/badge/YouTube-Watch%20Demo-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=zmvuWbD1urQ)

### Quick Preview
<img width="640" height="600" alt="ezgif-30c199806ebf3800" src="https://github.com/user-attachments/assets/50957eaf-c1e3-4c42-a7e8-24ec7592bf32" />

## 🎮 Features

* Custom AABB collision detection.
* Frame-independent physics and gravity calculations.
* Entity states (walking, jumping, sliding, power-ups).
* Memory-efficient texture rendering using SDL3.

## ⚙️ Prerequisites

To edit, build, and run this project, you will need the following libraries:
* **SDL3**
* **SDL3_image**

You must configure the exact paths for `SDL3` and `SDL3_image` within the required sections of the `CMakeLists.txt` file before building.

> **Note:** The mechanics and level structures are heavily inspired by classic 8-bit platformer games from the 1980s. Due to intellectual property regulations, no copyrighted assets (images, sounds) are included in this repository. The engine requires placeholder graphics in the `assets/` directory to run successfully.