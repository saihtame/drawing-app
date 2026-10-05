# AGENTS.md - Drawing App

## Project Overview

This is a lightweight GPU-driven drawing application built as a school project using C++20. It features a docked ImGui editor with multi-document management, layer-based rendering, and a custom GPU compositing pipeline. The canvas rendering, drawing tools, and save/load functionality are noted as in-progress.

### Tech Stack

- **Language**: C++20
- **Build System**: CMake 3.20+
- **Windowing & Input**: SDL3
- **Graphics API**: OpenGL 4.6 (Core Profile)
- **GUI**: ImGui (docking branch)
- **Math**: GLM
- **OpenGL Loader**: glad
- **Image I/O**: stb
- **UUID**: uuid_v4
- **Architecture**: Clean Architecture with ports/adapters

## Project Structure

The codebase follows Clean Architecture principles - dependencies point inward (Infrastructure → Application → Domain).

```
drawing-app/
├── build/                    # Build output (created by cmake)
├── cmake/                    # CMake modules
│   └── imgui.cmake           # ImGui build configuration
├── external/                 # Vendored dependencies (git submodules)
│   ├── glad/                 # OpenGL loader
│   ├── glm/                  # Math library
│   ├── imgui/                # ImGui (docking branch)
│   ├── sdl3/                 # SDL3
│   ├── stb/                  # stb_image
│   └── uuid_v4/              # UUID generation
├── shaders/                  # GPU shader programs
│   ├── compute/
│   │   └── texture_compositor.comp  # Layer compositing compute shader
│   └── visual/
│       ├── canvas.vert       # Canvas vertex shader
│       └── canvas.frag       # Canvas fragment shader
├── src/                      # Source code
│   ├── main.cpp              # Application entry point
│   ├── domain/               # Core business logic (no external deps)
│   ├── application/          # Use cases, interfaces/ports
│   ├── infrastructure/       # Implementations (GPU, IO)
│   └── presentation/         # ImGui UI, runtime
└── assets/                   # Static assets (copied to build/bin/assets)
```

## Core Architecture

### Domain Layer (`src/domain/`)

Pure business logic, no dependencies on other layers or framework libraries (only `uuid_v4` and the header-only `glm`, which is used by `Canvas`, `CanvasLayer` and `StrokeTool`).

**Documents** (`domain/documents/`):
- `Document`: Represents a drawing document. Owns a Canvas via factory, has UUID and optional save path.
- `Canvas` (abstract): Interface for canvas operations - rendering, layer management (add/remove/move), selection, zoom/pan utilities. 
- `CanvasLayer` (abstract): Interface for individual layers - visibility, fill, name, UUID.
- `CanvasFactory` (interface): Factory for creating Canvas instances.

**Tools** (`domain/tools/`):
- `Tool` (abstract base): Base tool with UUID, name, type (PAN, BRUSH), usage (CLICK, STROKE).
- `StrokeTool`: Extends Tool for drag-based operations with `start_stroke()`, `continue_stroke()`, `stop_stroke()`.
- `BrushTool`: Extends StrokeTool. Owns per-tool brush configuration (`BrushSettings`) and clamps it; subclasses supply the GPU-backed behaviour. Configuration lives in the domain because it is entity state, following the same pattern as `CanvasLayer::visible`/`fill` and `Canvas::set_zoom`.
- `BrushSettings`: Plain value struct (size, hardness, opacity, color) with defaults.

### Application Layer (`src/application/`)

Use cases and port interfaces. Depends on Domain.

**Documents**:
- `DocumentService`: Manages documents lifecycle (new/load/save/close), active document tracking, observer pattern for UI updates. Stores documents in unordered_map by UUID.
- `DocumentSerializer` (interface/port): Abstract serializer for document I/O.
- `DocumentServiceObserver`: Observer interface for document changes.

**Tools**:
- `Toolbar`: Manages available tools and tracks the active tool.

### Infrastructure Layer (`src/infrastructure/`)

Concrete implementations of Application ports and GPU functionality. Depends on Application and Domain.

