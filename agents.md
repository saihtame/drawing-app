# AGENTS.md - Drawing App

## Project Overview

This is a lightweight GPU-driven drawing application built as a school project using C++20. It features a docked ImGui editor with multi-document management, layer-based rendering, a custom GPU compositing pipeline, project save/load (`.dproj`), and flattened PNG export.

### Tech Stack

- **Language**: C++20 (the `drawing_app_infrastructure` target is compiled as C++23 because `glaze` advertises `cxx_std_23`; the other targets stay on C++20)
- **Build System**: CMake 3.20+
- **Windowing & Input**: SDL3
- **Graphics API**: OpenGL 4.6 (Core Profile)
- **GUI**: ImGui (docking branch)
- **Math**: GLM
- **OpenGL Loader**: glad
- **Image I/O**: stb (`stb_image` for decode, `stb_image_write` for PNG encode)
- **Archives / Compression**: miniz (zip reader/writer for `.dproj`)
- **JSON**: glaze (project metadata in `project.json`)
- **File Dialogs**: nativefiledialog-extended (NFD, GTK backend on Linux)
- **UUID**: uuid_v4
- **Architecture**: Clean Architecture with ports/adapters

## Project Structure

The codebase follows Clean Architecture principles - dependencies point inward (Infrastructure → Application → Domain).

