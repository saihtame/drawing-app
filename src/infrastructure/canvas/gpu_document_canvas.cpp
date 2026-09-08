#include "gpu_document_canvas.hpp"
#include <exception>
#include <iostream>


namespace Infrastructure::Canvas {

GpuDocumentCanvas::GpuDocumentCanvas(std::weak_ptr<Domain::Documents::Document> document)
 : document(document) {

}

void GpuDocumentCanvas::render(glm::ivec2 render_size) {
    if (document.expired()) {
        std::cerr << "GpuDocumentCanvas document expired" << std::endl;
        return;
    }

    // Check if we need to resize our frame buffers
    if (this->render_size != render_size) {
        try {
            resize(render_size);
        } catch (std::exception e) {
            std::cerr << "GpuDocumentCanvas failed to resize to "
                << render_size.x << ", " << render_size.y << std::endl;
            return;
        }
    }
}

inline void GpuDocumentCanvas::resize(glm::ivec2 new_size) {
    render_size = new_size;
}

}
