# Drawing App

A lightweight GPU-driven drawing application, built as a school project.

## Features

- Docked ImGui editor with multi-document management
- New-file dialog
- Layer-based domain model

> Canvas rendering, drawing tools, and save/load are in progress.

## Getting Started

### Prerequisites

- CMake 3.20+
- C++20 compiler
- Dependencies live in `external/` as git submodules or vendored copies

### Building

```bash
git submodule update --init --recursive
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

### Running

```bash
./build/bin/drawing_app
```

## Dependencies

| Dependency | Purpose |
| ---------- | ------- |
| SDL3 / OpenGL | Windowing and rendering |
| ImGui | User interface |
| glm | Math |
| glad | OpenGL loader |
| stb | Image loading/writing |

## Architecture

The project follows Clean Architecture, using ports and adapters between layers:

| Layer | Responsibility |
| ----- | -------------- |
| Domain | Content model (`Document`, `Layer`) |
| Application | `DocumentService` and port interfaces (`DocumentSerializer`, `DocumentCanvas`) |
| Presentation | ImGui UI |
| Infrastructure | GPU and IO implementations |

Dependencies point inward: infrastructure implements the application's interfaces.