```
drawing-app/
├── build/                    # Build output (created by cmake)
├── cmake/                    # CMake modules
│   └── imgui.cmake           # ImGui build configuration
├── external/                 # Vendored dependencies (mostly submodules)
│   ├── glad/                 # OpenGL loader
│   ├── glaze/                # JSON serialization (C++23)
│   ├── glm/                  # Math library
│   ├── imgui/                # ImGui (docking branch)
│   ├── miniz/                # Zip archive reader/writer
│   ├── nativefiledialog-extended/  # Native file dialogs (GTK on Linux)
│   ├── sdl3/                 # SDL3
│   ├── stb/                  # stb_image + stb_image_write
│   └── uuid_v4/              # UUID generation
├── shaders/                  # GPU shader programs
│   ├── compute/
│   │   ├── brush_painter.comp        # Brush capsule painting
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
- `Document`: Represents a drawing document. Owns a Canvas via factory, has UUID and an optional save path (`get_save_path()` / `set_save_path()`). `get_canvas()` returns a `Canvas&`: the canvas is created in the constructor and never reset.
- `Canvas` (abstract): Interface for canvas operations - rendering, layer management (add/remove/move), selection, zoom/pan utilities. `add_layer(name)` takes an optional display name, and `read_composited_pixels()` returns the flattened visible layers as packed RGBA8 for export. 
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
- `DocumentService`: Manages documents lifecycle (new/load/save/export/close), active document tracking, observer pattern for UI updates. Stores documents in unordered_map by UUID. Loading a document also makes it the active document. Save/export take a `Document&` because the calls are synchronous.
- `DocumentSerializer` (interface/port): Abstract serializer for document I/O - `load_document`, `save_document(Document&)`, `export_png(Document&)`.
- `DocumentServiceObserver`: Observer interface for document changes.

**Tools**:
- `Toolbar`: Manages available tools and tracks the active tool.

### Infrastructure Layer (`src/infrastructure/`)

Concrete implementations of Application ports and GPU functionality. Depends on Application and Domain.

**Canvas** (GPU implementation):
- `GpuCanvas`: Concrete Canvas using OpenGL textures. `composite_layers()` blends **visible** layers via the compute shader into a final texture, which `render()` then draws through material/plane. `read_composited_pixels()` recomposites and reads the result back as RGBA8. Also owns which layer is selected.
- `GpuCanvasLayer`: Concrete CanvasLayer backed by **two** GPU textures (RGBA16F) and a front/back index, so an edit can read and write within one compute dispatch. See "Layer Double Buffering". `upload_pixels()` replaces both buffers from packed RGBA8 (used by deserialization).
- `GpuCanvasFactory`: Creates GpuCanvas instances.

**Rendering** (OpenGL utilities):
- `Texture`: 2D texture (RGBA16F) with fill operation using `glClearTexImage`. **Owns a GL name; copying it copies the name and double-deletes.** See "Texture Ownership".
- `Shader`: Compiles individual GLSL shaders (vertex/fragment/compute).
- `ShaderProgram`: Links shaders, manages uniform setting, program usage.
- `Material`: Canvas rendering material - handles transform/projection matrices, zoom, texture binding.
- `Plane`: Fullscreen quad mesh for rendering canvases.

**Compute**:
- `TextureCompositor`: Compute shader-based layer compositor. Uses `texture_compositor.comp` with up to 32 source textures per pass, implements premultiplied alpha "over" blending. Dispatches workgroups of 16x16.
- `BrushPainter`: Wraps `brush_painter.comp`, painting a capsule (constant width segment) between two canvas-space points with hardness falloff and premultiplied-alpha "over" compositing. Reads one texture and writes another, so it must be given the layer's two buffers. Must be constructed lazily (first stroke, not at startup) because it owns a GL shader program.

**IO**:
- `DocumentFileSerializer`: Concrete `DocumentSerializer` implementing project save/load and PNG export.
  - **Save**: creates a zip with miniz, reads each layer's front texture back as RGBA8 with `glGetTextureImage`, encodes PNGs with `stb_image_write`, writes `layer_<i>.png` entries and a final `project.json`.
  - **Load**: opens the zip with miniz, parses `project.json` with glaze, recreates the `Document` at the stored size, and for each layer entry decodes the PNG with `stb_image` and uploads it via `GpuCanvasLayer::upload_pixels()`. Layer order and names are preserved; any bad entry fails the whole load (returns `nullptr`).
  - **Export**: flattens visible layers through `Canvas::read_composited_pixels()`, converts the compositor's premultiplied alpha to straight alpha, and writes a single PNG.
  - `ProjectJsonData` / `LayerJsonData` (`project_json_data.hpp`) define the metadata schema: canvas size plus an ordered `layers[]` array of `{name, file}`.
  - Uses RAII guards for `mz_zip_reader_end` / `mz_zip_writer_end`, and `mz_zip_archive{}` must always be value-initialized.

**Tools**:
- `PanTool`: Stroke tool that moves the active canvas's rendering offset.
- `BrushTool`: Derives from `Domain::Tools::BrushTool`, adding a lazily-created `BrushPainter`. Holds the painter behind a forward declaration + `unique_ptr` so `<glad/gl.h>` never reaches `main.cpp`.

### Presentation Layer (`src/presentation/`)

ImGui UI and runtime. Handles windowing, input, and rendering the editor interface.

**Core**:
- `EditorRuntime`: Main application loop - initializes SDL3 window + OpenGL 4.6 context, runs event/render loop, manages UI lifecycle.
- `UI`: Root ImGui UI. Sets up ImGui context with docking, style (Catppuccin Mocha theme), handles events, manages layout, observes DocumentService.
- `MenuBar`: Main menu (File/View). File menu covers New, Load, Save, Save As, and Export as PNG, all routed through native file dialogs (NFD). See "File Dialogs".
- `LayersPanel`: UI for layer management (add/remove/reorder, visibility toggle). Visibility now affects both the canvas and exports.

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
- GTK3 development headers for the NFD file dialogs on Linux (e.g. `libgtk-3-dev`)
- Dependencies are vendored in `external/` (mostly git submodules; `stb` is vendored directly)

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
- The root sets C++20, but `glaze` contributes `cxx_std_23`, so anything linking the infrastructure target (and therefore glaze) is compiled as C++23
- `stb_image_write.h` is vendored directly in `external/stb/`; both `STB_IMAGE_IMPLEMENTATION` and `STB_IMAGE_WRITE_IMPLEMENTATION` are defined in `external/stb/stb_image.cpp`

## Key Design Patterns

- **Clean Architecture**: Strict layer separation with inward dependencies
- **Observer Pattern**: `DocumentServiceObserver` for UI updates on document changes
- **Factory Pattern**: `CanvasFactory`/`GpuCanvasFactory` for canvas creation
- **Interface Segregation**: Abstract base classes for Domain entities (`Canvas`, `CanvasLayer`, `Tool`)
- **Dependency Injection**: Constructor injection throughout (see `main.cpp`)

### Ownership & Pointer Policy

The app is single-threaded and ownership is a strict tree, so most handlers should not be `weak_ptr`:

- **Owning handles**: use `unique_ptr` for exclusive ownership and `shared_ptr` only where ownership is genuinely shared (documents in `DocumentService`, layers in a canvas, panels in `CanvasPanelService`).
- **Non-owning handles**: prefer `T&` when the object is guaranteed to exist for the duration of the call (a parent owns its children; `Document::get_canvas()` can never fail). Use `T*` only when the reference is nullable, and document the lifetime invariant.
- **`weak_ptr`**: reserve it for relationships where expiration is a real state the code must handle:
  - observer lists (`DocumentService::observers`);
  - aliases into collections whose elements can be removed (`GpuCanvas::selected_layer`, `CanvasPanelService::visible_panel`);
  - back-references across objects with independent lifetimes (e.g. `CanvasPanel::document`).
- Do not put `weak_ptr` in function parameters when the caller owns the object and the call is synchronous - the caller already holds a strong reference. Save/export use `Document&` for this reason.
- Every `.lock()` implies a "maybe missing" branch. If the object cannot be missing by construction, a reference expresses that better than a null check nobody can trigger.

### Concurrency & Async IO (planned)

File dialogs block the render loop today (the OS may report the app as unresponsive while a dialog is open). The intended direction is to run dialogs and file/encoding work off the render thread. When doing that:

- **GPU work stays on the render thread.** Compositing and `glGetTextureImage`/`glTextureSubImage2D` require the GL context to be current. Async save/export should take a CPU snapshot of the pixels on the render thread, then encode/write on the worker.
- **Jobs must own what they use.** Capturing `T&`/`T*` in a deferred task is unsafe once document/layer removal can interleave. Capture an owning `shared_ptr` for the job's lifetime, or a value snapshot; `weak_ptr` is appropriate only to detect "document was closed" (lock it once and hold the result).
- **NFD/GTK is thread-affine.** Initialize and use a dialog on the same thread (NFD's GTK backend expects all GTK calls on one thread); do not call `NFD_Init`/dialog functions from multiple threads.

## Code Style & Conventions

- **Namespaces**: Nested namespaces (`Domain::Documents`, `Infrastructure::Canvas`, etc.)
- **Headers**: `.hpp`, Implementation: `.cpp`
- **Include guards**: `#pragma once`
- **Naming**: `snake_case` for files, classes use `PascalCase`, methods/variables `snake_case`
- **Const correctness**: Used where appropriate (`const`, `noexcept`, `[[nodiscard]]`)
- **Smart pointers**: `unique_ptr`/`shared_ptr` for ownership, `T&`/`T*` for non-owning references, `weak_ptr` only for observers and removable/optional aliases (see "Ownership & Pointer Policy")
- **Member order**: public/private sections organized (ctors, methods, data)
- **Forward declarations**: Used to minimize includes

