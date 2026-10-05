#include "brush_tool.hpp"
#include "infrastructure/canvas/gpu_canvas.hpp"
#include "infrastructure/canvas/gpu_canvas_layer.hpp"
#include "infrastructure/compute/brush_painter.hpp"
#include <iostream>


namespace Infrastructure::Tools {

BrushTool::BrushTool(Application::Documents::DocumentService& doc_service) : doc_service(doc_service) {
}

BrushTool::~BrushTool() = default;

void BrushTool::start_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::start_stroke(position);

    if (!in_stroke)
        return;

    target_layer = resolve_target_layer();
    if (target_layer.expired()) {
        std::cerr << "BrushTool: no selected layer to paint on" << std::endl;
        return;
    }

    last_position = position;
    // A zero length segment paints a single dot
    paint_segment(position, position);
}

void BrushTool::continue_stroke(glm::vec2 position) {
    Domain::Tools::StrokeTool::continue_stroke(position);

    if (!in_stroke || target_layer.expired())
        return;

    paint_segment(last_position, position);
    last_position = position;
}

void BrushTool::stop_stroke(glm::vec2 position) {
    // Paint the final segment before the stroke ends
    if (in_stroke && !target_layer.expired())
        paint_segment(last_position, position);

    Domain::Tools::StrokeTool::stop_stroke(position);
    target_layer.reset();
}

std::weak_ptr<Canvas::GpuCanvasLayer> BrushTool::resolve_target_layer() {
    auto document = doc_service.get_active_document().lock();
    if (document == nullptr) {
        std::cerr << "BrushTool: no active document" << std::endl;
        return {};
    }

    // The domain only exposes the abstract Canvas, but every document in this
    // app is created through GpuCanvasFactory. dynamic_pointer_cast rather than
    // static so a different implementation fails loudly instead of corrupting.
    auto canvas = std::dynamic_pointer_cast<Canvas::GpuCanvas>(document->get_canvas().lock());
    if (canvas == nullptr) {
        std::cerr << "BrushTool: active document canvas is not a GpuCanvas" << std::endl;
        return {};
    }

    // Canvas::get_selected_layer hands back the abstract domain type. Every
    // layer a GpuCanvas owns is a GpuCanvasLayer, so this is safe.
    auto layer = canvas->get_selected_layer().lock();
    if (layer == nullptr) {
        std::cerr << "BrushTool: no layer selected" << std::endl;
        return {};
    }

    return std::static_pointer_cast<Canvas::GpuCanvasLayer>(layer);
}

void BrushTool::paint_segment(glm::vec2 from, glm::vec2 to) {
    auto layer = target_layer.lock();
    if (layer == nullptr)
        return;

    // The painter owns a GL shader program, so it cannot be built until an
    // OpenGL context exists. Tools are constructed in main() before
    // EditorRuntime::run() creates the window and calls gladLoadGL, so the
    // program is created lazily on first use instead. Strokes only begin inside
    // the event loop, so a context is guaranteed by now.
    if (brush_painter == nullptr)
        brush_painter = std::make_unique<Compute::BrushPainter>();

    brush_painter->paint(from, to, layer->get_read_texture(), layer->get_write_texture(), get_settings());
    // Make the freshly written buffer the layer's contents
    layer->switch_buffers();
}

}
