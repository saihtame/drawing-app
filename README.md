# Drawing App

A lightweight GPU-driven drawing application, built as a school project.

## Features

- Docked ImGui editor with multi-document management
- GPU layer compositing with double-buffered layers
- Brush and pan tools
- Layer panel: add, remove, reorder, and show/hide layers
- Native file dialogs for loading, saving, and exporting
- Project save/load to `.dproj` (a zip containing `project.json` plus one PNG per layer)
- Export the flattened canvas to PNG

## Getting Started

### Prerequisites

- CMake 3.20+
- C++20 compiler (the infrastructure target is built as C++23 because `glaze` requires it)
- GTK3 development headers for the native file dialogs on Linux (e.g. `libgtk-3-dev`)
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
| stb | PNG decoding (`stb_image`) and encoding (`stb_image_write`) |
| glaze | JSON metadata for project files |
| miniz | Zip archive reader/writer for `.dproj` |
| nativefiledialog-extended | Native open/save file dialogs |
| uuid_v4 | UUID generation |
| GTK3 (system) | Linux backend for nativefiledialog-extended |

## Architecture

The project follows Clean Architecture, using ports and adapters between layers:

| Layer | Responsibility |
| ----- | -------------- |
| Domain | Content model (`Document`, `Canvas`, `CanvasLayer`, `CanvasFactory`) |
| Application | `DocumentService` and port interfaces (`DocumentSerializer`) |
| Presentation | ImGui UI |
| Infrastructure | GPU and IO implementations |

Dependencies point inward: infrastructure implements the application's interfaces.