**Canvas** (GPU implementation):
- `GpuCanvas`: Concrete Canvas using OpenGL textures. Composites all layers via compute shader into a final texture, renders via material/plane. Also owns which layer is selected.
- `GpuCanvasLayer`: Concrete CanvasLayer backed by **two** GPU textures (RGBA16F) and a front/back index, so an edit can read and write within one compute dispatch. See "Layer Double Buffering".
- `GpuCanvasFactory`: Creates GpuCanvas instances.

**Rendering** (OpenGL utilities):
- `Texture`: 2D texture (RGBA16F) with fill operation using `glClearTexImage`.
- `Shader`: Compiles individual GLSL shaders (vertex/fragment/compute).
- `ShaderProgram`: Links shaders, manages uniform setting, program usage.
- `Material`: Canvas rendering material - handles transform/projection matrices, zoom, texture binding.
- `Plane`: Fullscreen quad mesh for rendering canvases.

**Compute**:
- `TextureCompositor`: Compute shader-based layer compositor. Uses `texture_compositor.comp` with up to 32 source textures per pass, implements premultiplied alpha "over" blending. Dispatches workgroups of 16x16.
- `BrushPainter`: Wraps `brush_painter.comp`, painting a capsule (constant width segment) between two canvas-space points with hardness falloff and premultiplied-alpha "over" compositing. Reads one texture and writes another, so it must be given the layer's two buffers. Must be constructed lazily (first stroke, not at startup) because it owns a GL shader program.

**IO**:
- `DocumentFileSerializer`: Concrete implementation of DocumentSerializer. Currently stub implementation (load/save are not fully implemented - returns empty doc or does nothing).

**Tools**:
- `PanTool`: Stroke tool implementation for panning (currently just logs).
- `BrushTool`: Derives from `Domain::Tools::BrushTool`, adding a lazily-created `BrushPainter`. Holds the painter behind a forward declaration + `unique_ptr` so `<glad/gl.h>` never reaches `main.cpp`.

### Presentation Layer (`src/presentation/`)

ImGui UI and runtime. Handles windowing, input, and rendering the editor interface.

**Core**:
- `EditorRuntime`: Main application loop - initializes SDL3 window + OpenGL 4.6 context, runs event/render loop, manages UI lifecycle.
- `UI`: Root ImGui UI. Sets up ImGui context with docking, style (Catppuccin Mocha theme), handles events, manages layout, observes DocumentService.
- `MenuBar`: Main menu (File/Edit/View etc.)
- `LayersPanel`: UI for layer management

**Canvas Panels**:
- `CanvasPanel`: Renders a document's canvas in a docked window. Handles fit-to-screen on first appearance, updates active document on focus.
- `CanvasPanelService`: Manages multiple canvas panels.

**Input**:
- `InputManager`: Routes input events to tools/canvas interactions.

**Popups**:
- `PopupManager`, `NewFilePopup`: Dialog management.

**Toolbar**:
- `ToolbarPanel`: UI for selecting the active tool, plus a settings section rendered for whichever tool exposes options (currently the brush).

## Build & Development

### Prerequisites
- CMake 3.20+
- C++20 compiler (GCC/Clang)
- Dependencies are vendored in `external/` as git submodules

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

Post-build steps copy `shaders/` and `assets/` to `build/bin/`.

### Build Configuration Notes
- SDL3 built as static library (`SDL_SHARED OFF`, `SDL_STATIC ON`)
- OpenGL preference set to GLVND
- `CMAKE_EXPORT_COMPILE_COMMANDS ON` for LSP/IDE integration
- Compile warnings enabled (`-Wall -Wextra -Wpedantic`) for GCC/Clang - but only on the executable target, not on the layer libraries

## Key Design Patterns

- **Clean Architecture**: Strict layer separation with inward dependencies
- **Observer Pattern**: `DocumentServiceObserver` for UI updates on document changes
- **Factory Pattern**: `CanvasFactory`/`GpuCanvasFactory` for canvas creation
- **Interface Segregation**: Abstract base classes for Domain entities (`Canvas`, `CanvasLayer`, `Tool`)
- **Dependency Injection**: Constructor injection throughout (see `main.cpp`)

