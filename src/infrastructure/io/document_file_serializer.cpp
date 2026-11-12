#include "document_file_serializer.hpp"
#include <glaze/glaze.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "domain/documents/document.hpp"
#include "infrastructure/rendering/shader.hpp"
#include "project_json_data.hpp"
#include "miniz.h"
#include "../canvas/gpu_canvas_layer.hpp"
#include "stb_image.h"
#include "stb_image_write.h"


namespace Infrastructure::IO {

namespace {

/**
 * @brief Copies an archive entry into memory.
 * Returns an empty buffer when the entry is missing or cannot be extracted.
 */
std::vector<std::uint8_t> extract_entry(mz_zip_archive& archive, const std::string& name) {
    size_t size = 0;
    void* data = mz_zip_reader_extract_file_to_heap(&archive, name.c_str(), &size, 0);
    if (data == nullptr)
        return {};

    std::vector<std::uint8_t> entry(static_cast<std::uint8_t*>(data),
                                    static_cast<std::uint8_t*>(data) + size);
    mz_free(data);
    return entry;
}

}

DocumentFileSerializer::DocumentFileSerializer(const Domain::Documents::CanvasFactory& canvas_factory)
 : canvas_factory(canvas_factory) {
}

std::shared_ptr<Domain::Documents::Document> DocumentFileSerializer::load_document(std::filesystem::path path) const {
    std::cout << "Loading document from path " << path << std::endl;

    // Open project file
    mz_zip_archive project_file{};
    if (!mz_zip_reader_init_file(&project_file, path.c_str(), 0)) {
        std::cerr << "Could not open project file " << path << ": "
                  << mz_zip_get_error_string(mz_zip_get_last_error(&project_file))
                  << std::endl;
        return nullptr;
    }

    // Make sure the archive handle is always released, including on early returns
    struct ZipReaderGuard {
        mz_zip_archive& archive;
        ~ZipReaderGuard() { mz_zip_reader_end(&archive); }
    } project_file_guard{project_file};

    // Read and parse the project metadata
    auto json_bytes = extract_entry(project_file, "project.json");
    if (json_bytes.empty()) {
        std::cerr << "Could not read project.json from " << path << ": "
                  << mz_zip_get_error_string(mz_zip_get_last_error(&project_file))
                  << std::endl;
        return nullptr;
    }
    const std::string json(json_bytes.begin(), json_bytes.end());
    ProjectJsonData json_data{};
    if (auto ec = glz::read<glz::opts{}>(json_data, json)) {
        std::cerr << "Could not parse project.json: " << glz::format_error(ec, json) << std::endl;
        return nullptr;
    }
    if (json_data.size_width == 0 || json_data.size_height == 0) {
        std::cerr << "Project file has an invalid canvas size" << std::endl;
        return nullptr;
    }

    // Create the document and remember the file it came from
    auto doc = std::make_shared<Domain::Documents::Document>(
        canvas_factory, glm::uvec2{json_data.size_width, json_data.size_height});
    doc->set_save_path(path);

    // Get canvas
    auto& canvas = doc->get_canvas();

    // Restore each layer, in the order they were saved
    const int width = static_cast<int>(json_data.size_width);
    const int height = static_cast<int>(json_data.size_height);
    for (const auto& layer_data : json_data.layers) {
        // Extract the encoded layer
        auto png_bytes = extract_entry(project_file, layer_data.file);
        if (png_bytes.empty()) {
            std::cerr << "Could not read " << layer_data.file << " from " << path << ": "
                      << mz_zip_get_error_string(mz_zip_get_last_error(&project_file))
                      << std::endl;
            return nullptr;
        }

        // Decode the PNG into tightly packed RGBA8
        int decoded_width = 0;
        int decoded_height = 0;
        int channels = 0;
        stbi_uc* raw_pixels = stbi_load_from_memory(
            png_bytes.data(), static_cast<int>(png_bytes.size()),
            &decoded_width, &decoded_height, &channels, 4);
        std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> pixels(raw_pixels, &stbi_image_free);
        if (raw_pixels == nullptr) {
            std::cerr << "Could not decode " << layer_data.file << ": "
                      << stbi_failure_reason() << std::endl;
            return nullptr;
        }
        if (decoded_width != width || decoded_height != height) {
            std::cerr << layer_data.file << " has size " << decoded_width << "x" << decoded_height
                      << ", expected " << width << "x" << height << std::endl;
            return nullptr;
        }

        // Create the layer and upload the decoded pixels
        auto layer = canvas.add_layer(layer_data.name).lock();
        auto gpu_layer = std::dynamic_pointer_cast<Canvas::GpuCanvasLayer>(layer);
        if (gpu_layer == nullptr) {
            std::cerr << "Failed to cast canvas layer to GPU canvas layer" << std::endl;
            return nullptr;
        }
        gpu_layer->upload_pixels({
            raw_pixels,
            static_cast<size_t>(decoded_width) * decoded_height * 4,
        });
    }

    return doc;
}

void DocumentFileSerializer::export_png(std::filesystem::path path, Domain::Documents::Document &document) const {
    std::cout << "Exporting document to path " << path << std::endl;

    // Get canvas
    auto& canvas = document.get_canvas();

    // Flatten all visible layers
    const glm::uvec2 canvas_size = canvas.get_size();
    auto pixels = canvas.read_composited_pixels();

    // The canvas holds premultiplied alpha, PNG stores straight alpha
    for (size_t i = 0; i < pixels.size(); i += 4) {
        const unsigned int alpha = pixels[i + 3];
        if (alpha == 0 || alpha == 255)
            continue;
        for (size_t channel = 0; channel < 3; channel++) {
            const unsigned int value = pixels[i + channel] * 255u / alpha;
            pixels[i + channel] = static_cast<std::uint8_t>(value > 255u ? 255u : value);
        }
    }

    // Encode and write the PNG
    if (!stbi_write_png(path.c_str(),
                        static_cast<int>(canvas_size.x),
                        static_cast<int>(canvas_size.y),
                        4, pixels.data(),
                        static_cast<std::ptrdiff_t>(canvas_size.x) * 4)) {
        std::cerr << "Could not write PNG to " << path << std::endl;
    }
}

void DocumentFileSerializer::save_document(std::filesystem::path path, Domain::Documents::Document &document) const {
    std::cout << "Saving document to path " << path << std::endl;

    // Get canvas
    auto& canvas = document.get_canvas();

    // Prepare json data from document
    ProjectJsonData json_data;
    glm::uvec2 canvas_size = canvas.get_size();
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
    int layers = canvas.get_layer_count();
    for (int i = 0; i < layers; i++) {
        // Get canvas layer
        auto layer = canvas.get_layer(i);
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