## Important Implementation Details

### Layer Compositing
`GpuCanvas::composite_layers()` collects only layers where `is_visible()` is true and composites them using premultiplied alpha blending in order (as provided in `sources` array). Both `render()` and `read_composited_pixels()` go through it, so hidden layers are absent from the screen and from exports. It supports up to 32 layers per dispatch; exceeding this throws an exception (TODO notes multiple passes needed).

### Canvas Rendering
1. `GpuCanvas::composite_layers()` collects visible layer textures
2. Calls `compositor.composit()` to blend into the final canvas texture
3. Renders the final texture to screen via `Material` + `Plane` with transform/projection
4. `GpuCanvas::read_composited_pixels()` uses the same compositing step before reading back RGBA8 for export

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

Skipping step 4 leaves the edit invisible until the next one. `fill()` must write **both** buffers, or the stale one resurfaces on the first switch; `upload_pixels()` does the same when loading a layer. `get_texture()` returns the front buffer, which is what `GpuCanvas::render()` composites.

Switching is an index flip rather than a swap because `Texture` owns a GL name and has a user-declared destructor, which suppresses implicit moves. It is **not** actually non-copyable: the compiler still generates a copy constructor, and a copy duplicates `texture_id`. See "Texture Ownership" below.

`BrushPainter::paint()` deliberately takes two `Rendering::Texture&` rather than a `GpuCanvasLayer&`, so `infrastructure/compute` does not have to depend on `infrastructure/canvas`. The cost is that callers must remember steps 3 and 4.

### Texture Ownership
`Rendering::Texture` owns a GL texture name and deletes it in its destructor. Do **not** copy it: accessors like `GpuCanvasLayer::get_texture()` return a reference, so bind `auto&`/`Rendering::Texture&`, never `auto`. Copying creates a second owner of the same name, and when the copy dies its destructor deletes the layer's live texture - the canvas then composites black and binds report `GL_INVALID_OPERATION ... is not ... an existing texture object`. This exact bug appeared in the first version of `save_document`. If a new type stores a `Texture` member, either hold it in place or take it by reference; do not pass it by value.

### Layer Selection
`GpuCanvas::add_layer()` selects the layer it creates, so a new document's initial white layer (`DocumentService::new_document`) is selected immediately and there is always a paint target. Clicking a row in `LayersPanel` calls `set_selected_layer`. Tools resolve the surface once at `start_stroke`, since the selection cannot change mid-stroke.

### Texture Format
All GPU textures use `GL_RGBA16F` for HDR-like precision in the internal format. Persistence crosses the boundary as tightly packed RGBA8: save reads back with `glGetTextureImage(..., GL_RGBA, GL_UNSIGNED_BYTE)`, load uploads with `glTextureSubImage2D(..., GL_RGBA, GL_UNSIGNED_BYTE)`, and export writes RGBA8 PNG. Set `GL_PACK_ALIGNMENT`/`GL_UNPACK_ALIGNMENT` to 1 because rows are tightly packed.