## Code Style & Conventions

- **Namespaces**: Nested namespaces (`Domain::Documents`, `Infrastructure::Canvas`, etc.)
- **Headers**: `.hpp`, Implementation: `.cpp`
- **Include guards**: `#pragma once`
- **Naming**: `snake_case` for files, classes use `PascalCase`, methods/variables `snake_case`
- **Const correctness**: Used where appropriate (`const`, `noexcept`, `[[nodiscard]]`)
- **Smart pointers**: Extensive use of `shared_ptr`/`weak_ptr` for ownership/lifecycle management
- **Member order**: public/private sections organized (ctors, methods, data)
- **Forward declarations**: Used to minimize includes

## Important Implementation Details

### Layer Compositing
The compute shader (`texture_compositor.comp`) composites layers using premultiplied alpha blending in order (as provided in `sources` array). It supports up to 32 layers per dispatch; exceeding this throws an exception (TODO notes multiple passes needed).

### Canvas Rendering
1. GpuCanvas collects all layer textures
2. Calls `compositor.composit()` to blend into final texture
3. Renders final texture to screen via `Material` + `Plane` with transform/projection

### OpenGL Include Path Invariant
`glad` is linked `PRIVATE` by the `drawing_app_infrastructure` and `drawing_app_presentation` targets. That is only sound as long as **no header reachable from `main.cpp` includes `<glad/gl.h>`**. Several infrastructure headers do include it transitively (`shader.hpp`, `texture.hpp`, `material.hpp`, `plane.hpp`, `texture_compositor.hpp`, `brush_painter.hpp`), but they are kept out of `main.cpp`'s include tree by keeping the public headers `main.cpp` actually includes thin:

- `gpu_canvas_factory.hpp` includes only the domain `canvas_factory.hpp`; `create_canvas()` returns the abstract `shared_ptr<Canvas>`, so `main.cpp` never names `GpuCanvas`.
- `tools/brush_tool.hpp` forward-declares `Compute::BrushPainter` and holds it in a `std::unique_ptr` with an out-of-line destructor, confining the GL-heavy `brush_painter.hpp` to `brush_tool.cpp`.

Violating this does not fail inside the library - the library builds fine and the failure surfaces in `main.cpp` as `fatal error: glad/gl.h: No such file or directory`, which points several files away from the actual mistake. When adding an infrastructure member that owns GL resources, hold it behind a forward declaration.

### OpenGL Context Ordering
`gladLoadGL` is called from `EditorRuntime::initialize_window()`, which runs inside `EditorRuntime::run()`. Until then every glad dispatch pointer is `NULL`, so **any GL call segfaults at address 0**. `main()` constructs the tools - and therefore everything they own - before calling `run()`, so GPU-owning types must be created lazily, on first use inside the event loop. `TextureCompositor` gets away with eager construction only because it is owned by `GpuCanvas`, which is created lazily on the first `new_document()` call; that is an accident of ownership, not a safety guarantee.

### Layer Double Buffering
`GpuCanvasLayer` owns two `Rendering::Texture` buffers and an index, because GL cannot bind one texture as both a read and a write image in a single dispatch (there is no `GL_READ_WRITE` for image units; binding one texture to two roles is a feedback loop with undefined results). The convention for anything that edits layer pixels:

1. Read from `layer->get_read_texture()` (the front buffer)
2. Write into `layer->get_write_texture()` (the back buffer)
3. Issue a memory barrier
4. Call `layer->switch_buffers()` to promote the back buffer

Skipping step 4 leaves the edit invisible until the next one. `fill()` must write **both** buffers, or the stale one resurfaces on the first switch. `get_texture()` returns the front buffer, which is what `GpuCanvas::render()` composites. Switching is an index flip rather than a swap because `Texture` owns a GL name and has a user-declared destructor, so it is neither copyable nor movable in practice.

