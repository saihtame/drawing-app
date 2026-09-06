#pragma once
#include <list>
#include <memory>
#include <glm/ext/vector_int3.hpp>
#include <glm/vec2.hpp>
#include "layer.hpp"


namespace Domain::Documents {

class Document {
public:
    Document(glm::uvec2 size);
    ~Document() = default;

public:
    int get_layer_count() const noexcept;
    Layer* get_layer(int ind) const;
    void add_layer();
    void remove_layer(int ind);

public: // Operators
    bool operator==(const Document&) const = default;

private:
    std::string document_name;
    std::list<std::unique_ptr<Layer>> layers;
    glm::uvec2 size;
};

}
