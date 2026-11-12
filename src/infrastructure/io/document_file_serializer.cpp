#include "document_file_serializer.hpp"
#include <glaze/glaze.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <memory>
#include <string>
#include "domain/documents/document.hpp"
#include "infrastructure/rendering/shader.hpp"
#include "project_json_data.hpp"
#include "miniz.h"
#include "../canvas/gpu_canvas_layer.hpp"
#include "stb_image_write.h"


namespace Infrastructure::IO {

DocumentFileSerializer::DocumentFileSerializer(const Domain::Documents::CanvasFactory& canvas_factory)
 : canvas_factory(canvas_factory) {
}

std::shared_ptr<Domain::Documents::Document> DocumentFileSerializer::load_document(std::filesystem::path path) const {
    std::cout << "Loading document from path " << path << std::endl;
    auto doc = std::make_shared<Domain::Documents::Document>(canvas_factory, glm::uvec2());
    return doc;
}

void DocumentFileSerializer::save_document(std::filesystem::path path, const std::weak_ptr<Domain::Documents::Document> &document) const {
    std::cout << "Saving document to path " << path << std::endl;

    // Lock document
    auto doc_locked = document.lock();
    if (doc_locked == nullptr) {
        std::cerr << "Could not save document. Null document." << std::endl;
        return;
    }

    // Get canvas
    auto canvas = doc_locked->get_canvas();
    auto canvas_locked = canvas.lock();
    if (canvas_locked == nullptr) {
        std::cerr << "Could not save document. Null canvas." << std::endl;
        return;
    }

    // Prepare json data from document
    ProjectJsonData json_data;
    glm::uvec2 canvas_size = canvas_locked->get_size();
    json_data.size_width = canvas_size.x;
    json_data.size_height= canvas_size.y;

    // Create project file
    mz_zip_archive project_file{};
    if (!mz_zip_writer_init_file(&project_file, path.c_str(), 0)) {
        std::cerr << "Could not create project file " << path << ": "
                  << mz_zip_get_error_string(mz_zip_get_last_error(&project_file))
                  << std::endl;
        return;
    }

    // Make sure the archive handle is always released, including on early returns
    struct ZipWriterGuard {
        mz_zip_archive& archive;
        ~ZipWriterGuard() { mz_zip_writer_end(&archive); }
    } project_file_guard{project_file};

    // Compress each canvas layer
    const int width = static_cast<int>(canvas_size.x);
    const int height = static_cast<int>(canvas_size.y);
    const std::ptrdiff_t row_stride = static_cast<std::ptrdiff_t>(canvas_size.x) * 4;
    int layers = canvas_locked->get_layer_count();
    for (int i = 0; i < layers; i++) {
        // Get canvas layer
        auto layer = canvas_locked->get_layer(i);
        auto layer_locked = layer.lock();
        if (layer_locked == nullptr) {
            std::cerr << "Failed to lock layer" << std::endl;
            return;
        }
        // Cast to GpuCanvasLayer
        auto gpu_layer = std::dynamic_pointer_cast<Canvas::GpuCanvasLayer>(layer_locked);
        if (gpu_layer == nullptr) {
            std::cerr << "Failed to cast canvas layer to GPU canvas layer" << std::endl;
            return;
        }
        // Get texture
        auto& texture = gpu_layer->get_texture();
        std::vector<unsigned char> pixels(static_cast<size_t>(canvas_size.x) * canvas_size.y * 4);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);   // avoids row-padding surprises
        glGetTextureImage(texture.get_id(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                          (GLsizei)pixels.size(), pixels.data());

        // Encode the texture as PNG. The encoded bytes go into a buffer of their
        // own: using `pixels` as both the context and the source would make the
        // callback append to the buffer stb is still reading from.
        std::vector<unsigned char> png_bytes;
        int encode_ok = stbi_write_png_to_func(
            [](void* ctx, void* data, int size) {
                auto* out = static_cast<std::vector<unsigned char>*>(ctx);
                auto* bytes = static_cast<unsigned char*>(data);
                out->insert(out->end(), bytes, bytes + size);
            },
            &png_bytes, width, height, 4, pixels.data(), row_stride);
        if (!encode_ok || png_bytes.empty()) {
            std::cerr << "Failed to encode layer " << i << " as PNG" << std::endl;
            return;
        }

        // Add the encoded layer to the project file
        const std::string entry_name = "layer_" + std::to_string(i) + ".png";
        if (!mz_zip_writer_add_mem(&project_file, entry_name.c_str(),
                                   png_bytes.data(), png_bytes.size(),
                                   MZ_DEFAULT_COMPRESSION)) {
            std::cerr << "Could not add " << entry_name << " to project file" << std::endl;
            return;
        }

        // Record the layer in the project metadata
        json_data.layers.push_back({
            .name = layer_locked->get_name(),
            .file = entry_name,
        });
    }

    // Convert to json and add it to the project file
    std::string json;
    auto ec = glz::write<glz::opts{.prettify = true}>(json_data, json);
    if (ec) {
        std::cerr << "JSON write failed\n";
        return;
    }
    if (!mz_zip_writer_add_mem(&project_file, "project.json",
                               json.data(), json.size(),
                               MZ_DEFAULT_COMPRESSION)) {
        std::cerr << "Could not add project.json to project file" << std::endl;
        return;
    }

    // Write the archive's central directory to disk
    if (!mz_zip_writer_finalize_archive(&project_file)) {
        std::cerr << "Could not finalize project file " << path << std::endl;
        return;
    }
}

}
