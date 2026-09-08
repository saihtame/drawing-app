#pragma once
#include "application/documents/document_canvas.hpp"
#include "domain/documents/document.hpp"
#include <glm/ext/vector_int2.hpp>


namespace Infrastructure::Canvas {

class GpuDocumentCanvas final : public Application::Documents::DocumentCanvas {
public: // Ctors
    GpuDocumentCanvas(std::weak_ptr<Domain::Documents::Document> document);
    ~GpuDocumentCanvas() = default;

public: // DocumentCanvas implementation
    virtual void render(glm::ivec2 render_size);

private:
    inline void resize(glm::ivec2 new_size);

private: // Members
    glm::ivec2 size;
    glm::ivec2 render_size = {0, 0};
    std::weak_ptr<Domain::Documents::Document> document;
};

}