### Project File Format (`.dproj`)
A `.dproj` is a zip archive (miniz) containing:

```
project.json        # metadata, written last but read by name
layer_0.png         # one entry per layer, in layer order
layer_1.png
...
```

`project.json` (see `ProjectJsonData`):

```json
{
  "size_width": 1920,
  "size_height": 1080,
  "layers": [
    { "name": "Background", "file": "layer_0.png" },
    { "name": "Ink", "file": "layer_1.png" }
  ]
}
```

- Layer PNGs store the layer texture contents **premultiplied**, matching the compositor's internal space; deserialization uploads them verbatim.
- `layers[]` order is the canvas layer order, and `file` must match an archive entry.
- Loading validates the canvas size and every layer entry; a missing/corrupt entry or a size mismatch fails the whole load and returns `nullptr`.
- Export writes a standalone PNG instead, with premultiplied alpha converted back to straight alpha for external viewers.

### File Dialogs (NFD)
All File-menu dialogs live in `MenuBar` and follow the same shape: `NFD_Init()` immediately before the dialog, `NFD_Quit()` on every exit path, then copy `out_path` and release it with `NFD_FreePathU8()`.

- `NFD_OKAY` can come back with `out_path == nullptr` (GTK returns no local filename for virtual locations such as Recent/Search). Always null-check before constructing a `std::filesystem::path`. This caused a real crash in export.
- Export forces the `.png` extension and opens in the document's folder (or cwd) via `defaultPath`, so the chooser starts in a real directory.
- Save/load use the `.dproj` filter; export uses `PNG Image (*.png)` with `defaultName = "untitled.png"`.

### Document Lifecycle
- Documents created via `DocumentService::new_document()` - automatically creates initial white layer
- Loaded documents restore their canvas size, layer names/order, and pixel contents; the loader also sets the document's save path and makes it the active document
- UUIDs generated using `uuid_v4` library (mt19937_64 generator)
- Documents stored in `unordered_map<UUIDv4::UUID, shared_ptr<Document>>`

### UI Layout
- ImGui docking enabled with custom transparent dockspace background
- Default layout splits left (Tools), center (Canvas panels), right (Layers)
- Canvas panels auto-dock to central area on creation

## Development Notes

- Project save/load/export is implemented in `DocumentFileSerializer`; `.dproj` is a zip of `project.json` + `layer_*.png`
- Save As records the chosen path on the document, so subsequent Save reuses it without prompting
- Brush and Pan tools are implemented: the brush paints via `brush_painter.comp`, panning moves the canvas rendering offset
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
10. Use `T&` for non-owning references with guaranteed lifetime and `T*` for nullable ones; reserve `weak_ptr` for observers and removable aliases (see "Ownership & Pointer Policy")
11. Never copy a `Rendering::Texture`: bind accessor results by reference (`auto&`), since copies share and double-delete the GL name
12. Value-initialize miniz archives (`mz_zip_archive archive{}`) and wrap reader/writer lifetime in a guard so `mz_zip_*_end` always runs
13. Null-check NFD dialog output before converting it to a path; `NFD_OKAY` does not guarantee a non-null filename
14. Persisted pixel data is RGBA8 and premultiplied for layers; export converts to straight alpha. Keep readback/upload alignment at 1

## File Locations Quick Reference

| Component | Path |
|---|---|
| Entry point | `src/main.cpp` |
| Domain interfaces | `src/domain/documents/*.hpp`, `src/domain/tools/*.hpp` |
| Tools | `src/domain/tools/*.hpp,cpp`, `src/infrastructure/tools/*.hpp,cpp` |
| App services | `src/application/documents/document_service.*`, `src/application/tools/toolbar.*` |
| Document IO | `src/infrastructure/io/document_file_serializer.*`, `project_json_data.hpp` |
| File dialogs | `src/presentation/menu_bar.*` (NFD) |
| GPU canvas | `src/infrastructure/canvas/gpu_canvas.*`, `gpu_canvas_layer.*` |
| Compositor | `src/infrastructure/compute/texture_compositor.*` |
| Brush painter | `src/infrastructure/compute/brush_painter.*` |
| Rendering | `src/infrastructure/rendering/*.hpp,cpp` |
| UI root | `src/presentation/ui.*`, `src/presentation/editor_runtime.*` |
| Canvas UI | `src/presentation/canvas_panels/*.hpp,cpp` |
| Shaders | `shaders/compute/texture_compositor.comp`, `shaders/compute/brush_painter.comp`, `shaders/visual/canvas.*` |
| Build config | `CMakeLists.txt`, `src/*/CMakeLists.txt`, `cmake/imgui.cmake` |
| External deps | `external/` (submodules; `stb` vendored directly) |