# AndEngine

![Release](https://img.shields.io/github/v/release/xImDoctor/AndEngine)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)
![Language](https://img.shields.io/badge/language-C%2B%2B17-orange)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3_Core-green)


This is a lightweight `2.5D` (*pseudo-3D*) game engine inspired by really old (classic!) games like Doom. The engine uses raycasting techniques to create pseudo-3D environments from 2D maps, providing an authentic retro gaming experience.

Current stage:
- Console Renderer:
![Console Renderer Preview](demo-img/MapRenderer.gif)
- GL Renderer:
![Graphics Renderer Preview](demo-img/GLRenderer.gif)


## Сontent
The brief of document links
1. [Controls and Quick Start](#-controls-in-current-realization)
2. [Current Game Feautures](#current-status-and-features)
    - [Completed Features and TechStack](#-completed-features)
    - [Previous WinAPI projects](#another-previous-winapi-projects-you-can-open-from-here)
3. [Feature Plans (roadmap)](#-roadmap)
4. [Technical Details (realization)](#-technical-details)
5. [Goals](#-goals)
6. [Contributing](#-contributing)

---

## 🎮 Controls in current realization
- `W/S` - Move forward/backward
- `A/D` - Rotate left/right
- `ESC` - Exit (stop rendering)

---

## Quick Start

### Launch only (releases)

If you only want to launch application (Engine render test), you can just download current build as `.zip`-archive from releases, unpack it and use directly.

### Code and build from scratch

Built with `Visual Studio 2022`. Clone or download the repository and build for `Windows`.

**Dependencies** (via [vcpkg](https://github.com/microsoft/vcpkg)):
```
vcpkg install glfw3:x64-windows glad:x64-windows
vcpkg integrate install
```

> Actual dependecies are also located at [vcpkg.json](vcpkg.json) of this repository.

Then launch `AndEngine.exe` and enjoy!

### After launch

After starting of `AndEngine.exe`, type one of the CLI commands to proceed:

| Command | Description |
|---|---|
| `start raycast` | Console renderer, step-based raycast |
| `start dda` | Console renderer, DDA algorithm (faster) |
| `use console` | Switch to console renderer (default) |
| `use gl` | Switch to OpenGL renderer |
| `change seed` | Change map generation seed |
| `show map` | Preview the generated map |
| `info` | Controls reference |
| `exit` | Quit |

> The commands are also fully described in the app.

---

## Current status and features

### ✅ Completed Features
- **Dual Renderer Architecture**: switchable at runtime between two backends
  - **Console renderer** — ASCII art, distance-based wall shading, fish-eye correction, minimap
  - **OpenGL renderer** — GLFW window, GLAD, OpenGL 3.3 Core Profile, per-column GL_LINES
- **Raycasting Engine**: decoupled geometry module used by both renderers
  - Step-based algorithm
  - DDA algorithm (should be faster than step-based one)
  - Precomputed sin/cos lookup tables (3600 points, 0.1 degree precision)
- **Delta Time system**: frame-rate independent movement, 60 FPS lock (console), VSync (OpenGL)
- **Input System**: `GetAsyncKeyState`, tracks key pressed/held/released states
- **Map System**: procedural 16x16 grid generation with the seed configurable by user, 30% wall density
- **Mini-map**: real-time overhead view for debugging and navigation (console renderer)

### 🔧 Technical Stack
- **Language**: C++17
- **Platform**: Windows (Win32 API, Visual Studio 2022)
- **Console renderer**: WinAPI console output
- **OpenGL renderer**: GLFW 3 + GLAD, OpenGL 3.3 Core Profile

#### Another (*previous*) `WinAPI` projects you can open from here:

[![SnakeConsoleGame](https://github-readme-stats.vercel.app/api/pin/?username=xImDoctor&repo=SnakeConsoleGame&theme=radical)](https://github.com/xImDoctor/SnakeConsoleGame)

[![GuessTheWordGame](https://github-readme-stats.vercel.app/api/pin/?username=xImDoctor&repo=GuessTheWordGame&theme=radical)](https://github.com/xImDoctor/GuessTheWordGame)

[![WindowsConsoleColor](https://github-readme-stats.vercel.app/api/pin/?username=xImDoctor&repo=WindowsConsoleColor&theme=radical)](https://github.com/xImDoctor/WindowsConsoleColor)

---

## 📋 Roadmap
> This is a rough draft of the plan, the stages can change and be adjusted depending on the work done or planned.

> Upd: you can understand phases as a different project sides. Their realization can be parallel or asynchronous.

### Phase 1: Core Engine ✅
- [x] Raycasting renderer (step-based + DDA)
- [x] Basic player movement with delta time
- [x] Procedural map generation with seed
- [x] Precomputed trig tables
- [x] Fish-eye distortion correction
- [x] Basic collision detection (*wall collision*)

### Phase 2: Graphics *(in progress)*
- [x] OpenGL window (GLFW + GLAD)
- [x] Dual renderer architecture (IRenderer interface)
- [x] Basic OpenGL raycasting output
- [ ] Texture mapping for walls
- [ ] Enhanced map structure with tile properties
- [ ] Sprite rendering
- [ ] Lighting system
- [ ] Particle effects

### Phase 3: Game Objects
- [ ] Entity system
- [ ] Player class with health, ammo
- [ ] Primitive enemy AI
- [ ] Weapon system
- [ ] Pickups
- [ ] Advanced collision with all stuff

### Phase 4: Audio & Effects
- [ ] Sound effects and music
- [ ] Visual & screen effects

### Phase 5: Level System
- [ ] Level loader/editor
- [ ] Multiple floor heights
- [ ] Interactive objects (doors, switches)
- [ ] Scripting system

### Phase 6: Shooter Implementation
- [ ] Complete Doom-like shooter game
- [ ] Multiple weapon types
- [ ] Enemy variants
- [ ] Power-ups and secrets
- [ ] HUD and UI system

---

## 📚 Technical Details

### Architecture

```
Engine
├── RaycastEngine       – ray geometry (DDA, stepped, trig tables)
├── ConsoleRenderer     – ASCII output, screenBuffer, minimap
├── GLRenderer          – OpenGL output, VAO/VBO, shaders
├── InputController     – key state tracking (pressed/held/released)
└── MapGenerator        – procedural map generation
```

Both renderers implement `IRenderer` interface and share `RaycastEngine` for geometry (Raycasting algorithms have moved here). Switching renderers at runtime does not affect game logic.

### Raycasting Algorithms

#### Step-based
1. Cast rays from player position across the FOV
2. Step along each ray (0.01 unit increments) until hitting a wall
3. Calculate wall height from distance
4. Apply fish-eye correction (`distance *= cos(rayAngle - playerAngle)`)
5. Render column with distance-based shading

#### DDA (Digital Differential Analysis)
Optimized algorithm that jumps directly from grid cell to grid cell: guaranteed to visit every cell in the ray path exactly once. Approximately *x10* faster than step-based.

---

## 🎯 Goals
- **Educational**: Learn game engine development from scratch with `C/C++`
- **Retro**: Recreate the magic of classic pseudo-3D games
- **Modular**: Build reusable engine components
- **Fun**: Create an enjoyable shooter experience!!!!!!1!!!1!

## 🤝 Contributing
This is currently just my personal learning project, but suggestions and feedback are welcome!

---

*"In the beginning, there was darkness. Then someone cast a ray."* 🔦
