#pragma once
#include <list>
#include <memory>
#include <glm/ext/vector_int3.hpp>
#include <glm/vec2.hpp>
#include "layer.hpp"


namespace Domain::Documents {

class Document final {
public:
    Document(glm::uvec2 size);
    ~Document() = default;

public:
    int get_layer_count() const noexcept;
    std::weak_ptr<Layer> get_layer(int ind) const;
    void add_layer();
    void remove_layer(int ind);

private:
    std::list<std::shared_ptr<Layer>> layers;
    glm::uvec2 size;
};

}