`BrushPainter::paint()` deliberately takes two `Rendering::Texture&` rather than a `GpuCanvasLayer&`, so `infrastructure/compute` does not have to depend on `infrastructure/canvas`. The cost is that callers must remember steps 3 and 4.

### Layer Selection
`GpuCanvas::add_layer()` selects the layer it creates, so a new document's initial white layer (`DocumentService::new_document`) is selected immediately and there is always a paint target. Clicking a row in `LayersPanel` calls `set_selected_layer`. Tools resolve the surface once at `start_stroke`, since the selection cannot change mid-stroke.

### Texture Format
All GPU textures use `GL_RGBA16F` for HDR-like precision in the internal format.

### Document Lifecycle
- Documents created via `DocumentService::new_document()` - automatically creates initial white layer
- UUIDs generated using `uuid_v4` library (mt19937_64 generator)
- Documents stored in `unordered_map<UUIDv4::UUID, shared_ptr<Document>>`

### UI Layout
- ImGui docking enabled with custom transparent dockspace background
- Default layout splits left (Tools), center (Canvas panels), right (Layers)
- Canvas panels auto-dock to central area on creation

## Development Notes

- Save/load is stubbed out in `DocumentFileSerializer` - not yet implemented
- Brush and Pan tools currently only log positions (no actual drawing/panning logic)
- StrokeTool base tracks `in_stroke` state but doesn't implement logic
- When creating new documents, first layer is filled white
- Debug mode enables OpenGL debug callback
- ImGui ini file is disabled (`io.IniFilename = nullptr`)

## Testing & Debugging

- Debug builds define `_DEBUG` - enables extra logging and GL debug output
- OpenGL errors go through debug callback in `EditorRuntime`
- Compile commands available at `compile_commands.json` (generated by CMake)

## Common Tasks for Agents

When working with this codebase:
1. Respect Clean Architecture boundaries - don't add Infrastructure deps in Domain; Domain should stay free of external framework deps (`uuid_v4` and `glm` are the only permitted ones)
2. Use existing patterns (observer, factory, interfaces, DI via constructors)
3. For GPU operations, follow patterns in `infrastructure/rendering/` and `infrastructure/compute/` (RAII, texture lifecycle)
4. For UI changes, work within ImGui docking paradigm; avoid breaking dockspace layout
5. Maintain const-correctness (`const`, `noexcept`, `[[nodiscard]]`) and modern C++20 style
6. When modifying shaders, ensure they're copied to build output (post-build copies shaders/ to bin)
7. Use the existing DI pattern in `main.cpp` for wiring components
8. Keep includes minimal; use forward declarations where appropriate
9. Follow existing naming: PascalCase for classes, snake_case for files/methods/vars
10. Prefer `std::weak_ptr` for non-owning references (e.g. observers, canvas references from Document)

## File Locations Quick Reference

| Component | Path |
|---|---|
| Entry point | `src/main.cpp` |
| Domain interfaces | `src/domain/documents/*.hpp`, `src/domain/tools/*.hpp` |
| Tools | `src/domain/tools/*.hpp,cpp`, `src/infrastructure/tools/*.hpp,cpp` |
| App services | `src/application/documents/document_service.*`, `src/application/tools/toolbar.*` |
| GPU canvas | `src/infrastructure/canvas/gpu_canvas.*`, `gpu_canvas_layer.*` |
| Compositor | `src/infrastructure/compute/texture_compositor.*` |
| Brush painter | `src/infrastructure/compute/brush_painter.*` |
| Rendering | `src/infrastructure/rendering/*.hpp,cpp` |
| UI root | `src/presentation/ui.*`, `src/presentation/editor_runtime.*` |
| Canvas UI | `src/presentation/canvas_panels/*.hpp,cpp` |
| Shaders | `shaders/compute/texture_compositor.comp`, `shaders/compute/brush_painter.comp`, `shaders/visual/canvas.*` |
| Build config | `CMakeLists.txt`, `src/*/CMakeLists.txt`, `cmake/imgui.cmake` |
| External deps | `external/` (submodules) |